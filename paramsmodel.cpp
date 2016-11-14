#include "paramsmodel.h"
#include <QStringList>
#include <QString>
#include <QVariant>
#include "parameter.h"
#include <QDebug>

#define MENU_DESCR_MACRO
#define MENU_DESCRS_ATTRIBS static const
#define MENU_ITEMS_ATTRIBS static
#include "menu/menu.h"

MENU_DESCRS(menu_descrs) {
    MENU_DESCR(0, 0, "Питание", 0, 0, 0),
        MENU_DESCR(1, 0, "Сеть", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_NOM, "Ном. U сети, В", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_NOM_ALLOW_VAR, "Доп. отклон. U сети, %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_NOM_CRIT_VAR, "Крит. отклон. U сети, %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_IN_CUTOFF, "Токовая отсечка по фазам, А", 0, 0, 0),
        MENU_DESCR(1, 0, "Шумы нуля", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_ZERO_NOISE, "Шум нулевого U, В", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ZERO_NOISE, "Шум нулевого I, А", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ROT_ZERO_NOISE, "Шум нулевого I якоря, А", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_EXC_ZERO_NOISE, "Шум нулевого I возбужд., А", 0, 0, 0),
        MENU_DESCR(1, 0, "Якорь", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_ROT_NOM, "Ном. U якоря, В", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_ROT_ALLOW_VAR, "Доп. отклон. U якоря, %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_U_ROT_CRIT_VAR, "Крит. отклон. U якоря, %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ROT_NOM, "Ном. I якоря, А", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ROT_CUTOFF_MULT, "Множ. отсечки I якоря", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ROT_ALLOW_VAR, "Доп. отклон. I якоря, %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_ROT_CRIT_VAR, "Крит. отклон. I якоря, %", 0, 0, 0),
        MENU_DESCR(1, 0, "Возбуждение", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_EXC, "I возбужд., А", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_EXC_ALLOW_VAR, "Доп. отклон. I возбужд., %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_I_EXC_CRIT_VAR, "Крит. отклон. I возбужд., %", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EXC_PHASE, "Фаза возбужд.", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EXC_MODE, "Режим возбужд.", 0, 0, 0),
    MENU_DESCR(0, 0, "Разгон", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_TIME, "Время разгона 0-100%, с", 0, 0, 0),
    MENU_DESCR(0, 0, "ПИД регулятор", 0, 0, 0),
        MENU_DESCR(1, 0, "ПИД якоря", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_ROT_PID_K_P, "Kp", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_ROT_PID_K_I, "Ki", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_ROT_PID_K_D, "Kd", 0, 0, 0),
        MENU_DESCR(1, 0, "ПИД возбужд.", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EXC_PID_K_P, "Kp", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EXC_PID_K_I, "Ki", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EXC_PID_K_D, "Kd", 0, 0, 0),
    MENU_DESCR(0, 0, "Запуск и останов", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_ROT_STOP_TIME, "Время останова якоря, с", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_EXC_STOP_TIME, "Время останова возбужд., с", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_EXC_START_TIME, "Время запуска возбужд., с", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_PHASES_CHECK_TIME, "Время ожидания фаз, мс", 0, 0, 0),
    MENU_DESCR(0, 0, "Тиристоры", 0, 0, 0),
        MENU_DESCR(1, 0, "Время открытия, мкс", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIACS_PAIRS_OPEN_TIME, "Силовые", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIAC_EXC_OPEN_TIME, "Возбужд.", 0, 0, 0),
    MENU_DESCR(0, 0, "Тепловая защита", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_THERMAL_OVERLOAD_PROT_TIME_6I, "Время работы при перегрузе 6x, с", 0, 0, 0),
    MENU_DESCR(0, 0, "Вычисления", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALC_PHASE_CURRENT, "Вычислять ток для фазы", 0, 0, 0),
    MENU_DESCR(0, 0, "Коммуникация", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_MODBUS_BAUD, "Скорость Modbus RTU", 0, 0, 0),
    MENU_DESCR(0, 0, "Цифровые входа", 0, 0, 0),
        MENU_DESCR(1, 0, "Вход 1", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_1_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_1_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Вход 2", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_2_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_2_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Вход 3", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_3_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_3_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Вход 4", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_4_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_4_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Вход 5", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_5_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_5_INVERSION, "Инверсия", 0, 0, 0),
    MENU_DESCR(0, 0, "Цифровые выхода", 0, 0, 0),
        MENU_DESCR(1, 0, "Выход 1", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_1_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_1_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Выход 2", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_2_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_2_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Выход 3", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_3_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_3_INVERSION, "Инверсия", 0, 0, 0),
        MENU_DESCR(1, 0, "Выход 4", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_4_TYPE, "Тип", 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_4_INVERSION, "Инверсия", 0, 0, 0),
    MENU_DESCR(0, 0, "Коэффициенты АЦП", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ua, "Коэффициент Ua", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ub, "Коэффициент Ub", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Uc, "Коэффициент Uc", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Urot, "Коэффициент Urot", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ia, "Коэффициент Ia", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ib, "Коэффициент Ib", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ic, "Коэффициент Ic", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Irot, "Коэффициент Irot", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Iexc, "Коэффициент Iexc", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Iref, "Коэффициент Iref", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ifan, "Коэффициент Ifan", 0, 0, 0),
    MENU_DESCR(0, 0, "Калибровочные данные", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ua, "Значение калибровки Ua", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ub, "Значение калибровки Ub", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Uc, "Значение калибровки Uc", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Urot, "Значение калибровки Urot", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ia, "Значение калибровки Ia", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ib, "Значение калибровки Ib", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ic, "Значение калибровки Ic", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Irot, "Значение калибровки Irot", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Iexc, "Значение калибровки Iexc", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Iref, "Значение калибровки Iref", 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALIBRATION_DATA_Ifan, "Значение калибровки Ifan", 0, 0, 0),
    //MENU_DESCR(0, 0, "Menu1_1", 0, 0, 0)
};


#define PARAMS_COLUMNS_COUNT 5
static const QString header_names[PARAMS_COLUMNS_COUNT] = {
    QObject::tr("Параметр"),
    QObject::tr("Значение"),
    QObject::tr("Минимум"),
    QObject::tr("Максимум"),
    QObject::tr("По-умолчанию")
};

#define PARAMS_COL_NAME 0
#define PARAMS_COL_VALUE 1
#define PARAMS_COL_MIN 2
#define PARAMS_COL_MAX 3
#define PARAMS_COL_DEF 4


ParamsModel::ParamsModel(QObject *parent)
    :QAbstractItemModel(parent)
{
    params_menu = new menu_t();
    params_items = new menu_item_t[MENU_DESCRS_COUNT(menu_descrs)];
    menu_make_from_descrs(params_menu, params_items, MENU_DESCRS_COUNT(menu_descrs),
                                       menu_descrs, MENU_DESCRS_COUNT(menu_descrs));
    setupParams();
}

ParamsModel::~ParamsModel()
{
    cleanUpParams();
    delete[] params_items;
    delete params_menu;
}

QVariant ParamsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();
    if(role != Qt::DisplayRole){
        if(role != Qt::EditRole || index.column()!= PARAMS_COL_VALUE){
            return QVariant();
        }
    }

    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    QVariant res;
    Parameter* param = static_cast<Parameter*>(menu_item_user_data(item));

    switch(index.column()){
    case PARAMS_COL_NAME:
        res = QVariant(tr(menu_item_text(item)));
        break;
    case PARAMS_COL_VALUE:
        if(param) res = QVariant(param->toString());
        break;
    case PARAMS_COL_MIN:
        if(param) res = QVariant(param->minToString());
        break;
    case PARAMS_COL_MAX:
        if(param) res = QVariant(param->maxToString());
        break;
    case PARAMS_COL_DEF:
        if(param) res = QVariant(param->defToString());
        break;
    }

    return res;
}

bool ParamsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole) return QAbstractItemModel::setData(index, value, role);

    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return false;

    Parameter* param = static_cast<Parameter*>(menu_item_user_data(item));

    if(!param) return false;

    bool is_ok = false;

    switch(param->type()){
    case PARAM_TYPE_INT:
        if(!param->validate(value.toInt(&is_ok)))
            return false;
        if(is_ok) param->setInt(value.toInt());
        break;
    case PARAM_TYPE_UINT:
        if(!param->validate(value.toUInt(&is_ok)))
            return false;
        if(is_ok) param->setUInt(value.toUInt());
        break;
    case PARAM_TYPE_FRACT_10:
    case PARAM_TYPE_FRACT_100:
    case PARAM_TYPE_FRACT_1000:
    case PARAM_TYPE_FRACT_10000:
        if(!param->validate(value.toFloat(&is_ok)))
            return false;
        if(is_ok) param->setFloat(value.toFloat());
        break;
    }

    emit dataChanged(index, index);

    return true;
}

