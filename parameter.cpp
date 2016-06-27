#include "parameter.h"
#include <algorithm>


Parameter::Parameter(param_type_t parameter_type)
    :ParameterBase()
{
    param_type = parameter_type;

    param_id = 0;

    param_flags = PARAM_FLAG_NONE;

    switch(parameter_type){
    case PARAM_TYPE_INT:
        param = new ParameterInt();
        break;
    case PARAM_TYPE_UINT:
        param = new ParameterUInt();
        break;
    case PARAM_TYPE_FRACT_10:
        param = new ParameterFract10();
        break;
    case PARAM_TYPE_FRACT_100:
        param = new ParameterFract100();
        break;
    case PARAM_TYPE_FRACT_1000:
        param = new ParameterFract1000();
        break;
    default:
        param = nullptr;
    }
}

Parameter::Parameter(param_type_t parameter_type, param_id_t parameter_id)
    :Parameter(parameter_type)
{
    param_id = parameter_id;
}

Parameter::Parameter(const param_descr_t *parameter_descr)
    :Parameter(parameter_descr->type, parameter_descr->id)
{
    setFlags(parameter_descr->flags);

    switch(parameter_descr->type){
    case PARAM_TYPE_INT:
        setRange(parameter_descr->min.int_value, parameter_descr->max.int_value);
        setDefault(parameter_descr->def.int_value);
        break;
    case PARAM_TYPE_UINT:
        setRange(parameter_descr->min.uint_value, parameter_descr->max.uint_value);
        setDefault(parameter_descr->def.uint_value);
        break;
    case PARAM_TYPE_FRACT_10:
    case PARAM_TYPE_FRACT_100:
    case PARAM_TYPE_FRACT_1000:
        setRange(fixed32_to_float(parameter_descr->min.fixed_value),
                 fixed32_to_float(parameter_descr->max.fixed_value));
        setDefault(fixed32_to_float(parameter_descr->def.fixed_value));
        break;
    default:
        break;
    }
}

Parameter::~Parameter()
{
    delete param;
}

param_type_t Parameter::type() const
{
    return param_type;
}

param_id_t Parameter::id() const
{
    return param_id;
}

void Parameter::setId(param_id_t parameter_id)
{
    param_id = parameter_id;
}

param_flags_t Parameter::flags() const
{
    return param_flags;
}

void Parameter::setFlags(param_flags_t parameter_flags)
{
    param_flags = parameter_flags;
}

QString Parameter::toString() const
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(toInt());
    case PARAM_TYPE_UINT:
        return QString::number(toUInt());
    case PARAM_TYPE_FRACT_10:
        return QString::number(toFloat(), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(toFloat(), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(toFloat(), 'f', 3);
    default:
        break;
    }
    return QString();
}

QString Parameter::minToString() const
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(minToInt());
    case PARAM_TYPE_UINT:
        return QString::number(minToUInt());
    case PARAM_TYPE_FRACT_10:
        return QString::number(minToFloat(), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(minToFloat(), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(minToFloat(), 'f', 3);
    default:
        break;
    }
    return QString();
}

QString Parameter::maxToString() const
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(maxToInt());
    case PARAM_TYPE_UINT:
        return QString::number(maxToUInt());
    case PARAM_TYPE_FRACT_10:
        return QString::number(maxToFloat(), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(maxToFloat(), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(maxToFloat(), 'f', 3);
    default:
        break;
    }
    return QString();
}

QString Parameter::defToString() const
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(defToInt());
    case PARAM_TYPE_UINT:
        return QString::number(defToUInt());
    case PARAM_TYPE_FRACT_10:
        return QString::number(defToFloat(), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(defToFloat(), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(defToFloat(), 'f', 3);
    default:
        break;
    }
    return QString();
}

int Parameter::toInt() const
{
    if(param) return param->toInt();
    return 0;
}

unsigned int Parameter::toUInt() const
{
    if(param) return param->toUInt();
    return 0;
}

float Parameter::toFloat() const
{
    if(param) return param->toFloat();
    return 0.0f;
}

uint16_t Parameter::toRaw() const
{
    if(param) return param->toRaw();
    return 0;
}

void Parameter::setInt(int val)
{
    if(param) param->setInt(val);
}

void Parameter::setUInt(unsigned int val)
{
    if(param) param->setUInt(val);
}

void Parameter::setFloat(float val)
{
    if(param) param->setFloat(val);
}

