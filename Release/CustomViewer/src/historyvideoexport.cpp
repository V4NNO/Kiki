#include "historyvideoexport.h"

#include "historysegmentstore.h"

#include <QPainter>
#include <QRect>
#include <QThread>

#include <cmath>
#include <functional>

namespace {
// VideoComposer::processing (FUN_1402a90a0) and the layout pass before it
// (FUN_1402a8430). Runs on the worker thread; true when the file is done.
bool composeVideo(const HistoryVideoJob &job, const std::atomic<bool> &cancel,
                  const std::function<void(double)> &reportProgress, QString *error, bool *canceled)
{
    *canceled = false;
    if (job.markers.isEmpty()) {
        *error = QStringLiteral("No data on chosen period.");
        return false;
    }
    if (!std::isfinite(job.mps) || job.mps <= 0) {
        *error = QStringLiteral("bad mps: %1").arg(job.mps);
        return false;
    }
    HistorySegmentStore store;
    if (!store.open(job.cacheFile)) {
        *error = QStringLiteral("No data on chosen period.");
        return false;
    }

    // Layout: every screen gets a fixed rect the first time it shows up
    // with a picture, side by side at its native size, top-aligned. The
    // next one starts at the previous rect's right() -- x + w - 1, the
    // original's one-pixel overlap included.
    QHash<quint32, QRect> rects;
    int right = 0;
    for (const HistoryVideoJob::Marker &marker : job.markers) {
        if (cancel.load()) {
            *canceled = true;
            return false;
        }
        for (quint32 stream : job.streams) {
            const auto lookup = marker.lookupMs.constFind(stream);
            if (lookup == marker.lookupMs.cend() || rects.contains(stream)) {
                continue;
            }
            const QImage image = store.frameAt(stream, lookup.value());
            if (image.isNull()) {
                continue;
            }
            const QRect rect(right, 0, image.width(), image.height());
            rects.insert(stream, rect);
            right = qMax(right, rect.right());
        }
        if (rects.size() == job.streams.size()) {
            break;
        }
    }
    if (rects.isEmpty()) {
        *error = QStringLiteral("failed to init frame layout");
        return false;
    }
    int width = 0;
    int height = 0;
    for (const QRect &rect : std::as_const(rects)) {
        width = qMax(width, rect.right());
        height = qMax(height, rect.bottom());
    }
    width = qMax(16, width);
    height = qMax(16, height);
    width += width % 2;
    height += height % 2;

    WebmWriter writer;
    if (!writer.open(job.destination, width, height, job.quality, job.trackName)) {
        *error = writer.errorString();
        return false;
    }

    // Each marker is shown 1/mps seconds; a run of identical pictures is one
    // encoded frame lasting the whole run.
    const qint64 duration = qMax<qint64>(1, qRound64(1.0 / job.mps * 1000));
    const int count = int(job.markers.size());
    QImage previous;
    int runStart = 0;
    int runLength = 0;
    for (int i = 0; i < count; ++i) {
        if (cancel.load()) {
            writer.abort();
            *canceled = true;
            return false;
        }
        const HistoryVideoJob::Marker &marker = job.markers.at(i);
        // The canvas is RGB555 filled with 0, every picture copied in at its
        // rect without scaling; a screen with no picture stays black.
        QImage canvas(width, height, QImage::Format_RGB555);
        if (canvas.isNull()) {
            writer.abort();
            *error = QStringLiteral("failed to allocate image");
            return false;
        }
        canvas.fill(0);
        {
            QPainter painter(&canvas);
            for (quint32 stream : job.streams) {
                const auto lookup = marker.lookupMs.constFind(stream);
                const auto rect = rects.constFind(stream);
                if (lookup == marker.lookupMs.cend() || rect == rects.cend()) {
                    continue;
                }
                const QImage image = store.frameAt(stream, lookup.value());
                if (!image.isNull()) {
                    painter.drawImage(rect.value().topLeft(), image);
                }
            }
        }
        if (runLength > 0 && canvas == previous) {
            ++runLength;
        } else {
            if (runLength > 0 && !writer.addFrame(previous, runStart * duration, runLength * duration)) {
                *error = writer.errorString();
                return false;
            }
            previous = canvas;
            runStart = i;
            runLength = 1;
        }
        if (i % 4 == 0) {
            reportProgress(double(i) / count);
        }
        if (i % 32 == 0 && !marker.lookupMs.isEmpty()) {
            store.trim(marker.lookupMs.cbegin().value());
        }
    }
    if (runLength > 0 && !writer.addFrame(previous, runStart * duration, runLength * duration)) {
        *error = writer.errorString();
        return false;
    }
    if (!writer.finish()) {
        *error = writer.errorString();
        return false;
    }
    return true;
}
} // namespace

HistoryVideoSaver::HistoryVideoSaver(QObject *parent)
    : QObject(parent)
{
}

HistoryVideoSaver::~HistoryVideoSaver()
{
    if (m_thread) {
        m_cancelFlag->store(true);
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
}

void HistoryVideoSaver::setState(State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged();
    }
}

void HistoryVideoSaver::wait()
{
    m_progress = 0.0;
    m_error.clear();
    setState(State::Waiting);
}

void HistoryVideoSaver::fail(const QString &message)
{
    m_error = message;
    setState(State::Error);
}

void HistoryVideoSaver::startJob(const HistoryVideoJob &job)
{
    if (m_thread) {
        return;
    }
    m_progress = 0.0;
    m_error.clear();
    m_cancelFlag = std::make_shared<std::atomic<bool>>(false);
    const std::shared_ptr<std::atomic<bool>> cancel = m_cancelFlag;
    QPointer<HistoryVideoSaver> self(this);
    m_thread = QThread::create([job, cancel, self] {
        QString error;
        bool canceled = false;
        const auto report = [self](double progress) {
            if (HistoryVideoSaver *saver = self.data()) {
                QMetaObject::invokeMethod(saver, [self, progress] {
                    if (self) {
                        self->onWorkerProgress(progress);
                    }
                }, Qt::QueuedConnection);
            }
        };
        const bool ok = composeVideo(job, *cancel, report, &error, &canceled);
        if (HistoryVideoSaver *saver = self.data()) {
            QMetaObject::invokeMethod(saver, [self, ok, error, canceled] {
                if (self) {
                    self->onWorkerFinished(ok, canceled ? QString() : error);
                }
            }, Qt::QueuedConnection);
        }
    });
    setState(State::Working);
    m_thread->start();
}

void HistoryVideoSaver::cancel()
{
    if (m_state == State::Waiting) {
        setState(State::Canceled);
        return;
    }
    if (m_state == State::Working && m_cancelFlag) {
        m_cancelFlag->store(true);
        setState(State::Canceled);
    }
}

void HistoryVideoSaver::onWorkerProgress(double progress)
{
    if (m_state != State::Working) {
        return;
    }
    // onWorkerProgress: only a change of the rounded percent counts.
    if (qRound(progress * 100) != qRound(m_progress * 100)) {
        m_progress = progress;
        emit progressChanged(m_progress);
    }
}

void HistoryVideoSaver::onWorkerFinished(bool ok, const QString &error)
{
    if (m_thread) {
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
    if (m_state == State::Canceled) {
        return;
    }
    if (ok) {
        setState(State::Done);
        return;
    }
    // finish(false) while not canceled.
    m_error = error.isEmpty() ? QStringLiteral("failed to make video") : error;
    setState(State::Error);
}
