#include "eventsmodel.h"
#include <string.h>
#include <QString>


#define EVENTS_COLUMNS_COUNT 1
static const QString header_names[EVENTS_COLUMNS_COUNT] = {
    QObject::tr("Событие")
};

#define EVENTS_COL_EVENT 0


EventsModel::EventsModel(QObject *parent)
    :QAbstractItemModel(parent)
{
    events_list = new QList<DriveEvent>();
}

EventsModel::~EventsModel()
{
    delete events_list;
}

QVariant EventsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole) return QVariant();
    if(!index.isValid()) return QVariant();
    if(index.column() >= EVENTS_COLUMNS_COUNT) return QVariant();
    if(index.row() >= events_list->size()) return QVariant();

    return QString("Событие %1").arg(events_list->at(index.row()).id());
}

Qt::ItemFlags EventsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) return 0;

    return QAbstractItemModel::flags(index);
}

QVariant EventsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal) return QVariant();
    if(role != Qt::DisplayRole) return QVariant();
    if(section >= EVENTS_COLUMNS_COUNT) return QVariant();

    return header_names[section];
}

QModelIndex EventsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)) return QModelIndex();

    return createIndex(row, column);
}

QModelIndex EventsModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int EventsModel::rowCount(const QModelIndex &/*index*/) const
{
    return events_list->size();
}

int EventsModel::columnCount(const QModelIndex &/*index*/) const
{
    return EVENTS_COLUMNS_COUNT;
}

const DriveEvent *EventsModel::eventByIndex(const QModelIndex &index) const
{
    if(index.row() >= events_list->size()){
        return nullptr;
    }

    return &(events_list->at(index.row()));
}

void EventsModel::setEvents(QList<DriveEvent> events)
{
    emit beginResetModel();

    *events_list = events;

    emit endResetModel();
}
