#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <stdint.h>
#include <math.h>
#include "drive_types.h"
#include "parameters_ids.h"
#include "parameters_list.h"



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

    virtual void setRange(int min_val, int max_val) = 0;
    virtual void setRange(unsigned int min_val, unsigned int max_val) = 0;
    virtual void setRange(float min_val, float max_val) = 0;

    virtual void setDefault(int def_val) = 0;
    virtual void setDefault(unsigned int def_val) = 0;
    virtual void setDefault(float def_val) = 0;

    virtual int minToInt() const = 0;
    virtual unsigned int minToUInt() const = 0;
    virtual float minToFloat() const = 0;

    virtual int maxToInt() const = 0;
    virtual unsigned int maxToUInt() const = 0;
    virtual float maxToFloat() const = 0;

    virtual int defToInt() const = 0;
    virtual unsigned int defToUInt() const = 0;
    virtual float defToFloat() const = 0;

    virtual bool validate(int val) const = 0;
    virtual bool validate(unsigned int val) const = 0;
    virtual bool validate(float val) const = 0;

    virtual void applyDefault() = 0;
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

    /**
     * @brief Конструктор из дескриптора.
     * @param parameter_descr Дескриптор параметра.
     */
    Parameter(const param_descr_t* parameter_descr);
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
     * @brief Получает флаги параметра.
     * @return Флаги параметра.
     */
    param_flags_t flags() const;

    /**
     * @brief Устанавливает флаги параметра.
     * @param parameter_flags Флаги параметра.
     */
    void setFlags(param_flags_t parameter_flags);

    /**
     * @brief Получает строковое представление значения параметра.
     * @return Строковое представление значения параметра.
     */
    QString toString() const;

    /**
     * @brief Получает строковое представление минимального значения параметра.
     * @return Строковое представление минимального значения параметра.
     */
    QString minToString() const;

    /**
     * @brief Получает строковое представление максимального значения параметра.
     * @return Строковое представление максимального значения параметра.
     */
    QString maxToString() const;

    /**
     * @brief Получает строковое представление значения по-умолчанию параметра.
     * @return Строковое представление значения по-умолчанию параметра.
     */
    QString defToString() const;

    /**
     * @brief Получает целочисленное знаковое значение параметра.
     * @return Целочисленное знаковое значение параметра.
     */
    int toInt() const;

    /**
     * @brief Получает целочисленное беззнаковое значение параметра.
     * @return Целочисленное беззнаковое значение параметра.
     */
    unsigned int toUInt() const;

    /**
     * @brief Получает значение с плавающей запятой.
     * @return Значение с плавающей запятой.
     */
    float toFloat() const;

    /**
     * @brief Получает сырое(упакованное) значение
     * для передачи в привод.
     * @return Сырое(упакованное) значение.
     */
    uint16_t toRaw() const;

    /**
     * @brief Устанавливает целочисленное знаковое значение.
     * @param val Целочисленное знаковое значение.
     */
    void setInt(int val);

    /**
     * @brief Устанавливает целочисленное беззнаковое значение.
     * @param val Целочисленное беззнаковое значение.
     */
    void setUInt(unsigned int val);

    /**
     * @brief Устанавливает значение с плавающей запятой.
     * @param val Значение с плавающей запятой.
     */
    void setFloat(float val);

    /**
     * @brief Устанавливает сырое(упакованное) значение,
     * полученное от привода.
     * @param val Сырое(упакованное значение).
     */
    void setRaw(uint16_t val);

    /**
     * @brief Устанавливает диапазон значений.
     * @param min_val Минимальное значение.
     * @param max_val Максимальное значение.
     */
    void setRange(int min_val, int max_val);

    /**
     * @brief Устанавливает диапазон значений.
     * @param min_val Минимальное значение.
     * @param max_val Максимальное значение.
     */
    void setRange(unsigned int min_val, unsigned int max_val);

    /**
     * @brief Устанавливает диапазон значений.
     * @param min_val Минимальное значение.
     * @param max_val Максимальное значение.
     */
    void setRange(float min_val, float max_val);

    /**
     * @brief Устанавливает значение по-умолчанию.
     * @param def_val Значение по-умолчанию.
     */
    void setDefault(int def_val);

    /**
     * @brief Устанавливает значение по-умолчанию.
     * @param def_val Значение по-умолчанию.
     */
    void setDefault(unsigned int def_val);

    /**
     * @brief Устанавливает значение по-умолчанию.
     * @param def_val Значение по-умолчанию.
     */
    void setDefault(float def_val);

    /**
     * @brief Получает минимальное значение.
     * @return Минимальное значение.
     */
    int minToInt() const;

    /**
     * @brief Получает минимальное значение.
     * @return Минимальное значение.
     */
    unsigned int minToUInt() const;

    /**
     * @brief Получает минимальное значение.
     * @return Минимальное значение.
     */
    float minToFloat() const;

    /**
     * @brief Получает максимальное значение.
     * @return Максимальное значение.
     */
    int maxToInt() const;

    /**
     * @brief Получает максимальное значение.
     * @return Максимальное значение.
     */
    unsigned int maxToUInt() const;

    /**
     * @brief Получает максимальное значение.
     * @return Максимальное значение.
     */
    float maxToFloat() const;

    /**
     * @brief Получает значение по-умолчанию.
     * @return Значение по-умолчанию.
     */
    int defToInt() const;

    /**
     * @brief Получает значение по-умолчанию.
     * @return Значение по-умолчанию.
     */
    unsigned int defToUInt() const;

    /**
     * @brief Получает значение по-умолчанию.
     * @return Значение по-умолчанию.
     */
    float defToFloat() const;

    /**
     * @brief Проверяет вхождение значения в допустимый диапазон.
     * @param val Значение.
     * @return Флаг вхождения значения в допустимый диапазон.
     */
    bool validate(int val) const;

    /**
     * @brief Проверяет вхождение значения в допустимый диапазон.
     * @param val Значение.
     * @return Флаг вхождения значения в допустимый диапазон.
     */
    bool validate(unsigned int val) const;

    /**
     * @brief Проверяет вхождение значения в допустимый диапазон.
     * @param val Значение.
     * @return Флаг вхождения значения в допустимый диапазон.
     */
    bool validate(float val) const;

    /**
     * @brief Применяет значение по-умолчанию.
     */
    void applyDefault();

    /**
     * @brief Получает дескриптор параметра по
     * идентификатору в сортированном массиве
     * дескрипторов параметров.
     * @param parameter_id Идентификатор параметра.
     * @return Указатель на дескриптор, либо nullptr.
     */
    static const param_descr_t* descrById(param_id_t parameter_id);

    /**
     * @brief Создаёт параметр по дескриптору,
     * который получает по идентификатору в
     * сортированном массиве дескрипторов параметров.
     * @param parameter_id Идентификатор параметра.
     * @return Параметр, либо nullptr.
     */
    static Parameter* byId(param_id_t parameter_id);

    /**
     * @brief Получает строковое представление значения согласно типа параметра.
     * @param param_type Тип параметра.
     * @param val Значение.
     * @return Строковое представление.
     */
    static QString number(param_type_t param_type, int val);

    /**
     * @brief Получает строковое представление значения согласно типа параметра.
     * @param param_type Тип параметра.
     * @param val Значение.
     * @return Строковое представление.
     */
    static QString number(param_type_t param_type, unsigned int val);

    /**
     * @brief Получает строковое представление значения согласно типа параметра.
     * @param param_type Тип параметра.
     * @param val Значение.
     * @return Строковое представление.
     */
    static QString number(param_type_t param_type, float val);

