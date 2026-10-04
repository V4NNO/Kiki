#include "vp8codec.h"

#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

#include <algorithm>
#include <cstring>

namespace {
inline unsigned char clamp8(int v)
{
    return static_cast<unsigned char>(v < 0 ? 0 : (v > 255 ? 255 : v));
}

// QImage (any format) -> I420 planes of a vpx_image_t (BT.601, 2x2-averaged
// chroma). The image is expected to already be w x h (even).
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
            const int g =
                (r0[x * 3 + 1] + r0[(x + 1) * 3 + 1] + r1[x * 3 + 1] + r1[(x + 1) * 3 + 1]) / 4;
            const int b =
                (r0[x * 3 + 2] + r0[(x + 1) * 3 + 2] + r1[x * 3 + 2] + r1[(x + 1) * 3 + 2]) / 4;
            up[cy * us + cx] = clamp8((-169 * r - 331 * g + 500 * b) / 1000 + 128);
            vp[cy * vs + cx] = clamp8((500 * r - 419 * g - 81 * b) / 1000 + 128);
        }
    }
}

QImage i420ToImage(const vpx_image_t *img)
{
    const int w = static_cast<int>(img->d_w);
    const int h = static_cast<int>(img->d_h);
    QImage out(w, h, QImage::Format_RGB888);
    const unsigned char *yp = img->planes[VPX_PLANE_Y];
    const unsigned char *up = img->planes[VPX_PLANE_U];
    const unsigned char *vp = img->planes[VPX_PLANE_V];
    const int ys = img->stride[VPX_PLANE_Y];
    const int us = img->stride[VPX_PLANE_U];
    const int vs = img->stride[VPX_PLANE_V];
    for (int y = 0; y < h; ++y) {
        unsigned char *dst = out.scanLine(y);
        for (int x = 0; x < w; ++x) {
            const int Y = yp[y * ys + x];
            const int U = up[(y / 2) * us + (x / 2)] - 128;
            const int V = vp[(y / 2) * vs + (x / 2)] - 128;
            dst[x * 3] = clamp8(Y + (1402 * V) / 1000);
            dst[x * 3 + 1] = clamp8(Y - (344 * U) / 1000 - (714 * V) / 1000);
            dst[x * 3 + 2] = clamp8(Y + (1772 * U) / 1000);
        }
    }
    return out;
}
} // namespace

struct Vp8Encoder::Impl {
    vpx_codec_ctx_t ctx {};
    vpx_image_t img {};
    long pts = 0;
    bool imgAllocated = false;
};

Vp8Encoder::Vp8Encoder() : m_d(new Impl) {}

Vp8Encoder::~Vp8Encoder()
{
    end();
    delete m_d;
}

bool Vp8Encoder::begin(int width, int height)
{
    end();
    const int w = width & ~1;  // VP8 needs even dimensions
    const int h = height & ~1;
    if (w <= 0 || h <= 0) {
        return false;
    }
    vpx_codec_enc_cfg_t cfg {};
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0)) {
        return false;
    }
    cfg.g_w = static_cast<unsigned int>(w);
    cfg.g_h = static_cast<unsigned int>(h);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;  // emit each frame's packet synchronously
    cfg.g_threads = 2;
    cfg.g_error_resilient = 0;
    // Constant-quality: a screenshot timeline wants steady visual quality
    // regardless of how much moved, not a fixed bitrate. The high ceiling
    // only caps pathological frames.
    cfg.rc_end_usage = VPX_CQ;
    cfg.rc_target_bitrate = static_cast<unsigned int>(
        std::min<long long>(20000, static_cast<long long>(w) * h * 30 / 50000));
    cfg.rc_min_quantizer = 4;
    cfg.rc_max_quantizer = 56;
    cfg.kf_mode = VPX_KF_DISABLED;  // we force keyframes ourselves
    if (vpx_codec_enc_init(&m_d->ctx, vpx_codec_vp8_cx(), &cfg, 0)) {
        return false;
    }
    vpx_codec_control(&m_d->ctx, VP8E_SET_CQ_LEVEL, 26);
    vpx_codec_control(&m_d->ctx, VP8E_SET_CPUUSED, 4);
    if (!vpx_img_alloc(&m_d->img, VPX_IMG_FMT_I420, w, h, 1)) {
        vpx_codec_destroy(&m_d->ctx);
        return false;
    }
    m_d->imgAllocated = true;
    m_d->pts = 0;
    m_width = w;
    m_height = h;
    m_open = true;
    return true;
}

