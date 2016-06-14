#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <stdint.h>
#include <math.h>
#include "parameters_ids.h"



//! @brief Тип идентификатора параметра.
typedef uint16_t param_id_t;


//! @brief Тип значения параметра.
typedef enum _Parameter_Type {
    PARAM_TYPE_INT = 0,
    PARAM_TYPE_UINT = 1,
    PARAM_TYPE_FRACT_10 = 2,
    PARAM_TYPE_FRACT_100 = 3,
    PARAM_TYPE_FRACT_1000 = 4
} param_type_t;

/**
 * @brief Базовый класс параметра привода.
 */
class ParameterBase
{
public:
    ParameterBase() {}
    virtual ~ParameterBase() {}

    virtual int toInt() const = 0;
    virtual unsigned int toUInt() const = 0;
    virtual float toFloat() const = 0;
    virtual uint16_t toRaw() const = 0;

    virtual void setInt(int val) = 0;
    virtual void setUInt(unsigned int val) = 0;
    virtual void setFloat(float val) = 0;
    virtual void setRaw(uint16_t val) = 0;
};

/**
 * @brief Класс параметра привода.
 */
class Parameter
        :public ParameterBase
{
public:
    /**
     * @brief Конструктор.
     * @param param_type Тип параметра.
     */
    explicit Parameter(param_type_t parameter_type);

    /**
     * @brief Конструктор.
     * @param parameter_type Тип параметра.
     * @param parameter_id Идентификатор параметра.
     */
    Parameter(param_type_t parameter_type, param_id_t parameter_id);
    ~Parameter();

    /**
     * @brief Получает тип параметра.
     * @return Тип параметра.
     */
    param_type_t type() const;

    /**
     * @brief Получает идентификатор параметра.
     * @return Идентификатор параметра.
     */
    param_id_t id() const;

    /**
     * @brief Устанавливает идентификатор параметра.
     * @param parameter_id Идентификатор параметра.
     */
    void setId(param_id_t parameter_id);

    /**
     * @brief Получает строковое представление значения параметра.
     * @return Строковое представление значения параметра.
     */
    QString toString() const;

    /**
     * @brief Получает целочисленное знаковое значение параметра.
     * @return Целочисленное знаковое значение параметра.
     */
    virtual int toInt() const;

    /**
     * @brief Получает целочисленное беззнаковое значение параметра.
     * @return Целочисленное беззнаковое значение параметра.
     */
    virtual unsigned int toUInt() const;

    /**
     * @brief Получает значение с плавающей запятой.
     * @return Значение с плавающей запятой.
     */
    virtual float toFloat() const;

    /**
     * @brief Получает сырое(упакованное) значение
     * для передачи в привод.
     * @return Сырое(упакованное) значение.
     */
    virtual uint16_t toRaw() const;

    /**
     * @brief Устанавливает целочисленное знаковое значение.
     * @param val Целочисленное знаковое значение.
     */
    virtual void setInt(int val);

    /**
     * @brief Устанавливает целочисленное беззнаковое значение.
     * @param val Целочисленное беззнаковое значение.
     */
    virtual void setUInt(unsigned int val);

    /**
     * @brief Устанавливает значение с плавающей запятой.
     * @param val Значение с плавающей запятой.
     */
    virtual void setFloat(float val);

    /**
     * @brief Устанавливает сырое(упакованное) значение,
     * полученное от привода.
     * @param val Сырое(упакованное значение).
     */
    virtual void setRaw(uint16_t val);

protected:
    Parameter();
    ParameterBase* param;
    param_type_t param_type;
    param_id_t param_id;
};

/**
 * @brief Шаблонный класс реализации параметра со значением заданного типа.
 */
template <typename T>
class ParameterImpl
        :public ParameterBase
{
public:
    ParameterImpl() :ParameterBase() {}
    ~ParameterImpl() {}

    int toInt() const
        { return static_cast<int>(value); }
    unsigned int toUInt() const
        { return static_cast<unsigned int>(value); }
    float toFloat() const
        { return static_cast<float>(value); }
    uint16_t toRaw() const
        { return static_cast<uint16_t>(value); }

    void setInt(int val)
        { value = static_cast<T>(val); }
    void setUInt(unsigned int val)
        { value = static_cast<T>(val); }
    void setFloat(float val)
        { value = static_cast<T>(val); }
    void setRaw(uint16_t val)
        { value = static_cast<T>(val); }

protected:
    T value;
};

/**
 * @brief Тип параметра с целочисленным знаковым значением.
 */
typedef ParameterImpl<int> ParameterInt;

/**
 * @brief Тип параметра с целочисленным беззнаковым значением.
 */
typedef ParameterImpl<unsigned int> ParameterUInt;

/**
 * @brief Класс параметра со значение с плавающей запятой и точностью 0.1.
 */
class ParameterFract10
        :public ParameterImpl<float>
{
public:
    ParameterFract10() :ParameterImpl<float>() {}
    ~ParameterFract10() {}

    uint16_t toRaw() const
        { return static_cast<uint16_t>(floor(value * 10.0f + 0.5f)); }

    void setRaw(uint16_t val)
        { value = static_cast<float>(val) / 10.0f; }
};


/**
 * @brief Класс параметра со значение с плавающей запятой и точностью 0.01.
 */
class ParameterFract100
        :public ParameterImpl<float>
{
public:
    ParameterFract100() :ParameterImpl<float>() {}
    ~ParameterFract100() {}

    uint16_t toRaw() const
        { return static_cast<uint16_t>(floor(value * 100.0f + 0.5f)); }

    void setRaw(uint16_t val)
        { value = static_cast<float>(val) / 100.0f; }
};


/**
 * @brief Класс параметра со значение с плавающей запятой и точностью 0.001.
 */
class ParameterFract1000
        :public ParameterImpl<float>
{
public:
    ParameterFract1000() :ParameterImpl<float>() {}
    ~ParameterFract1000() {}

    uint16_t toRaw() const
        { return static_cast<uint16_t>(floor(value * 1000.0f + 0.5f)); }

    void setRaw(uint16_t val)
        { value = static_cast<float>(val) / 1000.0f; }
};

#endif // PARAMETER_H
