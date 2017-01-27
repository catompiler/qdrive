#ifndef PARAMSDELEGATE_H
#define PARAMSDELEGATE_H

#include <QItemDelegate>
#include <QStringList>

class ParamsModel;
typedef struct _MenuValue menu_value_t;

class ParamsDelegate
        :public QItemDelegate
{
Q_OBJECT
public:
    ParamsDelegate(QObject* parent = 0);
    ~ParamsDelegate();

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;

private:
    QStringList getMenuValueStringList(menu_value_t* menu_value, size_t min_index, size_t max_index) const;
};

#endif // PARAMSDELEGATE_H
