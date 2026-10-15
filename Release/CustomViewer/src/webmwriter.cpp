#include "webmwriter.h"

#include <QThread>

#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

#include <cstring>

namespace {
// Matroska / EBML element ids (with their length marker bits).
constexpr quint32 kEbml = 0x1A45DFA3;
constexpr quint32 kEbmlVersion = 0x4286;
constexpr quint32 kEbmlReadVersion = 0x42F7;
constexpr quint32 kEbmlMaxIdLength = 0x42F2;
constexpr quint32 kEbmlMaxSizeLength = 0x42F3;
constexpr quint32 kDocType = 0x4282;
constexpr quint32 kDocTypeVersion = 0x4287;
constexpr quint32 kDocTypeReadVersion = 0x4285;
constexpr quint32 kSegment = 0x18538067;
constexpr quint32 kSeekHead = 0x114D9B74;
constexpr quint32 kSeek = 0x4DBB;
constexpr quint32 kSeekId = 0x53AB;
constexpr quint32 kSeekPosition = 0x53AC;
constexpr quint32 kInfo = 0x1549A966;
constexpr quint32 kTimecodeScale = 0x2AD7B1;
constexpr quint32 kDuration = 0x4489;
constexpr quint32 kMuxingApp = 0x4D80;
constexpr quint32 kWritingApp = 0x5741;
constexpr quint32 kTracks = 0x1654AE6B;
constexpr quint32 kTrackEntry = 0xAE;
constexpr quint32 kTrackNumber = 0xD7;
constexpr quint32 kTrackUid = 0x73C5;
constexpr quint32 kTrackType = 0x83;
constexpr quint32 kFlagLacing = 0x9C;
constexpr quint32 kCodecId = 0x86;
constexpr quint32 kName = 0x536E;
constexpr quint32 kVideo = 0xE0;
constexpr quint32 kPixelWidth = 0xB0;
constexpr quint32 kPixelHeight = 0xBA;
constexpr quint32 kCluster = 0x1F43B675;
constexpr quint32 kTimecode = 0xE7;
constexpr quint32 kSimpleBlock = 0xA3;
constexpr quint32 kCues = 0x1C53BB6B;
constexpr quint32 kCuePoint = 0xBB;
constexpr quint32 kCueTime = 0xB3;
constexpr quint32 kCueTrackPositions = 0xB7;
constexpr quint32 kCueTrack = 0xF7;
constexpr quint32 kCueClusterPosition = 0xF1;

// An "unknown"-style 8-byte size, patched once the real size is known.
const QByteArray kSizePlaceholder = QByteArray::fromHex("01FFFFFFFFFFFFFF");

QByteArray encodeId(quint32 id)
{
    QByteArray out;
    int bytes = id > 0xFFFFFF ? 4 : id > 0xFFFF ? 3 : id > 0xFF ? 2 : 1;
    for (int i = bytes - 1; i >= 0; --i) {
        out.append(char((id >> (8 * i)) & 0xFF));
    }
    return out;
}

QByteArray encodeSize(quint64 size)
{
    int length = 1;
    while (length < 8 && size >= (quint64(1) << (7 * length)) - 1) {
        ++length;
    }
    QByteArray out(length, '\0');
    for (int i = length - 1; i >= 0; --i) {
        out[i] = char(size & 0xFF);
        size >>= 8;
    }
    out[0] = char(quint8(out.at(0)) | quint8(0x80 >> (length - 1)));
    return out;
}

QByteArray encodeSize8(quint64 size)
{
    QByteArray out(8, '\0');
    out[0] = char(0x01);
    for (int i = 7; i >= 1; --i) {
        out[i] = char(size & 0xFF);
        size >>= 8;
    }
    return out;
}

QByteArray element(quint32 id, const QByteArray &payload)
{
    return encodeId(id) + encodeSize(quint64(payload.size())) + payload;
}

QByteArray uintElement(quint32 id, quint64 value)
{
    QByteArray payload;
    do {
        payload.prepend(char(value & 0xFF));
        value >>= 8;
    } while (value != 0);
    return element(id, payload);
}

QByteArray uint8Element(quint32 id, quint64 value) // fixed 8 bytes, patchable
{
    QByteArray payload(8, '\0');
    for (int i = 7; i >= 0; --i) {
        payload[i] = char(value & 0xFF);
        value >>= 8;
    }
    return element(id, payload);
}

QByteArray floatElement(quint32 id, double value)
{
    quint64 bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return uint8Element(id, bits);
}

QByteArray stringElement(quint32 id, const QString &value)
{
    return element(id, value.toUtf8());
}

inline unsigned char clamp8(int v)
{
    return static_cast<unsigned char>(v < 0 ? 0 : (v > 255 ? 255 : v));
}

// QImage -> I420 (BT.601, 2x2-averaged chroma), the same conversion
// KikiHost's recorder uses.
void imageToI420(const QImage &src, vpx_image_t *img, int w, int h)
{
    const QImage rgb = src.convertToFormat(QImage::Format_RGB888);
    unsigned char *yp = img->planes[VPX_PLANE_Y];
    unsigned char *up = img->planes[VPX_PLANE_U];
    unsigned char *vp = img->planes[VPX_PLANE_V];
    const int ys = img->stride[VPX_PLANE_Y];
    const int us = img->stride[VPX_PLANE_U];
    const int vs = img->stride[VPX_PLANE_V];
    for (int y = 0; y < h; ++y) {
        const unsigned char *row = rgb.constScanLine(y);
        for (int x = 0; x < w; ++x) {
            const unsigned char *p = row + x * 3;
            yp[y * ys + x] = clamp8((299 * p[0] + 587 * p[1] + 114 * p[2]) / 1000);
        }
    }
    for (int cy = 0; cy < h / 2; ++cy) {
        const unsigned char *r0 = rgb.constScanLine(cy * 2);
        const unsigned char *r1 = rgb.constScanLine(cy * 2 + 1);
        for (int cx = 0; cx < w / 2; ++cx) {
            const int x = cx * 2;
            const int r = (r0[x * 3] + r0[(x + 1) * 3] + r1[x * 3] + r1[(x + 1) * 3]) / 4;
            const int g = (r0[x * 3 + 1] + r0[(x + 1) * 3 + 1] + r1[x * 3 + 1] + r1[(x + 1) * 3 + 1]) / 4;
            const int b = (r0[x * 3 + 2] + r0[(x + 1) * 3 + 2] + r1[x * 3 + 2] + r1[(x + 1) * 3 + 2]) / 4;
            up[cy * us + cx] = clamp8((-169 * r - 331 * g + 500 * b) / 1000 + 128);
            vp[cy * vs + cx] = clamp8((500 * r - 419 * g - 81 * b) / 1000 + 128);
        }
    }
}
} // namespace

