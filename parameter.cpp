#include "parameter.h"

Parameter::Parameter(param_type_t parameter_type)
    :ParameterBase()
{
    param_type = parameter_type;

    param_id = 0;

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

Parameter::Parameter()
{
    param = nullptr;
}
