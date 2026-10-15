#pragma once

#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QList>
#include <QString>

// A VP8 encoder (the bundled libvpx) feeding a minimal WebM (Matroska)
// muxer: EBML header, Segment { SeekHead, Info, Tracks, Clusters, Cues }.
// That is what the original's PlayerVideoSaver produces -- History exported
// as a .webm file (VideoSaverSelector.qml: extension "webm").
//
// Frames are added with their presentation time; each one is encoded at a
// fixed size (the size given to open(); a differently sized image is scaled
// into it). The file is only valid once finish() succeeded; abort() closes
// and removes it.
class WebmWriter
{
public:
    // VideoSaverSelector.qml's "Quallity": PlayerVideoSaver.Q_poor / Q_good /
    // Q_best (Low / Medium / High).
    enum class Quality { Poor, Good, Best };

    WebmWriter();
    ~WebmWriter();
    WebmWriter(const WebmWriter &) = delete;
    WebmWriter &operator=(const WebmWriter &) = delete;

    // title becomes the video track's Name (start()'s trackName).
    bool open(const QString &path, int width, int height, Quality quality, const QString &title);
    // One picture shown from timestampMs (ms in the output, 0 = start) for
    // durationMs; timestamps must grow.
    bool addFrame(const QImage &image, qint64 timestampMs, qint64 durationMs);
    // Flushes the encoder, writes the Cues, the duration and the sizes, and
    // closes the file.
    bool finish();
    void abort();

    int width() const { return m_width; }
    int height() const { return m_height; }
    QString errorString() const { return m_error; }

private:
    struct Encoder;
    bool fail(const QString &message);
    bool drain();
    bool writeBlock(const QByteArray &frame, qint64 timestampMs, bool key);
    void startCluster(qint64 timestampMs);
    void closeCluster();
    void releaseEncoder();

    QFile m_file;
    Encoder *m_encoder = nullptr;
    int m_width = 0;
    int m_height = 0;
    QString m_error;
    // Positions relative to the start of the Segment's payload.
    qint64 m_segmentDataStart = 0;
    qint64 m_segmentSizePos = 0;
    qint64 m_durationPos = 0;
    qint64 m_cuesSeekPos = 0; // the Cues entry's SeekPosition value
    qint64 m_clusterStart = -1; // absolute file position of the open Cluster
    qint64 m_clusterSizePos = 0;
    qint64 m_clusterTimecode = 0;
    qint64 m_lastTimestamp = -1;
    qint64 m_endTimestamp = 0; // end of the last frame (the Duration)
    struct Cue {
        qint64 timeMs = 0;
        qint64 clusterPosition = 0; // relative to the Segment payload
    };
    QList<Cue> m_cues;
};
