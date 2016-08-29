#include "eventmodel.h"
#include <string.h>
#include <QString>


#define EVENT_COLUMNS_COUNT 2
static const QString header_names[EVENT_COLUMNS_COUNT] = {
    QObject::tr("Параметр"),
    QObject::tr("Значение")
};

#define EVENT_COL_PARAM 0
#define EVENT_COL_VALUE 1

#define EVENT_ROW_ID 0
#define EVENT_ROW_TYPE 1
#define EVENT_ROW_STATE 2
#define EVENT_ROW_DIRECTION 3
#define EVENT_ROW_INIT_STATE 4
#define EVENT_ROW_CALIBR_STATE 5
#define EVENT_ROW_STARTING_STATE 6
#define EVENT_ROW_STOPPING_STATE 7
#define EVENT_ROW_ERR_STOPPING_STATE 8
#define EVENT_ROW_REFERENCE 9
#define EVENT_ROW_FLAGS 10
#define EVENT_ROW_WARNINGS 11
#define EVENT_ROW_ERRORS 12
#define EVENT_ROW_POWER_WARNINGS 13
#define EVENT_ROW_POWER_ERRORS 14
#define EVENT_ROW_PHASE_ERRORS 15
#define EVENT_ROW_PHASE_A_TIME 16
#define EVENT_ROW_PHASE_B_TIME 17
#define EVENT_ROW_PHASE_C_TIME 18
#define EVENT_ROW_TIME 19

#define EVENT_ROWS_COUNT 20
static const QString rows_names[EVENT_ROWS_COUNT] = {
    QObject::tr("Идентификатор"),
    QObject::tr("Тип"),
    QObject::tr("Состояние"),
    QObject::tr("Направление"),
    QObject::tr("Сост. инициализ."),
    QObject::tr("Сост. калибровки"),
    QObject::tr("Сост. запуска"),
    QObject::tr("Сост. останова"),
    QObject::tr("Сост. ош. останова"),
    QObject::tr("Задание"),
    QObject::tr("Флаги"),
    QObject::tr("Предупреждения"),
    QObject::tr("Ошибки"),
    QObject::tr("Предупреждения пит."),
    QObject::tr("Ошибки пит."),
    QObject::tr("Ошибки фаз"),
    QObject::tr("Время фазы A"),
    QObject::tr("Время фазы B"),
    QObject::tr("Время фазы C"),
    QObject::tr("Время"),
};


EventModel::EventModel(QObject *parent)
    :QAbstractItemModel(parent)
{
    cur_event = nullptr;
}

EventModel::~EventModel()
{
    delete cur_event;
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole) return QVariant();
    if(!index.isValid()) return QVariant();
    if(index.column() >= EVENT_COLUMNS_COUNT) return QVariant();
    if(index.row() >= EVENT_ROWS_COUNT) return QVariant();

    if(index.column() == EVENT_COL_PARAM) return rows_names[index.row()];

    if(cur_event == nullptr || !cur_event->isValid()) return QVariant();

    switch(index.row()){
    case EVENT_ROW_ID:
        return QString("%1").arg(cur_event->id(), 0, 10);
    case EVENT_ROW_TYPE:
        return QString("%1").arg(cur_event->type(), 0, 10);
    case EVENT_ROW_STATE:
        return QString("%1").arg(cur_event->state(), 0, 10);
    case EVENT_ROW_DIRECTION:
        return QString("%1").arg(cur_event->direction(), 0, 10);
    case EVENT_ROW_INIT_STATE:
        return QString("%1").arg(cur_event->initState(), 0, 10);
    case EVENT_ROW_CALIBR_STATE:
        return QString("%1").arg(cur_event->calibrationState(), 0, 10);
    case EVENT_ROW_STARTING_STATE:
        return QString("%1").arg(cur_event->startingState(), 0, 10);
    case EVENT_ROW_STOPPING_STATE:
        return QString("%1").arg(cur_event->stoppingState(), 0, 10);
    case EVENT_ROW_ERR_STOPPING_STATE:
        return QString("%1").arg(cur_event->errStoppingState(), 0, 10);
    case EVENT_ROW_REFERENCE:
        return QString("%1").arg(cur_event->reference(), 0, 10);
    case EVENT_ROW_FLAGS:
        return QString("0x%1").arg(cur_event->flags(), 0, 16);
    case EVENT_ROW_WARNINGS:
        return QString("0x%1").arg(cur_event->warnings(), 0, 16);
    case EVENT_ROW_ERRORS:
        return QString("0x%1").arg(cur_event->errors(), 0, 16);
    case EVENT_ROW_POWER_WARNINGS:
        return QString("0x%1").arg(cur_event->powerWarnings(), 0, 16);
    case EVENT_ROW_POWER_ERRORS:
        return QString("0x%1").arg(cur_event->powerErrors(), 0, 16);
    case EVENT_ROW_PHASE_ERRORS:
        return QString("0x%1").arg(cur_event->phaseErrors(), 0, 16);
    case EVENT_ROW_PHASE_A_TIME:
        return QString("%1").arg(cur_event->phaseTimeA(), 0, 10);
    case EVENT_ROW_PHASE_B_TIME:
        return QString("%1").arg(cur_event->phaseTimeB(), 0, 10);
    case EVENT_ROW_PHASE_C_TIME:
        return QString("%1").arg(cur_event->phaseTimeC(), 0, 10);
    case EVENT_ROW_TIME:
        return QString("0x%1").arg(cur_event->time(), 0, 10);
    }

    return QVariant();
}

Qt::ItemFlags EventModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) return 0;

    return QAbstractItemModel::flags(index);
}

QVariant EventModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal) return QVariant();
    if(role != Qt::DisplayRole) return QVariant();
    if(section >= EVENT_COLUMNS_COUNT) return QVariant();

    return header_names[section];
}

QModelIndex EventModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)) return QModelIndex();

    return createIndex(row, column);
}

QModelIndex EventModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()) return 0;
    return EVENT_ROWS_COUNT;
}

int EventModel::columnCount(const QModelIndex &/*parent*/) const
{
    return EVENT_COLUMNS_COUNT;
}

void EventModel::setEvent(const DriveEvent *event)
{
    emit beginResetModel();

    delete cur_event;
    cur_event = new DriveEvent(*event);

    emit endResetModel();
}