protected:
    Parameter();
    ParameterBase* param;
    param_type_t param_type;
    param_id_t param_id;
    param_flags_t param_flags;

    static float fixed32_to_float(fixed32_t f);
};

/**
 * @brief Шаблонный класс реализации параметра со значением заданного типа.
 */
template <typename T>
class ParameterImpl
        :public ParameterBase
{
public:
    ParameterImpl() :ParameterBase()
    { value = T(); value_def = T();
      value_min = T(); value_max = T(); }
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

    void setRange(int min_val, int max_val)
        { value_min = static_cast<T>(min_val); value_max = static_cast<T>(max_val); }
    void setRange(unsigned int min_val, unsigned int max_val)
        { value_min = static_cast<T>(min_val); value_max = static_cast<T>(max_val); }
    void setRange(float min_val, float max_val)
        { value_min = static_cast<T>(min_val); value_max = static_cast<T>(max_val); }

    void setDefault(int def_val)
        { value_def = static_cast<T>(def_val); }
    void setDefault(unsigned int def_val)
        { value_def = static_cast<T>(def_val); }
    void setDefault(float def_val)
        { value_def = static_cast<T>(def_val); }


    int minToInt() const
        { return static_cast<int>(value_min); }
    unsigned int minToUInt() const
        { return static_cast<unsigned int>(value_min); }
    float minToFloat() const
        { return static_cast<float>(value_min); }

    int maxToInt() const
        { return static_cast<int>(value_max); }
    unsigned int maxToUInt() const
        { return static_cast<unsigned int>(value_max); }
    float maxToFloat() const
        { return static_cast<float>(value_max); }

    int defToInt() const
        { return static_cast<int>(value_def); }
    unsigned int defToUInt() const
        { return static_cast<unsigned int>(value_def); }
    float defToFloat() const
        { return static_cast<float>(value_def); }

    bool validate(int val) const
        { return (static_cast<T>(val) >= value_min) && (static_cast<T>(val) <= value_max); }
    bool validate(unsigned int val) const
        { return (static_cast<T>(val) >= value_min) && (static_cast<T>(val) <= value_max); }
    bool validate(float val) const
        { return (static_cast<T>(val) >= value_min) && (static_cast<T>(val) <= value_max); }

    void applyDefault()
        { value = value_def; }

protected:
    T value;
    T value_min;
    T value_max;
    T value_def;
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
        { value = static_cast<float>(static_cast<int16_t>(val)) / 10.0f; }
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
        { value = static_cast<float>(static_cast<int16_t>(val)) / 100.0f; }
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
        { value = static_cast<float>(static_cast<int16_t>(val)) / 1000.0f; }
};

#endif // PARAMETER_H
