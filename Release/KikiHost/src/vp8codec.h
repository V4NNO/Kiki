#pragma once

#include <QByteArray>
#include <QImage>
#include <QList>

// Thin libvpx VP8 wrapper for the History video storage, matching the
// original Kickidler node's video_sequence / video_frame model: a sequence
// is a run of frames that opens with a keyframe, the rest coded as deltas
// against the previous one. Only KikiHost uses this -- it encodes captured
// frames into VP8 and, when a viewer asks for a history frame, decodes the
// relevant sequence back and hands the viewer a plain JPEG, so the wire
// protocol and the viewer stay unchanged.
class Vp8Encoder
{
public:
    Vp8Encoder();
    ~Vp8Encoder();
    Vp8Encoder(const Vp8Encoder &) = delete;
    Vp8Encoder &operator=(const Vp8Encoder &) = delete;

    // (Re)opens a sequence at width x height (rounded down to even). The next
    // encode() is always a keyframe. Returns false if libvpx init failed.
    bool begin(int width, int height);
    bool isOpen() const { return m_open; }
    int width() const { return m_width; }
    int height() const { return m_height; }

    // Encodes one frame. forceKey (or the sequence's first frame) makes it a
    // keyframe; set *isKey to what was produced. Returns the VP8 frame bytes,
    // empty on failure.
    QByteArray encode(const QImage &image, bool forceKey, bool *isKey);

    void end();

private:
    struct Impl;
    Impl *m_d = nullptr;
    bool m_open = false;
    int m_width = 0;
    int m_height = 0;
};

namespace Vp8 {
// Decodes a run of VP8 frames (the first must be a keyframe, the rest its
// following deltas in order) and returns the last one as an RGB QImage.
// Empty QImage on failure.
QImage decodeLast(const QList<QByteArray> &frames);
}

// A persistent VP8 decoder that keeps libvpx's reference-frame state between
// calls, so stepping forward through a sequence decodes one delta per frame
// instead of replaying from the keyframe each time (what made holding "next"
// stall). Reset when seeking to a different sequence or backward.
class Vp8Decoder
{
public:
    Vp8Decoder();
    ~Vp8Decoder();
    Vp8Decoder(const Vp8Decoder &) = delete;
    Vp8Decoder &operator=(const Vp8Decoder &) = delete;

    bool begin();          // (re)initialize a fresh decode context
    bool isOpen() const { return m_open; }
    void end();
    // Feed one VP8 frame packet; returns the image it produced (may be null
    // if the packet yielded no frame).
    QImage decode(const QByteArray &frame);

private:
    struct Impl;
    Impl *m_d = nullptr;
    bool m_open = false;
};
