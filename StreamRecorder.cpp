#include "StreamRecorder.h"

#include <QDateTime>
#include <QDebug>

StreamRecorder::StreamRecorder(QObject *parent) : QObject(parent)
{
    m_process = new QProcess(this);
    QObject::connect(m_process, SIGNAL(started()), SLOT(onStarted()));
    QObject::connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(onFinished(int, QProcess::ExitStatus)));

    QObject::connect(m_process, SIGNAL(readyRead()), SLOT(readStdout()));
}

void StreamRecorder::startStream()
{
    qDebug() << "Starting streaming of" << m_name;

    QString filename = m_name + "_" + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss") + ".mp4";

    m_process->start("ffmpeg/bin/ffmpeg.exe -i " + m_m3u8 + " -c copy " + "recordings/" + filename);
    m_process->waitForStarted(-1);

    qDebug() << "Recording started, saving streaming into file" << filename;
}

void StreamRecorder::stopStream()
{
    qDebug() << "Stopping streaming of" << m_name;
    m_process->write("q");
    m_process->write("\r\n");
}

void StreamRecorder::onStarted()
{
    qDebug() << "QProcess::started, streamer is" << m_name;
    emit recordingChanged(true);
}

void StreamRecorder::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "QProcess::finished, streamer is" << m_name << ", exitCode is" << exitCode << ", exitStatus is" << exitStatus;
    emit recordingChanged(false);
}

void StreamRecorder::readStdout()
{
    qDebug() << "StreamRecorder::readStdout";
    qDebug() << m_process->readAllStandardOutput();
}

void StreamRecorder::setName(const QString& name)
{
    m_name = name;
}

void StreamRecorder::setM3u8(const QString& m3u8)
{
    m_m3u8 = m3u8;
}