Qt::ItemFlags ParamsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) return 0;

    if(index.column() == PARAMS_COL_VALUE){
        menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());
        if(item && menu_item_user_data(item) != nullptr){
            return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
        }
    }
    return QAbstractItemModel::flags(index);
}

QVariant ParamsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal) return QVariant();
    if(role != Qt::DisplayRole) return QVariant();
    if(section >= PARAMS_COLUMNS_COUNT) return QVariant();

    return header_names[section];
}

QModelIndex ParamsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)) return QModelIndex();

    menu_item_t* item = nullptr;

    if(parent.isValid()){
        item = menu_item_child_at(static_cast<menu_item_t*>(parent.internalPointer()), row);
    }else{
        item = menu_item_next_at(menu_root(params_menu), row);
    }

    if(!item) return QModelIndex();

    return createIndex(row, column, static_cast<void*>(item));
}

QModelIndex ParamsModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) return QModelIndex();

    menu_item_t* item = menu_item_parent(static_cast<menu_item_t*>(index.internalPointer()));

    if(!item) return QModelIndex();

    return createIndex(menu_item_pos(item), 0, static_cast<void*>(item));
}

int ParamsModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column() > 0) return 0;

    menu_item_t* item = nullptr;

    if(parent.isValid()){
        item = static_cast<menu_item_t*>(parent.internalPointer());
        return static_cast<int>(menu_item_childs_count(item));
    }
    item = menu_root(params_menu);
    return static_cast<int>(menu_item_count(item));
}

