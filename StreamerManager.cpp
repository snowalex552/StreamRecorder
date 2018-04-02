#include "StreamerManager.h"

#include <QDebug>
#include <QSettings>

StreamerManager* StreamerManager::m_instance = NULL;

StreamerManager::StreamerManager(QObject *parent) : QObject(parent)
{
    m_shutdown = false;
}

StreamerManager* StreamerManager::getInstance()
{
    if(!m_instance)
    {
        m_instance = new StreamerManager();
    }

    return m_instance;
}

void StreamerManager::deleteInstance()
{
    if(m_instance)
    {
        delete m_instance;
    }
}

void StreamerManager::initialize()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    quint32 size = settings.beginReadArray("Models");
    for(quint32 i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        addStreamer(settings.value("Model").toString());
    }
    settings.endArray();
}

void StreamerManager::addStreamer(const QString& name)
{
    foreach(Streamer* i_streamer, m_streamers)
    {
        if(i_streamer->getName() == name)
        {
            return;
        }
    }

    qDebug() << "Adding model" << name;
    addModel(name);

    Streamer* streamer = new Streamer(name);
    QObject::connect(streamer, SIGNAL(recordingChanged(bool)), SLOT(onRecordingChanged(bool)));
    streamer->checkAvailability();
    m_streamers.push_back(streamer);

    emit streamerAdded(streamer);
}

void StreamerManager::deleteStreamer(Streamer* streamer)
{
    qDebug() << "Removing model" << streamer->getName();
    removeModel(streamer->getName());

    m_streamers.removeOne(streamer);
    streamer->deleteLater();
}

void StreamerManager::setShutdown(bool shutdown)
{
    qDebug() << (shutdown ? "Shutting down PC" : "Not shutting down PC");
    m_shutdown = shutdown;
}

void StreamerManager::onRecordingChanged(bool status)
{
    qDebug() << "status" << status << "isRecordingAnyone" << isRecordingAnyone() << "m_shutdown" << m_shutdown;
    if(!status && !isRecordingAnyone() && m_shutdown)
    {

        //SE IL PC SI DISCONNETTE E RISULTA CHE SI STA REGISTRANDO UNA STREAMER,
        //FERMARE IL PROCESSO E FAR PARTIRE LO SHUTDOWN
        //qDebug() << "Shutdown";
        QProcess::startDetached("shutdown -s -f -t 60");
    }
}

bool StreamerManager::isRecordingAnyone()
{
    foreach(Streamer* i_streamer, m_streamers)
    {
        if(i_streamer->isRecording())
        {
            return true;
        }
    }

    return false;
}

void StreamerManager::addModel(const QString& string)
{
    QStringList models = readModels();
    if(!models.contains(string))
    {
        models << string;

        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.beginWriteArray("Models");
        for(qint32 i = 0; i < models.size(); ++i)
        {
            settings.setArrayIndex(i);
            settings.setValue("Model", models.at(i));
        }
        settings.endArray();
    }
}

QStringList StreamerManager::readModels()
{
    QStringList models;

    QSettings settings("settings.ini", QSettings::IniFormat);
    quint32 size = settings.beginReadArray("Models");
    for(quint32 i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        models << settings.value("Model").toString();
    }
    settings.endArray();

    return models;
}

void StreamerManager::removeModel(const QString& string)
{
    QStringList models = readModels();
    models.removeOne(string);

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginWriteArray("Models");
    for(qint32 i = 0; i < models.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Model", models.at(i));
    }
    settings.endArray();
}
