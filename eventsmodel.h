#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QAbstractItemModel>
#include "driveevent.h"


class EventsModel
        :public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EventsModel(QObject *parent = 0);
    ~EventsModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;

    const DriveEvent* eventByIndex(const QModelIndex& index) const;

public slots:
    /**
     * @brief Устанавливает список событий модели.
     * @param events Список событий.
     */
    void setEvents(QList<DriveEvent> events);

private:
    QList<DriveEvent>* events_list;
};

#endif // EVENTSMODEL_H
