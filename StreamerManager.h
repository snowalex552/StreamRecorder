#ifndef STREAMERMANAGER_H
#define STREAMERMANAGER_H

#include <QObject>
#include <QVector>

#include "Streamer.h"

class StreamerManager : public QObject
{
        Q_OBJECT

    public:
        static StreamerManager* getInstance();
        static void deleteInstance();

        void initialize();

        void addStreamer(const QString& name);
        void deleteStreamer(Streamer* streamer);

        void setShutdown(bool shutdown);

    signals:
        void streamerAdded(Streamer* streamer);
        void streamerDeleted(Streamer* streamer);

    public slots:

    private slots:
        void onRecordingChanged(bool status);

    protected:
        explicit StreamerManager(QObject* parent = nullptr);

    private:
        bool isRecordingAnyone();

        void addModel(const QString& string);
        QStringList readModels();
        void removeModel(const QString& string);

    private:
        static StreamerManager* m_instance;

        QVector<Streamer*> m_streamers;
        bool m_shutdown;
};

#endif // STREAMERMANAGER_H