struct WebmWriter::Encoder {
    vpx_codec_ctx_t ctx {};
    vpx_image_t img {};
    bool imageAllocated = false;
    bool codecOpen = false;
    unsigned long deadline = VPX_DL_GOOD_QUALITY;
};

WebmWriter::WebmWriter() = default;

WebmWriter::~WebmWriter()
{
    abort();
}

bool WebmWriter::fail(const QString &message)
{
    m_error = message;
    abort();
    return false;
}

bool WebmWriter::open(const QString &path, int width, int height, Quality quality, const QString &title)
{
    abort();
    m_error.clear();
    m_width = width;
    m_height = height;
    m_cues.clear();
    m_clusterStart = -1;
    m_lastTimestamp = -1;
    m_endTimestamp = 0;

    // PlayerVideoSaver's encoder: VP8, ms timebase, every core, a keyframe
    // at least every 10 frames, 256 kbps per 320x240 of picture; quality
    // only picks the encode deadline (realtime / good / best) and, below
    // best, VP8 profile 3.
    m_encoder = new Encoder;
    vpx_codec_enc_cfg_t cfg {};
    const vpx_codec_err_t configError = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
    if (configError != VPX_CODEC_OK) {
        return fail(QStringLiteral("failed to get enc config: %1")
                        .arg(QString::fromUtf8(vpx_codec_err_to_string(configError))));
    }
    cfg.g_w = static_cast<unsigned int>(width);
    cfg.g_h = static_cast<unsigned int>(height);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 1000;
    cfg.g_threads = static_cast<unsigned int>(qMax(1, QThread::idealThreadCount()));
    cfg.kf_max_dist = 10;
    cfg.rc_target_bitrate = static_cast<unsigned int>(256LL * width * height / 320 / 240);
    if (quality != Quality::Best) {
        cfg.g_profile = 3;
    }
    m_encoder->deadline = quality == Quality::Poor ? VPX_DL_REALTIME
                        : quality == Quality::Best ? VPX_DL_BEST_QUALITY
                                                   : VPX_DL_GOOD_QUALITY;
    const vpx_codec_err_t initError = vpx_codec_enc_init(&m_encoder->ctx, vpx_codec_vp8_cx(), &cfg, 0);
    if (initError != VPX_CODEC_OK) {
        return fail(QStringLiteral("failed to init encoder: %1")
                        .arg(QString::fromUtf8(vpx_codec_err_to_string(initError))));
    }
    m_encoder->codecOpen = true;
    if (!vpx_img_alloc(&m_encoder->img, VPX_IMG_FMT_I420, static_cast<unsigned int>(width),
                       static_cast<unsigned int>(height), 1)) {
        return fail(QStringLiteral("failed to allocate image"));
    }
    m_encoder->imageAllocated = true;

    // The original deletes whatever was there first.
    QFile::remove(path);
    m_file.setFileName(path);
    if (!m_file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        return fail(QStringLiteral("Unable to save file %1. Choose another directory.").arg(path));
    }

    QByteArray header = element(kEbml, uintElement(kEbmlVersion, 1) + uintElement(kEbmlReadVersion, 1)
                                           + uintElement(kEbmlMaxIdLength, 4) + uintElement(kEbmlMaxSizeLength, 8)
                                           + stringElement(kDocType, QStringLiteral("webm"))
                                           + uintElement(kDocTypeVersion, 2) + uintElement(kDocTypeReadVersion, 2));
    header += encodeId(kSegment);
    m_segmentSizePos = header.size();
    header += kSizePlaceholder;
    m_segmentDataStart = header.size();

    // SeekHead: Info, Tracks and Cues, each position patchable (8 bytes).
    const auto seek = [](quint32 id) {
        return element(kSeek, element(kSeekId, encodeId(id)) + uint8Element(kSeekPosition, 0));
    };
    const QByteArray seekHead = element(kSeekHead, seek(kInfo) + seek(kTracks) + seek(kCues));
    const qint64 seekHeadPos = header.size();
    header += seekHead;
    // Where each SeekPosition's 8 value bytes sit in the file: the three
    // Seeks are the same size (4-byte ids) and each ends with those bytes.
    const qint64 seekEntrySize = seek(kInfo).size();
    const qint64 seekPayloadStart = seekHeadPos + seekHead.size() - 3 * seekEntrySize;
    const qint64 infoSeekValue = seekPayloadStart + seekEntrySize - 8;
    const qint64 tracksSeekValue = infoSeekValue + seekEntrySize;
    m_cuesSeekPos = tracksSeekValue + seekEntrySize;

    const qint64 infoPos = header.size();
    QByteArray info = uintElement(kTimecodeScale, 1000000) + stringElement(kMuxingApp, QStringLiteral("KikiViewer"))
                      + stringElement(kWritingApp, QStringLiteral("kickidler"));
    const qint64 durationValueInInfo = info.size() + encodeId(kDuration).size() + 1; // size byte of an 8-byte float
    info += floatElement(kDuration, 0.0);
    const QByteArray infoElement = element(kInfo, info);
    m_durationPos = infoPos + (infoElement.size() - info.size()) + durationValueInInfo;
    header += infoElement;

    const qint64 tracksPos = header.size();
    const QByteArray video = uintElement(kPixelWidth, quint64(width)) + uintElement(kPixelHeight, quint64(height));
    QByteArray track = uintElement(kTrackNumber, 1) + uintElement(kTrackUid, 1) + uintElement(kTrackType, 1)
                       + uintElement(kFlagLacing, 0) + stringElement(kCodecId, QStringLiteral("V_VP8"));
    if (!title.isEmpty()) {
        track += stringElement(kName, title); // the trackName argument of start()
    }
    track += element(kVideo, video);
    header += element(kTracks, element(kTrackEntry, track));

    const auto putUint8 = [&header](qint64 pos, quint64 value) {
        for (int i = 7; i >= 0; --i) {
            header[int(pos + i)] = char(value & 0xFF);
            value >>= 8;
        }
    };
    putUint8(infoSeekValue, quint64(infoPos - m_segmentDataStart));
    putUint8(tracksSeekValue, quint64(tracksPos - m_segmentDataStart));

    if (m_file.write(header) != header.size()) {
        return fail(QStringLiteral("failed to write header to file: %1").arg(path));
    }
    return true;
}

