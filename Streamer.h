#ifndef STREAMER_H
#define STREAMER_H

#include <QObject>
#include <QProcess>
#include <QThread>

#include "HttpRequestManager.h"
#include "StreamRecorder.h"

class Streamer : public QObject
{
    Q_OBJECT

    public:
        explicit Streamer(const QString& getName, QObject *parent = nullptr);

        bool isAvailable() const;
        void checkAvailability();

        void startStream();
        void stopStream();

        QString getName() const;
        QString getM3u8() const;

        bool isRecording() const;

    signals:
        void startStream_sig();

        void availabilityChanged(bool available);
        void recordingChanged(bool status);

    public slots:

    private slots:
        void onCheckFinished();

        void onThreadStarted();
        void onRecordingChanged(bool status);

    private:
        QString m_name;

        QString m_m3u8;
        QString m_url;

        bool m_available;
        bool m_recording;

        StreamRecorder* m_streamRecorder;
        QThread* m_thread;
};

#endif // STREAMER_H
