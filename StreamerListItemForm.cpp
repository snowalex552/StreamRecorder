#include "StreamerListItemForm.h"
#include "ui_StreamerListItemForm.h"

StreamerListItemForm::StreamerListItemForm(bool sort, QWidget* parent) :
    QWidget(parent)
{
    m_streamer = NULL;

    m_sort = sort;
    m_parent = NULL;
}

StreamerListItemForm::StreamerListItemForm(Streamer* streamer, StreamerListItemForm* parent) : QWidget(parent)
{
    ui = new Ui::StreamerListItemForm;
    ui->setupUi(this);

    m_streamer = streamer;
    QObject::connect(m_streamer, SIGNAL(availabilityChanged(bool)), SLOT(onAvailabilityChanged(bool)));
    QObject::connect(m_streamer, SIGNAL(recordingChanged(bool)), SLOT(onRecordingChanged(bool)));

    QString resource = m_streamer->isAvailable() ? ":/resources/glossy_button_blank_green_circle.svg" : ":/resources/glossy_button_blank_red_circle.svg";
    QPixmap pix = QPixmap::fromImage(QImage(resource)).scaled(10,
                                                              10,
                                                              Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation);
    if(!pix.isNull())
    {
        ui->streamerStatus->setPixmap(pix);
    }
    else
    {
        ui->streamerStatus->setText(m_streamer->isAvailable() ? "Y" : "N");
    }

    ui->streamerName->setText(m_streamer->getName());

    m_parent = parent;

    hide(); //hide widget because it's rendered by StreamerListDelegate
}

StreamerListItemForm::~StreamerListItemForm()
{
    //delete ui;
}

QVariant StreamerListItemForm::data(int column) const
{
    Q_UNUSED(column)

    return QVariant::fromValue(static_cast<void*>(const_cast<StreamerListItemForm*>(this)));
}

StreamerListItemForm* StreamerListItemForm::parent() const
{
    return m_parent;
}

int StreamerListItemForm::row() const
{
    if(m_parent)
    {
        return m_parent->m_childs.indexOf(const_cast<StreamerListItemForm*>(this));
    }
    else
    {
        return 0;
    }
}

StreamerListItemForm* StreamerListItemForm::child(int row) const
{
    return m_childs.value(row);
}

int StreamerListItemForm::columnCount() const
{
    return 1;
}

int StreamerListItemForm::rowCount() const
{
    return m_childs.count();
}

int StreamerListItemForm::indexOf(Streamer* streamer)
{
    int index = -1;

    for(quint16 i = 0; i < m_childs.size(); ++i)
    {
        if(m_childs.at(i)->streamer() == streamer)
        {
            index = i;
            break;
        }
    }

    return index;
}

void StreamerListItemForm::prependChild(StreamerListItemForm* item)
{
    m_childs.prepend(item);
    sort();
}

void StreamerListItemForm::appendChild(StreamerListItemForm* item)
{
    m_childs.append(item);
    sort();
}

void StreamerListItemForm::insertChildAt(StreamerListItemForm* item, int row)
{
    m_childs.insert(row, item);
    sort();
}

void StreamerListItemForm::removeFirstChild()
{
    if(!m_childs.isEmpty())
    {
        delete m_childs.takeFirst();
    }
}

void StreamerListItemForm::removeLastChild()
{
    if(!m_childs.isEmpty())
    {
        delete m_childs.takeLast();
    }
}

void StreamerListItemForm::removeChildAt(int row)
{
    if(row < m_childs.size())
    {
        delete m_childs.takeAt(row);
    }
}

void StreamerListItemForm::clear()
{
    qDeleteAll(m_childs);
    m_childs.clear();
}

Streamer* StreamerListItemForm::streamer() const
{
    return m_streamer;
}

void StreamerListItemForm::onAvailabilityChanged(bool available)
{
    QString resource = available ? ":/resources/glossy_button_blank_green_circle.svg" : ":/resources/glossy_button_blank_red_circle.svg";
    QPixmap pix = QPixmap::fromImage(QImage(resource)).scaled(10,
                                                              10,
                                                              Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation);
    if(!pix.isNull())
    {
        ui->streamerStatus->setPixmap(pix);
    }
    else
    {
        ui->streamerStatus->setText(m_streamer->isAvailable() ? "Y" : "N");
    }
}

void StreamerListItemForm::onRecordingChanged(bool status)
{
    ui->streamerStatus->setText("------>"+status);
}

void StreamerListItemForm::sort()
{
    if(m_sort)
    {
        std::sort(m_childs.begin(), m_childs.end(), [] (const StreamerListItemForm* streamerListItemForm1, const StreamerListItemForm* streamerListItemForm2) -> bool
        {
            return streamerListItemForm1->streamer()->getName() < streamerListItemForm2->streamer()->getName();
        });
    }
}