void WebmWriter::startCluster(qint64 timestampMs)
{
    closeCluster();
    m_clusterStart = m_file.pos();
    m_clusterTimecode = timestampMs;
    QByteArray head = encodeId(kCluster);
    m_clusterSizePos = m_clusterStart + head.size();
    head += kSizePlaceholder;
    head += uintElement(kTimecode, quint64(timestampMs));
    m_file.write(head);
}

void WebmWriter::closeCluster()
{
    if (m_clusterStart < 0) {
        return;
    }
    const qint64 end = m_file.pos();
    m_file.seek(m_clusterSizePos);
    m_file.write(encodeSize8(quint64(end - m_clusterSizePos - 8)));
    m_file.seek(end);
    m_clusterStart = -1;
}

bool WebmWriter::writeBlock(const QByteArray &frame, qint64 timestampMs, bool key)
{
    // Muxed timestamps must grow strictly (else previous + 1 ms).
    if (timestampMs <= m_lastTimestamp) {
        timestampMs = m_lastTimestamp + 1;
    }
    m_lastTimestamp = timestampMs;
    if (m_clusterStart < 0 || key || timestampMs - m_clusterTimecode > 32767) {
        startCluster(timestampMs);
        if (key) {
            m_cues.append({timestampMs, m_clusterStart - m_segmentDataStart});
        }
    }
    QByteArray payload;
    payload.append(char(0x81)); // track 1
    const qint16 relative = qint16(timestampMs - m_clusterTimecode);
    payload.append(char((relative >> 8) & 0xFF));
    payload.append(char(relative & 0xFF));
    payload.append(char(key ? 0x80 : 0x00));
    payload.append(frame);
    const QByteArray block = element(kSimpleBlock, payload);
    if (m_file.write(block) != block.size()) {
        m_error = QStringLiteral("failed to write block to file: %1").arg(m_file.fileName());
        return false;
    }
    return true;
}

