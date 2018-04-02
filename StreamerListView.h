#ifndef STREAMERLISTVIEW_H
#define STREAMERLISTVIEW_H

#include <QListView>

#include "Streamer.h"
#include "StreamerListDelegate.h"
#include "StreamerListModel.h"

class StreamerListView : public QListView
{
        Q_OBJECT

    public:
        StreamerListView(bool sort = true, QWidget* parent = nullptr);

        StreamerListModel* artistsListModel() const;
        Streamer* getSelectedStreamer();

        int rowCount() const;
        int columnCount() const;
        void propendItem(Streamer* streamer);
        void insertItemAt(Streamer* streamer, int row);
        void removeFirstItem();
        void removeLastItem();
        void removeItemAt(int row);
        void clear();

        QSize maxSize();

    public slots:
        void appendItem(Streamer* streamer);
        void removeItem(Streamer* streamer);

    signals:
        void artistClicked(Streamer* streamer);
        void artistEntered(Streamer* streamer);
        void artistPressed(Streamer* streamer);
        void artistSelected(Streamer* streamer);
        void artistDoubleClicked(Streamer* streamer);

    private slots:
        void onStreamerAdded(Streamer* streamer);
        void onStreamerRemoved(Streamer* streamer);

        void onContextMenuRequested(QPoint position);
        void onItemClicked(const QModelIndex& index);
        void onItemEntered(const QModelIndex& index);
        void onItemPressed(const QModelIndex& index);
        void onItemDoubleClicked(const QModelIndex& index);
        void onCurrentRowChanged(QModelIndex current, QModelIndex previous);

    private:
        StreamerListModel* m_streamerListModel;
        StreamerListDelegate* m_streamerListDelegate;
};

#endif // STREAMERLISTVIEW_H
