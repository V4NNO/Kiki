#pragma once

#include <QByteArray>
#include <QImage>

// The decoding half of KikiHost's vp8codec.h, on the viewer side: History
// video arrives as runs of VP8 frames (a keyframe followed by deltas, see
// MessageType::HistorySegment) and is decoded here instead of on the host.
// That is how the original Kickidler viewer works -- viewer.exe carries
// libvpx's VP8 decoder and pulls `node::protocol::history::video` segments --
// and it is what makes scrubbing instant: a delta frame is ~9 KB on the wire
// against ~150 KB for a JPEG of the same screen, so whole stretches can be
// held locally and any position inside them drawn without asking the host.
//
// Decoding and colour conversion are separate on purpose. Reaching a frame
// means feeding every delta since its keyframe, but only the last one is ever
// shown -- converting each intermediate frame to RGB (a full-screen pass) was
// what made a drag stutter. feed() only decodes; image() converts the latest.
class Vp8Decoder
{
public:
    Vp8Decoder();
    ~Vp8Decoder();
    Vp8Decoder(const Vp8Decoder &) = delete;
    Vp8Decoder &operator=(const Vp8Decoder &) = delete;

    bool begin(); // (re)initialize a fresh decode context
    bool isOpen() const { return m_open; }
    void end();

    // Decodes one VP8 packet into the decoder's state without producing an
    // image. A keyframe resets every reference buffer, so it can be fed at
    // any point to jump straight to it. Returns false on a decode error.
    bool feed(const QByteArray &frame);
    // The most recently decoded frame as an image (null if none yet). Valid
    // until the next feed().
    QImage image() const;

    // feed() followed by image() -- for a caller that wants every frame.
    QImage decode(const QByteArray &frame);

private:
    struct Impl;
    Impl *m_d = nullptr;
    bool m_open = false;
};
