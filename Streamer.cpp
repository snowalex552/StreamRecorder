#include "Streamer.h"

#include <QTimer>

Streamer::Streamer(const QString& name, QObject *parent) : QObject(parent)
{
    m_name = name;
    m_url = "https://it.chaturbate.com/" + m_name + "/";

    m_available = false;
    m_recording = false;

    m_thread = new QThread();
    m_streamRecorder = new StreamRecorder();
    QObject::connect(m_streamRecorder, SIGNAL(recordingChanged(bool)), SLOT(onRecordingChanged(bool)));
    QObject::connect(m_streamRecorder, SIGNAL(recordingChanged(bool)), SIGNAL(recordingChanged(bool)));

    m_streamRecorder->moveToThread(m_thread);
    QObject::connect(m_thread, SIGNAL(started()), SLOT(onThreadStarted()));
    QObject::connect(m_thread, SIGNAL(finished()), SLOT(deleteLater()));

    QObject::connect(this, SIGNAL(startStream_sig()), m_streamRecorder, SLOT(startStream()));
}

bool Streamer::isAvailable() const
{
    return m_available;
}

void Streamer::checkAvailability()
{
    HttpRequest input(m_url, "GET");

    HttpRequestWorker* worker = new HttpRequestWorker();
    QObject::connect(worker, SIGNAL(requestFinished()), this, SLOT(onCheckFinished()));
    worker->execute(input);
}

void Streamer::startStream()
{
    m_streamRecorder->setName(m_name);
    m_streamRecorder->setM3u8(m_m3u8);

    if(!m_thread->isRunning())
    {
        m_thread->start();
    }
    else
    {
        emit startStream_sig();
    }
}

void Streamer::stopStream()
{
    m_streamRecorder->stopStream();
}

void Streamer::onCheckFinished()
{
    HttpRequestWorker* worker = qobject_cast<HttpRequestWorker*>(QObject::sender());

    QString response = worker->getResponse();

    if(response.contains("m3u8"))
    {
        m_available = true;

        //qDebug() << m_name << "is online, retrieving url";

        response = response.left(response.indexOf("m3u8") + 4);
        response = response.mid(response.lastIndexOf("http"));

        m_m3u8 = response;
        //qDebug() << "Url is" << response;
    }
    else
    {
        m_available = false;

        //qDebug() << m_name << "is offline";
    }

    worker->deleteLater();

    emit availabilityChanged(m_available);

    QTimer::singleShot(60000, [=] () {
        checkAvailability();
    });
}

void Streamer::onThreadStarted()
{
    emit startStream_sig();
}

void Streamer::onRecordingChanged(bool status)
{
    m_recording = status;
}

QString Streamer::getM3u8() const
{
    return m_m3u8;
}

QString Streamer::getName() const
{
    return m_name;
}

bool Streamer::isRecording() const
{
    return m_recording;
}