void Parameter::setRaw(uint16_t val)
{
    if(param) param->setRaw(val);
}

void Parameter::setRange(int min_val, int max_val)
{
    if(param) param->setRange(min_val, max_val);
}

void Parameter::setRange(unsigned int min_val, unsigned int max_val)
{
    if(param) param->setRange(min_val, max_val);
}

void Parameter::setRange(float min_val, float max_val)
{
    if(param) param->setRange(min_val, max_val);
}

void Parameter::setDefault(int def_val)
{
    if(param) param->setDefault(def_val);
}

void Parameter::setDefault(unsigned int def_val)
{
    if(param) param->setDefault(def_val);
}

void Parameter::setDefault(float def_val)
{
    if(param) param->setDefault(def_val);
}

int Parameter::minToInt() const
{
    if(param) return param->minToInt();
    return 0;
}

unsigned int Parameter::minToUInt() const
{
    if(param) return param->minToUInt();
    return 0;
}

float Parameter::minToFloat() const
{
    if(param) return param->minToFloat();
    return 0;
}

int Parameter::maxToInt() const
{
    if(param) return param->maxToInt();
    return 0;
}

unsigned int Parameter::maxToUInt() const
{
    if(param) return param->maxToUInt();
    return 0;
}

float Parameter::maxToFloat() const
{
    if(param) return param->maxToFloat();
    return 0;
}

int Parameter::defToInt() const
{
    if(param) return param->defToInt();
    return 0;
}

unsigned int Parameter::defToUInt() const
{
    if(param) return param->defToUInt();
    return 0;
}

float Parameter::defToFloat() const
{
    if(param) return param->defToFloat();
    return 0;
}

bool Parameter::validate(int val) const
{
    if(param) return param->validate(val);
    return false;
}

bool Parameter::validate(unsigned int val) const
{
    if(param) return param->validate(val);
    return false;
}

bool Parameter::validate(float val) const
{
    if(param) return param->validate(val);
    return false;
}

void Parameter::applyDefault()
{
    if(param) param->applyDefault();
}

const param_descr_t *Parameter::descrById(param_id_t parameter_id)
{
    const param_descr_t* descrs_end = parameters_descrs + PARAMETERS_COUNT;
    const param_descr_t* descr = std::lower_bound(parameters_descrs, descrs_end, parameter_id,
                                            [](const param_descr_t& pd, param_id_t id) -> int{
                                                if(pd.id < id) return 1;
                                                //if(pd.id > id) return -1;
                                                return 0;
                                            });

    if(descr == descrs_end) return nullptr;
    if(descr->id != parameter_id) return nullptr;

    return descr;
}

Parameter *Parameter::byId(param_id_t parameter_id)
{
    const param_descr_t* descr = descrById(parameter_id);
    if(descr == nullptr) return nullptr;
    return new Parameter(descr);
}

QString Parameter::number(param_type_t param_type, int val)
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(val);
    case PARAM_TYPE_UINT:
        return QString::number(static_cast<unsigned int>(val));
    case PARAM_TYPE_FRACT_10:
        return QString::number(static_cast<float>(val), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(static_cast<float>(val), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(static_cast<float>(val), 'f', 3);
    default:
        break;
    }
    return QString();
}

QString Parameter::number(param_type_t param_type, unsigned int val)
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(val);
    case PARAM_TYPE_UINT:
        return QString::number(val);
    case PARAM_TYPE_FRACT_10:
        return QString::number(static_cast<float>(val), 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(static_cast<float>(val), 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(static_cast<float>(val), 'f', 3);
    default:
        break;
    }
    return QString();
}

QString Parameter::number(param_type_t param_type, float val)
{
    switch(param_type){
    case PARAM_TYPE_INT:
        return QString::number(static_cast<int>(val));
    case PARAM_TYPE_UINT:
        return QString::number(static_cast<int>(val));
    case PARAM_TYPE_FRACT_10:
        return QString::number(val, 'f', 1);
    case PARAM_TYPE_FRACT_100:
        return QString::number(val, 'f', 2);
    case PARAM_TYPE_FRACT_1000:
        return QString::number(val, 'f', 3);
    default:
        break;
    }
    return QString();
}

Parameter::Parameter()
{
    param = nullptr;
}

float Parameter::fixed32_to_float(fixed32_t f)
{
    return static_cast<float>(f) / 0x10000;
}
