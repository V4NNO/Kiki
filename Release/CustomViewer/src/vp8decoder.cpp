#include "vp8decoder.h"

#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>

#include <algorithm>

namespace {
// One output pixel from its luma and the chroma offsets it shares with its
// 2x2 block. Plain integer arithmetic (no table lookups), so the compiler can
// keep it in registers and vectorize the row loops.
inline quint32 packPixel(int y, int rOff, int gOff, int bOff)
{
    const int r = std::clamp(y + rOff, 0, 255);
    const int g = std::clamp(y - gOff, 0, 255);
    const int b = std::clamp(y + bOff, 0, 255);
    return 0xff000000u | (quint32(r) << 16) | (quint32(g) << 8) | quint32(b);
}

// I420 -> RGB32, BT.601 full range (the inverse of KikiHost's imageToI420),
// with 8-bit fixed-point coefficients:
//   R = Y + 1.402 V          -> 359/256
//   G = Y - 0.344 U - 0.714 V -> 88/256, 183/256
//   B = Y + 1.772 U          -> 454/256
// Each chroma sample covers a 2x2 block, so it is computed once and applied to
// four pixels across two rows. The previous per-pixel version cost ~13-27 ms
// for a 1920x1200 frame -- more than the VP8 decode itself and the bulk of
// what made a drag stutter.
QImage i420ToImage(const vpx_image_t *img)
{
    const int w = static_cast<int>(img->d_w);
    const int h = static_cast<int>(img->d_h);
    QImage out(w, h, QImage::Format_RGB32);
    const unsigned char *yPlane = img->planes[VPX_PLANE_Y];
    const unsigned char *uPlane = img->planes[VPX_PLANE_U];
    const unsigned char *vPlane = img->planes[VPX_PLANE_V];
    const int yStride = img->stride[VPX_PLANE_Y];
    const int uStride = img->stride[VPX_PLANE_U];
    const int vStride = img->stride[VPX_PLANE_V];
    const int pairs = w / 2;

    for (int y = 0; y < h; y += 2) {
        const bool twoRows = y + 1 < h;
        const unsigned char *y0 = yPlane + y * yStride;
        const unsigned char *y1 = twoRows ? y0 + yStride : y0;
        const unsigned char *uRow = uPlane + (y / 2) * uStride;
        const unsigned char *vRow = vPlane + (y / 2) * vStride;
        quint32 *d0 = reinterpret_cast<quint32 *>(out.scanLine(y));
        quint32 *d1 = twoRows ? reinterpret_cast<quint32 *>(out.scanLine(y + 1)) : d0;

        for (int c = 0; c < pairs; ++c) {
            const int u = int(uRow[c]) - 128;
            const int v = int(vRow[c]) - 128;
            const int rOff = (359 * v) >> 8;
            const int gOff = (88 * u + 183 * v) >> 8;
            const int bOff = (454 * u) >> 8;
            const int x = c * 2;
            d0[x] = packPixel(y0[x], rOff, gOff, bOff);
            d0[x + 1] = packPixel(y0[x + 1], rOff, gOff, bOff);
            d1[x] = packPixel(y1[x], rOff, gOff, bOff);
            d1[x + 1] = packPixel(y1[x + 1], rOff, gOff, bOff);
        }
        // An odd width leaves one column without a partner.
        if (w & 1) {
            const int c = pairs;
            const int u = int(uRow[c]) - 128;
            const int v = int(vRow[c]) - 128;
            const int rOff = (359 * v) >> 8;
            const int gOff = (88 * u + 183 * v) >> 8;
            const int bOff = (454 * u) >> 8;
            d0[w - 1] = packPixel(y0[w - 1], rOff, gOff, bOff);
            d1[w - 1] = packPixel(y1[w - 1], rOff, gOff, bOff);
        }
    }
    return out;
}
} // namespace

struct Vp8Decoder::Impl {
    vpx_codec_ctx_t ctx {};
    // The last frame libvpx produced; owned by the codec context and valid
    // until the next vpx_codec_decode().
    const vpx_image_t *last = nullptr;
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
    m_d->last = nullptr;
}

bool Vp8Decoder::feed(const QByteArray &frame)
{
    m_d->last = nullptr;
    if (!m_open || frame.isEmpty()) {
        return false;
    }
    if (vpx_codec_decode(&m_d->ctx, reinterpret_cast<const uint8_t *>(frame.constData()),
                         static_cast<unsigned int>(frame.size()), nullptr, 0)) {
        return false;
    }
    vpx_codec_iter_t it = nullptr;
    const vpx_image_t *img = nullptr;
    while ((img = vpx_codec_get_frame(&m_d->ctx, &it)) != nullptr) {
        m_d->last = img;
    }
    return m_d->last != nullptr;
}

QImage Vp8Decoder::image() const
{
    return m_d->last ? i420ToImage(m_d->last) : QImage();
}

QImage Vp8Decoder::decode(const QByteArray &frame)
{
    return feed(frame) ? image() : QImage();
}