QByteArray Vp8Encoder::encode(const QImage &image, bool forceKey, bool *isKey)
{
    if (isKey) {
        *isKey = false;
    }
    if (!m_open || image.isNull()) {
        return {};
    }
    QImage frame = image;
    if (frame.width() != m_width || frame.height() != m_height) {
        frame = frame.copy(0, 0, m_width, m_height);
        if (frame.width() != m_width || frame.height() != m_height) {
            frame = frame.scaled(m_width, m_height);
        }
    }
    imageToI420(frame, &m_d->img, m_width, m_height);

    const vpx_enc_frame_flags_t flags = forceKey ? VPX_EFLAG_FORCE_KF : 0;
    if (vpx_codec_encode(&m_d->ctx, &m_d->img, m_d->pts, 1, flags, VPX_DL_GOOD_QUALITY)) {
        return {};
    }
    ++m_d->pts;

    QByteArray out;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    vpx_codec_iter_t it = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&m_d->ctx, &it)) != nullptr) {
        if (pkt->kind != VPX_CODEC_CX_FRAME_PKT) {
            continue;
        }
        out = QByteArray(static_cast<const char *>(pkt->data.frame.buf),
                         static_cast<int>(pkt->data.frame.sz));
        if (isKey) {
            *isKey = (pkt->data.frame.flags & VPX_FRAME_IS_KEY) != 0;
        }
    }
    return out;
}

void Vp8Encoder::end()
{
    if (m_d->imgAllocated) {
        vpx_img_free(&m_d->img);
        m_d->imgAllocated = false;
    }
    if (m_open) {
        vpx_codec_destroy(&m_d->ctx);
        m_open = false;
    }
    m_width = 0;
    m_height = 0;
}

QImage Vp8::decodeLast(const QList<QByteArray> &frames)
{
    if (frames.isEmpty()) {
        return {};
    }
    vpx_codec_ctx_t dec {};
    if (vpx_codec_dec_init(&dec, vpx_codec_vp8_dx(), nullptr, 0)) {
        return {};
    }
    QImage last;
    for (const QByteArray &frame : frames) {
        if (frame.isEmpty()) {
            continue;
        }
        if (vpx_codec_decode(&dec, reinterpret_cast<const uint8_t *>(frame.constData()),
                             static_cast<unsigned int>(frame.size()), nullptr, 0)) {
            break;
        }
        vpx_codec_iter_t it = nullptr;
        const vpx_image_t *img = nullptr;
        while ((img = vpx_codec_get_frame(&dec, &it)) != nullptr) {
            last = i420ToImage(img);
        }
    }
    vpx_codec_destroy(&dec);
    return last;
}

struct Vp8Decoder::Impl {
    vpx_codec_ctx_t ctx {};
};

Vp8Decoder::Vp8Decoder() : m_d(new Impl) {}

Vp8Decoder::~Vp8Decoder()
{
    end();
    delete m_d;
}

bool Vp8Decoder::begin()
{
    end();
    if (vpx_codec_dec_init(&m_d->ctx, vpx_codec_vp8_dx(), nullptr, 0)) {
        return false;
    }
    m_open = true;
    return true;
}

void Vp8Decoder::end()
{
    if (m_open) {
        vpx_codec_destroy(&m_d->ctx);
        m_open = false;
    }
}

QImage Vp8Decoder::decode(const QByteArray &frame)
{
    if (!m_open || frame.isEmpty()) {
        return {};
    }
    if (vpx_codec_decode(&m_d->ctx, reinterpret_cast<const uint8_t *>(frame.constData()),
                         static_cast<unsigned int>(frame.size()), nullptr, 0)) {
        return {};
    }
    QImage last;
    vpx_codec_iter_t it = nullptr;
    const vpx_image_t *img = nullptr;
    while ((img = vpx_codec_get_frame(&m_d->ctx, &it)) != nullptr) {
        last = i420ToImage(img);
    }
    return last;
}