bool WebmWriter::drain()
{
    const vpx_codec_cx_pkt_t *packet = nullptr;
    vpx_codec_iter_t iterator = nullptr;
    while ((packet = vpx_codec_get_cx_data(&m_encoder->ctx, &iterator)) != nullptr) {
        if (packet->kind != VPX_CODEC_CX_FRAME_PKT) {
            continue;
        }
        const QByteArray data(static_cast<const char *>(packet->data.frame.buf),
                              static_cast<int>(packet->data.frame.sz));
        if (!writeBlock(data, static_cast<qint64>(packet->data.frame.pts),
                        (packet->data.frame.flags & VPX_FRAME_IS_KEY) != 0)) {
            return false;
        }
    }
    return true;
}

bool WebmWriter::addFrame(const QImage &image, qint64 timestampMs, qint64 durationMs)
{
    if (!m_encoder || !m_file.isOpen()) {
        m_error = QStringLiteral("failed to render a frame");
        return false;
    }
    QImage frame = image;
    if (frame.width() != m_width || frame.height() != m_height) {
        frame = frame.scaled(m_width, m_height);
    }
    if (frame.isNull()) {
        return fail(QStringLiteral("failed to convert image to YUV"));
    }
    imageToI420(frame, &m_encoder->img, m_width, m_height);
    const vpx_codec_err_t error = vpx_codec_encode(&m_encoder->ctx, &m_encoder->img, timestampMs,
                                                   static_cast<unsigned long>(qMax<qint64>(1, durationMs)), 0,
                                                   m_encoder->deadline);
    if (error != VPX_CODEC_OK) {
        return fail(QStringLiteral("encoding failed: %1").arg(QString::fromUtf8(vpx_codec_err_to_string(error))));
    }
    m_endTimestamp = qMax(m_endTimestamp, timestampMs + durationMs);
    if (!drain()) {
        const QString message = m_error;
        return fail(message);
    }
    return true;
}

