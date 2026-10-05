#pragma once

#include "frameprotocol.h"

#include <QByteArray>
#include <QHash>
#include <QImage>
#include <QList>
#include <QPair>

#include <memory>
#include <unordered_map>

class Vp8Decoder;

// What the original viewer calls its VideoStore (impl-viewer-history/player/
// videoStore.cpp): the stretches of History video downloaded so far, held as
// the VP8 packets they arrived as, decoded on demand.
//
// This is what makes scrubbing behave like a video player. Asking the host
// for one JPEG per position costs ~150 KB and a round trip each, so the
// picture can only ever catch up where the user stops. A downloaded run
// costs ~9 KB per frame and decodes locally in well under a millisecond, so
// every position inside it is immediate -- and the slider's "loaded" layer
// grows in visible chunks instead of one invisible marker per seek.
class HistorySegmentStore
{
public:
    HistorySegmentStore();
    ~HistorySegmentStore();
    HistorySegmentStore(const HistorySegmentStore &) = delete;
    HistorySegmentStore &operator=(const HistorySegmentStore &) = delete;

    void clear();

    // Takes one downloaded run. Returns false when it carried nothing or
    // duplicated what is already held.
    bool addSegment(quint32 streamId, const ViewerProtocol::HistorySegmentPayload &payload);

    // Is this moment inside a downloaded run?
    bool covers(quint32 streamId, qint64 timestampMs) const;
    // The downloaded run's frame at or before timestampMs, decoded. Null when
    // the moment has not been downloaded.
    QImage frameAt(quint32 streamId, qint64 timestampMs);
    // The exact moment frameAt() would serve (-1 when not downloaded), so the
    // caller can tell which stored frame is on screen.
    qint64 frameTimeAt(quint32 streamId, qint64 timestampMs) const;

    // Downloaded [first, last] spans for one screen, in order -- what the
    // slider paints as its loaded layer.
    QList<QPair<qint64, qint64>> spans(quint32 streamId) const;

    // The first stretch of [fromMs, toMs) that is NOT downloaded yet for this
    // screen, searching outward from `aroundMs` so the area the user is
    // looking at fills first. Returns false when everything is held.
    bool nextGap(quint32 streamId, qint64 fromMs, qint64 toMs, qint64 aroundMs,
                 qint64 *gapStartMs) const;

    qint64 bytes() const { return m_bytes; }
    void setBudgetBytes(qint64 budget) { m_budgetBytes = budget; }
    // Drops the runs furthest from `aroundMs` until the budget is met.
    void trim(qint64 aroundMs);

private:
    // One downloaded run: frames in time order, the first one a keyframe, so
    // it decodes without anything before it.
    struct Chunk {
        qint64 sequenceId = 0;
        qint64 firstMs = 0;
        qint64 lastMs = 0;
        qint64 bytes = 0;
        QList<ViewerProtocol::SegmentFrame> frames;
    };

    // Where the warm decoder for one screen currently stands, so stepping
    // forward feeds one delta instead of replaying the whole chunk.
    struct Warm {
        int chunkIndex = -1;
        int frameIndex = -1;
        QImage image;
        std::unique_ptr<Vp8Decoder> decoder;
    };

    struct Stream {
        QList<Chunk> chunks; // sorted by firstMs, non-overlapping
        Warm warm;
    };

    // Index of the chunk containing timestampMs, or -1.
    static int chunkAt(const QList<Chunk> &chunks, qint64 timestampMs);
    // Index of the last frame at or before timestampMs within a chunk, or -1.
    static int frameAtOrBefore(const Chunk &chunk, qint64 timestampMs);
    // Index of the last keyframe at or before frameIndex (0 at worst: every
    // chunk opens with one). The recorder forces one every 10 frames, so this
    // bounds what any seek has to replay.
    static int keyAtOrBefore(const Chunk &chunk, int frameIndex);

    // std::unordered_map, not QHash: a Stream owns its warm decoder through
    // a unique_ptr and so cannot be copied, which QHash requires.
    std::unordered_map<quint32, Stream> m_streams;
    qint64 m_bytes = 0;
    qint64 m_budgetBytes = 192 * 1024 * 1024;
};
