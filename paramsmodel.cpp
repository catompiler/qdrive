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



class MenuValue
        :public menu_value_t
{
public:
    MenuValue(menu_value_type_t t){
        type = t;
    }
};

class MenuValueString :public MenuValue {
public:
    MenuValueString(const char* val = NULL) :MenuValue(MENU_VALUE_TYPE_STRING)
        { value_string = val; }
};

class MenuValueInt :public MenuValue {
public:
    MenuValueInt(int val = 0) :MenuValue(MENU_VALUE_TYPE_INT)
    { value_int = val; }
};

class MenuValueBool :public MenuValue {
public:
    MenuValueBool(bool val = false) :MenuValue(MENU_VALUE_TYPE_BOOL)
    { value_bool = val; }
};

class MenuValueFixed :public MenuValue {
public:
    MenuValueFixed(fixed32_t val = 0) :MenuValue(MENU_VALUE_TYPE_FIXED)
    { value_fixed = val; }
};

class MenuValueEnum :public MenuValue {
public:
    MenuValueEnum(size_t current, size_t count, MenuValue* val) :MenuValue(MENU_VALUE_TYPE_ENUM)
    { value_enum.current = current; value_enum.count = count; value_enum.values = val; }
};

class MenuValueCustom :public MenuValue {
public:
    MenuValueCustom(void* val) :MenuValue(MENU_VALUE_TYPE_CUSTOM)
    { value_custom = val; }
};


//! Объявляет переменную и инициалищирует массив значений элемента меню.
#define MENU_VALUES(name, ...) static MenuValue name[] = {__VA_ARGS__}


//! Инициализирует пустое значение элемента меню.
#define MAKE_MENU_VALUE_NONE()\
            MenuValue(MENU_VALUE_TYPE_NONE)
//! Инициализирует строковое значение элемента меню.
#define MAKE_MENU_VALUE_STRING(arg_value_string)\
            MenuValueString(arg_value_string)
//! Инициализирует целочисленное значение элемента меню.
#define MAKE_MENU_VALUE_INT(arg_value_int)\
            MenuValueInt(arg_value_int)
//! Инициализирует логическое значение элемента меню.
#define MAKE_MENU_VALUE_BOOL(arg_value_bool)\
            MenuValueBool(arg_value_bool)
//! Инициализирует значение числа с фиксированной запятой элемента меню.
#define MAKE_MENU_VALUE_FIXED(arg_value_fixed)\
            MenuValueFixed(arg_value_fixed)
//! Инициализирует перечислимое значение элемента меню.
#define MAKE_MENU_VALUE_ENUM(arg_current, arg_count, arg_values)\
            MenuValueEnum(arg_current, arg_count, arg_values)
//! Инициализирует пользовательское значение элемента меню.
#define MAKE_MENU_VALUE_CUSTOM(arg_value_custom)\
            MenuValueCustom(arg_value_custom)


//! Обявляет переменную пустого значения элемента меню.
#define MENU_VALUE_NONE(name)\
        static MenuValue name(MENU_VALUE_TYPE_NONE)
//! Обявляет переменную строкового значения элемента меню.
#define MENU_VALUE_STRING(name, arg_value_string)\
        static MenuValueString name(arg_value_string)
//! Обявляет переменную целочисленного значения элемента меню.
#define MENU_VALUE_INT(name, arg_value_int)\
        static MenuValueInt name(arg_value_int)
//! Обявляет переменную логического значения элемента меню.
#define MENU_VALUE_BOOL(name, arg_value_bool)\
        static MenuValueBool name(arg_value_bool)
//! Обявляет переменную значения числа с фиксированной запятой элемента меню.
#define MENU_VALUE_FIXED(name, arg_value_fixed)\
        static MenuValueFixed name(arg_value_fixed)
//! Обявляет переменную перичислимого значения элемента меню.
#define MENU_VALUE_ENUM(name, arg_current, arg_count, arg_values)\
        static MenuValueEnum name(arg_current, arg_count, arg_values)
//! Обявляет переменную пользовательского значения элемента меню.
#define MENU_VALUE_CUSTOM(name, arg_value_custom)\
        static MenuValueCustom name(arg_value_custom)


#include "parameters_menu.h"
#include "translations.h"