bool WebmWriter::finish()
{
    if (!m_encoder || !m_file.isOpen()) {
        return false;
    }
    // Flush: NULL image at the end time.
    const vpx_codec_err_t error = vpx_codec_encode(&m_encoder->ctx, nullptr, m_endTimestamp, 1, 0,
                                                   m_encoder->deadline);
    if (error != VPX_CODEC_OK) {
        return fail(QStringLiteral("encoding failed: %1").arg(QString::fromUtf8(vpx_codec_err_to_string(error))));
    }
    if (!drain()) {
        const QString message = m_error;
        return fail(message);
    }
    closeCluster();

    const qint64 cuesPos = m_file.pos();
    QByteArray cues;
    for (const Cue &cue : std::as_const(m_cues)) {
        cues += element(kCuePoint, uintElement(kCueTime, quint64(cue.timeMs))
                                       + element(kCueTrackPositions,
                                                 uintElement(kCueTrack, 1)
                                                     + uintElement(kCueClusterPosition, quint64(cue.clusterPosition))));
    }
    if (!cues.isEmpty()) {
        m_file.write(element(kCues, cues));
    }
    const qint64 end = m_file.pos();

    const auto patch = [this](qint64 pos, const QByteArray &bytes) {
        m_file.seek(pos);
        return m_file.write(bytes) == bytes.size();
    };
    QByteArray cuesPosition(8, '\0');
    {
        quint64 value = cues.isEmpty() ? 0 : quint64(cuesPos - m_segmentDataStart);
        for (int i = 7; i >= 0; --i) {
            cuesPosition[i] = char(value & 0xFF);
            value >>= 8;
        }
    }
    double duration = double(m_endTimestamp);
    quint64 durationBits = 0;
    std::memcpy(&durationBits, &duration, sizeof durationBits);
    QByteArray durationBytes(8, '\0');
    for (int i = 7; i >= 0; --i) {
        durationBytes[i] = char(durationBits & 0xFF);
        durationBits >>= 8;
    }
    const bool ok = patch(m_cuesSeekPos, cuesPosition) && patch(m_durationPos, durationBytes)
                    && patch(m_segmentSizePos, encodeSize8(quint64(end - m_segmentDataStart)));
    m_file.seek(end);
    if (!ok || !m_file.flush()) {
        return fail(QStringLiteral("output file corrupted"));
    }
    m_file.close();
    releaseEncoder();
    return true;
}

void WebmWriter::releaseEncoder()
{
    if (!m_encoder) {
        return;
    }
    if (m_encoder->imageAllocated) {
        vpx_img_free(&m_encoder->img);
    }
    if (m_encoder->codecOpen) {
        vpx_codec_destroy(&m_encoder->ctx);
    }
    delete m_encoder;
    m_encoder = nullptr;
}

void WebmWriter::abort()
{
    releaseEncoder();
    if (m_file.isOpen()) {
        // A file that was not finished is not a video: closed and removed.
        m_file.close();
        m_file.remove();
    }
}