int ParamsModel::columnCount(const QModelIndex &/*parent*/) const
{
    return PARAMS_COLUMNS_COUNT;
}

QList<Parameter *> ParamsModel::getParamsList()
{
    QList<Parameter*> res_list;

    menu_item_t* item = nullptr;
    Parameter* param = nullptr;

    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param) res_list.append(param);
    }

    return res_list;
}

QHash<param_id_t, Parameter*> ParamsModel::getParamsHash()
{
    QHash<param_id_t, Parameter*> res_hash;

    menu_item_t* item = nullptr;
    Parameter* param = nullptr;

    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param) res_hash.insert(param->id(), param);
    }

    return res_hash;
}

void ParamsModel::applyDefault()
{
    menu_item_t* item = nullptr;
    Parameter* param = nullptr;
    QModelIndex index;
    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param){
            index = createIndex(menu_item_pos(item), PARAMS_COL_VALUE, static_cast<void*>(item));
            param->applyDefault();
            emit dataChanged(index, index);
        }
    }
}

void ParamsModel::paramsUpdated()
{
    menu_item_t* item = nullptr;
    QModelIndex index;
    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        if(menu_item_user_data(item) != nullptr){
            index = createIndex(menu_item_pos(item), PARAMS_COL_VALUE, static_cast<void*>(item));
            emit dataChanged(index, index);
        }
    }
}

void ParamsModel::setupParams()
{
    param_id_t id = 0;
    menu_item_t* item = nullptr;
    Parameter* param = nullptr;
    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        id = menu_item_id(item);
        param = Parameter::byId(id);
        menu_item_set_user_data(item, static_cast<menu_user_data_t>(param));
    }
}

void ParamsModel::cleanUpParams()
{
    menu_item_t* item = nullptr;
    Parameter* param = nullptr;
    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param) delete param;
    }
}
