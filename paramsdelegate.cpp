#include "paramsdelegate.h"
#include "paramsmodel.h"
#include "menu/menu.h"
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QApplication>
#include <QFontMetrics>
#include <QFont>
#include <QStyle>
#include <QSize>


ParamsDelegate::ParamsDelegate(QObject* parent)
    :QItemDelegate(parent)
{
}

ParamsDelegate::~ParamsDelegate()
{
}

QWidget* ParamsDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const ParamsModel* paramsModel = qobject_cast<const ParamsModel*>(index.model());

    if(!paramsModel) return QItemDelegate::createEditor(parent, option, index);

    const Parameter* p = paramsModel->parameter(index);

    if(!p) return QItemDelegate::createEditor(parent, option, index);

    menu_value_t* menu_value = const_cast<menu_value_t*>(paramsModel->menuValue(index));

    if(menu_value && menu_value_type(menu_value) == MENU_VALUE_TYPE_ENUM){
        QComboBox* cb = new QComboBox(parent);
        cb->addItems(getMenuValueStringList(menu_value, p->minToUInt(), p->maxToUInt()));
        return cb;
    }else{
        switch(p->type()){
        case PARAM_TYPE_INT:
        case PARAM_TYPE_UINT:{
            QSpinBox* sb = new QSpinBox(parent);
            sb->setRange(p->minToInt(), p->maxToInt());
            return sb;
            }
            break;
        case PARAM_TYPE_FRACT_10:
        case PARAM_TYPE_FRACT_100:
        case PARAM_TYPE_FRACT_1000:
        case PARAM_TYPE_FRACT_10000:{
            QDoubleSpinBox* dsb = new QDoubleSpinBox(parent);
            dsb->setSingleStep(0.1);
            dsb->setRange(p->minToFloat(), p->maxToFloat());
            return dsb;
            }
            break;
        }
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void ParamsDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    const ParamsModel* paramsModel = qobject_cast<const ParamsModel*>(index.model());

    if(!paramsModel){
        QItemDelegate::setEditorData(editor, index);
        return;
    }

    const Parameter* p = paramsModel->parameter(index);

    if(!p){
        QItemDelegate::setEditorData(editor, index);
        return;
    }

    menu_value_t* menu_value = const_cast<menu_value_t*>(paramsModel->menuValue(index));

    if(menu_value && menu_value_type(menu_value) == MENU_VALUE_TYPE_ENUM){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        if(!cb){
            QItemDelegate::setEditorData(editor, index);
            return;
        }
        cb->setCurrentIndex(paramsModel->data(index, Qt::EditRole).toUInt() - p->minToUInt());
    }else{
        switch(p->type()){
        case PARAM_TYPE_INT:
        case PARAM_TYPE_UINT:{
            QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
            if(!sb){
                QItemDelegate::setEditorData(editor, index);
                return;
            }
            sb->setValue(paramsModel->data(index, Qt::EditRole).toInt());
            }
            break;
        case PARAM_TYPE_FRACT_10:
        case PARAM_TYPE_FRACT_100:
        case PARAM_TYPE_FRACT_1000:
        case PARAM_TYPE_FRACT_10000:{
            QDoubleSpinBox* dsb = qobject_cast<QDoubleSpinBox*>(editor);
            if(!dsb){
                QItemDelegate::setEditorData(editor, index);
                return;
            }
            dsb->setValue(paramsModel->data(index, Qt::EditRole).toDouble());
            }
            break;
        }
    }
}

void ParamsDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ParamsModel* paramsModel = qobject_cast<ParamsModel*>(model);

    if(!paramsModel){
        QItemDelegate::setModelData(editor, model, index);
        return;
    }

    const Parameter* p = paramsModel->parameter(index);

    if(!p){
        QItemDelegate::setModelData(editor, model, index);
        return;
    }

    menu_value_t* menu_value = const_cast<menu_value_t*>(paramsModel->menuValue(index));

    if(menu_value && menu_value_type(menu_value) == MENU_VALUE_TYPE_ENUM){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        if(!cb){
            QItemDelegate::setModelData(editor, model, index);
            return;
        }
        paramsModel->setData(index, cb->currentIndex() + p->minToUInt());
    }else{
        switch(p->type()){
        case PARAM_TYPE_INT:
        case PARAM_TYPE_UINT:{
            QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
            if(!sb){
                QItemDelegate::setModelData(editor, model, index);
                return;
            }
            paramsModel->setData(index, sb->value());
            }
            break;
        case PARAM_TYPE_FRACT_10:
        case PARAM_TYPE_FRACT_100:
        case PARAM_TYPE_FRACT_1000:
        case PARAM_TYPE_FRACT_10000:{
            QDoubleSpinBox* dsb = qobject_cast<QDoubleSpinBox*>(editor);
            if(!dsb){
                QItemDelegate::setModelData(editor, model, index);
                return;
            }
            paramsModel->setData(index, dsb->value());
            }
            break;
        }
    }
}

QSize ParamsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QSize text_size = option.fontMetrics.size(0, text);

    QSize res_size = qApp->style()->sizeFromContents(QStyle::CT_ItemViewItem, &option, text_size);

    return res_size;
}

void ParamsDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

QStringList ParamsDelegate::getMenuValueStringList(menu_value_t* menu_value, size_t min_index, size_t max_index) const
{
    if(menu_value_type(menu_value) != MENU_VALUE_TYPE_ENUM) return QStringList();

    menu_value_t* enum_values = menu_value_enum_values(menu_value);

    if(!enum_values) return QStringList();

    QStringList res_list;

    if(max_index > menu_value_enum_count(menu_value)){
        max_index = menu_value_enum_count(menu_value);
    }

    for(size_t i = min_index; i < menu_value_enum_count(menu_value); i ++){
        if(menu_value_type(&enum_values[i]) == MENU_VALUE_TYPE_STRING){
            res_list << tr(menu_value_string(&enum_values[i]));
        }
    }

    return res_list;
}