//! Класс для однократной инициализации локализации для модели параметров.
class LocInit{
public:
    LocInit(){
        localization_init(trs_main, TRANSLATIONS_COUNT(trs_main));
        localization_set_default_lang(TR_LANG_ID_RU);
        localization_set_lang(TR_LANG_ID_RU);
        localization_set_default_text("НЕТ ПЕРЕВОДА");
    }
    ~LocInit(){}
};
static LocInit _loc_init;


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
                                       menu_descrs, MENU_DESCRS_COUNT(menu_descrs),
                                       [](const menu_descr_t* descr) -> bool {
                                            return ((descr->flags & MENU_FLAG_CMD) == 0) || (descr->id == 0);
                                       });
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
    if(role != Qt::DisplayRole && role != Qt::ToolTipRole){
        if(role != Qt::EditRole || index.column()!= PARAMS_COL_VALUE){
            return QVariant();
        }
    }

    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return QVariant();

    Parameter* param = static_cast<Parameter*>(menu_item_user_data(item));

    if(!param && index.column() != PARAMS_COL_NAME) return QVariant();

    QVariant res;

    menu_value_t* menu_val = menu_item_value(item);

    if(menu_val && menu_value_type(menu_val) == MENU_VALUE_TYPE_ENUM){

        size_t val_index = 0;

        switch(index.column()){
        case PARAMS_COL_NAME:
            if(role == Qt::ToolTipRole && menu_item_help(item))
                { return tr(menu_item_help(item)); }
            return getItemName(item, param);
        case PARAMS_COL_VALUE:
            if(role == Qt::DisplayRole)
                val_index = param->toUInt();
            else
                return param->toVariant();
            break;
        case PARAMS_COL_MIN:
        case PARAMS_COL_MAX:
            return QVariant();
        case PARAMS_COL_DEF:
            val_index = param->defToUInt();
            break;
        }

        if(val_index >= menu_value_enum_count(menu_val)) return QVariant();

        menu_value_t* menu_values = menu_value_enum_values(menu_val);
        if(!menu_values) return QVariant();

        menu_value_t* cur_menu_val = &menu_values[val_index];
        if(!cur_menu_val || menu_value_type(cur_menu_val) != MENU_VALUE_TYPE_STRING) return QVariant();

        trid_t text_trid = TRID(menu_value_string(cur_menu_val));
        const char* text = localization_translate(text_trid);

        if(text) return tr(text);

        return QVariant();
        //return  QVariant(tr(menu_value_string(cur_menu_val)));
    }else{
        switch(index.column()){
        case PARAMS_COL_NAME:
            if(role == Qt::ToolTipRole && menu_item_help(item)){
                trid_t help_trid = TRID(menu_item_help(item));
                const char* help_text = localization_translate(help_trid);
                return tr(help_text);
            }
            return getItemName(item, param);
        case PARAMS_COL_VALUE:
            if(role == Qt::DisplayRole || role == Qt::ToolTipRole)
                res = QVariant(param->toString());
            else
                res = param->toVariant();
            break;
        case PARAMS_COL_MIN:
            res = QVariant(param->minToString());
            break;
        case PARAMS_COL_MAX:
            res = QVariant(param->maxToString());
            break;
        case PARAMS_COL_DEF:
            res = QVariant(param->defToString());
            break;
        }
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
        if(item){
            Parameter* param = static_cast<Parameter*>(menu_item_user_data(item));
            if(param && !(param->flags() & PARAM_FLAG_VIRTUAL)){
                return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
            }
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

Parameter* ParamsModel::parameter(const QModelIndex& index)
{
    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return nullptr;

    return static_cast<Parameter*>(menu_item_user_data(item));
}

const Parameter* ParamsModel::parameter(const QModelIndex& index) const
{
    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return nullptr;

    return static_cast<Parameter*>(menu_item_user_data(item));
}

menu_value_t* ParamsModel::menuValue(const QModelIndex& index)
{
    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return nullptr;

    return menu_item_value(item);
}

const menu_value_t* ParamsModel::menuValue(const QModelIndex& index) const
{
    menu_item_t* item = static_cast<menu_item_t*>(index.internalPointer());

    if(!item) return nullptr;

    return menu_item_value(item);
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

QList<Parameter *> ParamsModel::getRealParamsList()
{
    QList<Parameter*> res_list;

    menu_item_t* item = nullptr;
    Parameter* param = nullptr;

    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param && !(param->flags() & PARAM_FLAG_VIRTUAL)) res_list.append(param);
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

QHash<param_id_t, Parameter*> ParamsModel::getRealParamsHash()
{
    QHash<param_id_t, Parameter*> res_hash;

    menu_item_t* item = nullptr;
    Parameter* param = nullptr;

    for(size_t i = 0; i < MENU_DESCRS_COUNT(menu_descrs); i ++){
        item = &params_items[i];
        param = static_cast<Parameter*>(menu_item_user_data(item));
        if(param && !(param->flags() & PARAM_FLAG_VIRTUAL)) res_hash.insert(param->id(), param);
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

QString ParamsModel::getItemName(menu_item_t* item, Parameter* param) const
{
    trid_t text_trid = TRID(menu_item_text(item));
    if(!text_trid) return QString();

    const char* text = localization_translate(text_trid);
    if(!text) return QString();

    if(param){
        trid_t units_trid = TRID(param->units());
        if(units_trid){
            const char* units = localization_translate(units_trid);
            if(units){
                return tr("%1, %2").arg(tr(text)).arg(tr(units));
            }
        }
    }
    return tr(text);
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
