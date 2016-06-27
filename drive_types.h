#ifndef DRIVE_TYPES_H
#define DRIVE_TYPES_H


#include <stdint.h>
#include "fixed/fixed32.h"


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


//! Флаги параметров.
typedef enum _Param_Flag {
    PARAM_FLAG_NONE = 0,
    PARAM_FLAG_VIRTUAL = 1
} param_flag_t;

//! Тип флагов параметров.
typedef uint32_t param_flags_t;


//! Тип значения параметра.
typedef union _Param_Value {
    fixed32_t fixed_value;
    uint32_t uint_value;
    int32_t int_value;
} param_value_t;

//! Тип дескриптора параметра.
typedef struct _Param_Descr {
    param_id_t id;
    param_type_t type;
    param_value_t min;
    param_value_t max;
    param_value_t def;
    param_flags_t flags;
} param_descr_t;

//! Начинает список дескрипторов параметров.
#define PARAM_DESCRS(arg_name, arg_count)\
        static const param_descr_t arg_name[arg_count] =

//! Описывает дескриптор параметра.
#define PARAM_DESCR(arg_id, arg_type, arg_min, arg_max, arg_def, arg_flags)\
        { arg_id, arg_type, arg_min, arg_max, arg_def, arg_flags }


//! Тип фазы.
typedef enum _Phase {
    PHASE_UNK = 0,
    PHASE_A = 1,
    PHASE_B = 2,
    PHASE_C = 3
} phase_t;


//! Режим возбуждения.
typedef enum _Drive_Triacs_Exc_Mode {
    DRIVE_TRIACS_EXC_FIXED = 0, //!< Фиксированная подача возбуждения.
    DRIVE_TRIACS_EXC_REGULATED = 1 //!< Регулирование тока возбуждения.
} drive_triacs_exc_mode_t;


//! Тип времени разгона.
typedef int32_t ramp_time_t;

//! Максимальное время разгона.
#define RAMP_TIME_MIN 1

//! Максимальное время разгона.
#define RAMP_TIME_MAX 0x20000

//! Время разгона по-умолчанию.
#define RAMP_TIME_DEFAULT 10


#endif // DRIVE_TYPES_H
