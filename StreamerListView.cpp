#include "StreamerListView.h"

#include <QDebug>
#include <QMenu>

#include "StreamerManager.h"

StreamerListView::StreamerListView(bool sort, QWidget* parent) : QListView(parent)
{
    m_streamerListModel = new StreamerListModel(sort, parent);
    setModel(m_streamerListModel);
    QObject::connect(m_streamerListModel, SIGNAL(itemUpdated()), SLOT(repaint()));

    m_streamerListDelegate = new StreamerListDelegate(this);
    setItemDelegate(m_streamerListDelegate);

    //setGridSize(QSize(100, 30));
    //setMovement(QListView::Static);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    //setSelectionMode(QAbstractItemView::ExtendedSelection); uncomment to enable multi selection
    //setUniformItemSizes(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onContextMenuRequested(QPoint)));

    setStyleSheet(QString("QListView {"
                          "background: transparent;"
                          "border: 0px;"
                          "}"));

    //    verticalScrollBar()->setStyle(new QCommonStyle());
    //    verticalScrollBar()->setStyleSheet(GuiUtils::SCROLL_BAR_STYLE);

    QObject::connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    QObject::connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(onItemPressed(QModelIndex)));
    QObject::connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked(QModelIndex)));

    QObject::connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(onCurrentRowChanged(QModelIndex, QModelIndex)));

    QObject::connect(StreamerManager::getInstance(), SIGNAL(streamerAdded(Streamer*)), SLOT(onStreamerAdded(Streamer*)));
    QObject::connect(StreamerManager::getInstance(), SIGNAL(streamerDeleted(Streamer*)), SLOT(onStreamerRemoved(Streamer*)));
    StreamerManager::getInstance()->initialize();
}

StreamerListModel* StreamerListView::artistsListModel() const
{
    return m_streamerListModel;
}

Streamer* StreamerListView::getSelectedStreamer()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    if(!selection.isEmpty())
    {
        return m_streamerListModel->rootItem()->child(selection.takeLast().row())->streamer();
    }

    return NULL;
}

int StreamerListView::rowCount() const
{
    return m_streamerListModel->rowCount();
}

int StreamerListView::columnCount() const
{
    return m_streamerListModel->columnCount();
}

void StreamerListView::propendItem(Streamer* streamer)
{
    m_streamerListModel->propendItem(streamer);
}

void StreamerListView::appendItem(Streamer* streamer)
{
    m_streamerListModel->appendItem(streamer);
}

void StreamerListView::removeItem(Streamer* streamer)
{
    m_streamerListModel->removeItem(streamer);
}

void StreamerListView::onStreamerAdded(Streamer* streamer)
{
    appendItem(streamer);
}

void StreamerListView::onStreamerRemoved(Streamer* streamer)
{
    removeItem(streamer);
}

void StreamerListView::insertItemAt(Streamer* streamer, int row)
{
    m_streamerListModel->insertItemAt(streamer, row);
}

void StreamerListView::removeFirstItem()
{
    m_streamerListModel->removeFirstItem();
}

void StreamerListView::removeLastItem()
{
    m_streamerListModel->removeLastItem();
}

void StreamerListView::removeItemAt(int row)
{
    m_streamerListModel->removeItemAt(row);
}

void StreamerListView::clear()
{
    m_streamerListModel->clear();
}

QSize StreamerListView::maxSize()
{
    QSize size;

    for(quint16 i = 0; i < m_streamerListModel->rootItem()->rowCount(); ++i)
    {
        StreamerListItemForm* item = m_streamerListModel->rootItem()->child(i);

        if(item->size().width() > size.width())
        {
            size.setWidth(item->size().width());
        }

        if(item->size().height() > size.height())
        {
            size.setHeight(item->size().height());
        }
    }

    return size;
}

void StreamerListView::onContextMenuRequested(QPoint position)
{
    QMenu menu(this);

    QAction* removeStreamer = menu.addAction("Remove streamer");
    QAction* selectedAction = menu.exec(viewport()->mapToGlobal(position));

    if(selectedAction == removeStreamer)
    {
        QModelIndexList selection = selectionModel()->selectedRows();
        while(!selection.isEmpty())
        {
            StreamerManager::getInstance()->deleteStreamer(m_streamerListModel->rootItem()->child(selection.takeLast().row())->streamer());
        }
    }
}

void StreamerListView::onItemClicked(const QModelIndex& index)
{
    emit artistClicked(m_streamerListModel->rootItem()->child(index.row())->streamer());
}

void StreamerListView::onItemEntered(const QModelIndex& index)
{
    emit artistEntered(m_streamerListModel->rootItem()->child(index.row())->streamer());
}

void StreamerListView::onItemPressed(const QModelIndex& index)
{
    emit artistPressed(m_streamerListModel->rootItem()->child(index.row())->streamer());
}

void StreamerListView::onItemDoubleClicked(const QModelIndex& index)
{
    emit artistDoubleClicked(m_streamerListModel->rootItem()->child(index.row())->streamer());
}

void StreamerListView::onCurrentRowChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);

    if(m_streamerListModel->rootItem()->child(current.row()))
    {
        emit artistSelected(m_streamerListModel->rootItem()->child(current.row())->streamer());
    }
}
