#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractItemModel>
#include "driveevent.h"


class EventModel
        :public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EventModel(QObject *parent = 0);
    ~EventModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

public slots:
    /**
     * @brief Устанавливает событие модели.
     * @param event Событие.
     */
    void setEvent(const DriveEvent* event);

private:
    const DriveEvent* cur_event;
};

#endif // EVENTMODEL_H
