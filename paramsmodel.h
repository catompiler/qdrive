#ifndef PARAMSMODEL_H
#define PARAMSMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QHash>
#include "parameter.h"

typedef struct _Menu menu_t;
typedef struct _MenuItem menu_item_t;
typedef struct _MenuValue menu_value_t;


class ParamsModel
        :public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParamsModel(QObject *parent = 0);
    ~ParamsModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Parameter* parameter(const QModelIndex& index);
    const Parameter* parameter(const QModelIndex& index) const;

    menu_value_t* menuValue(const QModelIndex& index);
    const menu_value_t* menuValue(const QModelIndex& index) const;

    QList<Parameter*> getParamsList();
    QHash<param_id_t, Parameter*> getParamsHash();


signals:

public slots:
    void applyDefault();
    void paramsUpdated();

private:
    menu_t* params_menu;
    menu_item_t* params_items;

    void setupParams();
    void cleanUpParams();
};

#endif // PARAMSMODEL_H
