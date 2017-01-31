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
    PARAM_TYPE_FRACT_1000 = 4,
    PARAM_TYPE_FRACT_10000 = 5
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


//! Флаги элементов меню.
typedef enum _Menu_Flag {
    MENU_FLAG_NONE  = 0,
    MENU_FLAG_DATA  = 1,
    MENU_FLAG_CMD   = 2,
    MENU_FLAG_ADMIN = 4,
    MENU_FLAG_ROOT  = 8
} menu_flag_t;


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


//! Режим останова.
typedef enum _Ramp_Stop_Mode {
    RAMP_STOP_MODE_NORMAL = 0, //!< Нормальный останов.
    RAMP_STOP_MODE_FAST   = 1  //!< Быстрый останов.
} ramp_stop_mode_t;

//! Тип задания разгона.
typedef uint32_t ramp_reference_t;

//! Минимальное задание.
#define RAMP_REFERENCE_MIN 0
//! Максимальное задание.
#define RAMP_REFERENCE_MAX 100

//! Тип времени разгона.
typedef int32_t ramp_time_t;

//! Максимальное время разгона.
#define RAMP_TIME_MIN 1

//! Максимальное время разгона.
#define RAMP_TIME_MAX 300

//! Время изменения задания по-умолчанию.
#define RAMP_REFERENCE_TIME_DEFAULT 10

//! Время разгона по-умолчанию.
#define RAMP_START_TIME_DEFAULT 30

//! Время разгона по-умолчанию.
#define RAMP_STOP_TIME_DEFAULT 20

//! Время разгона по-умолчанию.
#define RAMP_FAST_STOP_TIME_DEFAULT 2


//! Тип состояния регулятора привода.
typedef enum _Drive_Regulator_State {
    DRIVE_REGULATOR_STATE_IDLE = 0,
    DRIVE_REGULATOR_STATE_START,
    DRIVE_REGULATOR_STATE_RUN,
    DRIVE_REGULATOR_STATE_STOP
} drive_regulator_state_t;

//! Тип задания.
typedef ramp_reference_t reference_t;
//! Минимальное задание.
#define REFERENCE_MIN 0
#define REFERENCE_MIN_F 0
//! Максимальное задание.
#define REFERENCE_MAX 100
#define REFERENCE_MAX_F 0x640000


//! Флаги привода - тип флага.
typedef enum _Drive_Flag {
    DRIVE_FLAG_NONE             = 0x0,
    DRIVE_FLAG_POWER_CALIBRATED = 0x1,
    DRIVE_FLAG_POWER_DATA_AVAIL = 0x2
} drive_flag_t;

//! Тип флагов привода.
typedef uint32_t drive_flags_t;

//! Тип статуса привода.
typedef enum _Drive_Status {
    DRIVE_STATUS_INIT       = 0, //!< Инициализация.
    DRIVE_STATUS_IDLE       = 1, //!< Простой (готовность).
    DRIVE_STATUS_RUN        = 2, //!< Работа.
    DRIVE_STATUS_ERROR      = 3  //!< Ошибка.
} drive_status_t;

//! Тип состояния машины состояний привода.
typedef enum _Drive_State {
    DRIVE_STATE_INIT        = 0, //!< Инициализация.
    DRIVE_STATE_CALIBRATION = 1, //!< Калибровка питания.
    DRIVE_STATE_IDLE        = 2, //!< Простой (готовность).
    DRIVE_STATE_START       = 3, //!< Запуск.
    DRIVE_STATE_RUN         = 4, //!< Работа.
    DRIVE_STATE_STOP        = 5, //!< Останов.
    DRIVE_STATE_STOP_ERROR  = 6, //!< Останов при ошибке.
    DRIVE_STATE_ERROR       = 7  //!< Ошибка.
} drive_state_t;

//! Тип ошибки привода.
typedef enum _Drive_Error {
    DRIVE_ERROR_NONE                 = 0x0, //!< Нет ошибки.
    DRIVE_ERROR_POWER_DATA_NOT_AVAIL = 0x1, //!< Данные питания не поступают с АЦП.
    DRIVE_ERROR_POWER_INVALID        = 0x2, //!< Неправильные значения питания, см. drive_power_error_t.
    DRIVE_ERROR_EMERGENCY_STOP       = 0x4, //!< Аварийный останов (Грибок).
    DRIVE_ERROR_PHASE                = 0x8, //!< Ошибка фаз.
    DRIVE_ERROR_THERMAL_OVERLOAD     = 0x10, //!< Тепловая защита.
    DRIVE_ERROR_ROT_BREAK            = 0x20 //!< Обрыв якоря.
} drive_error_t;

//! Тип ошибок привода.
typedef uint32_t drive_errors_t;

//! Тип ошибки питания привода.
typedef enum _Drive_Power_Error {
    DRIVE_POWER_ERROR_NONE           = 0x0,
    DRIVE_POWER_ERROR_UNDERFLOW_Ua   = 0x1,
    DRIVE_POWER_ERROR_OVERFLOW_Ua    = 0x2,
    DRIVE_POWER_ERROR_UNDERFLOW_Ub   = 0x4,
    DRIVE_POWER_ERROR_OVERFLOW_Ub    = 0x8,
    DRIVE_POWER_ERROR_UNDERFLOW_Uc   = 0x10,
    DRIVE_POWER_ERROR_OVERFLOW_Uc    = 0x20,
    DRIVE_POWER_ERROR_UNDERFLOW_Urot = 0x40,
    DRIVE_POWER_ERROR_OVERFLOW_Urot  = 0x80,
    DRIVE_POWER_ERROR_UNDERFLOW_Ia   = 0x100,
    DRIVE_POWER_ERROR_OVERFLOW_Ia    = 0x200,
    DRIVE_POWER_ERROR_UNDERFLOW_Ib   = 0x400,
    DRIVE_POWER_ERROR_OVERFLOW_Ib    = 0x800,
    DRIVE_POWER_ERROR_UNDERFLOW_Ic   = 0x1000,
    DRIVE_POWER_ERROR_OVERFLOW_Ic    = 0x2000,
    DRIVE_POWER_ERROR_UNDERFLOW_Irot = 0x4000,
    DRIVE_POWER_ERROR_OVERFLOW_Irot  = 0x8000,
    DRIVE_POWER_ERROR_UNDERFLOW_Iexc = 0x10000,
    DRIVE_POWER_ERROR_OVERFLOW_Iexc  = 0x20000,
    DRIVE_POWER_ERROR_UNDERFLOW_Ifan = 0x40000,
    DRIVE_POWER_ERROR_OVERFLOW_Ifan  = 0x80000,
    DRIVE_POWER_ERROR_UNDERFLOW_Iref = 0x100000,
    DRIVE_POWER_ERROR_OVERFLOW_Iref  = 0x200000,
    DRIVE_POWER_ERROR_IDLE_Ia        = 0x400000,
    DRIVE_POWER_ERROR_IDLE_Ib        = 0x800000,
    DRIVE_POWER_ERROR_IDLE_Ic        = 0x1000000,
    DRIVE_POWER_ERROR_IDLE_Urot      = 0x2000000,
    DRIVE_POWER_ERROR_IDLE_Irot      = 0x4000000,
    DRIVE_POWER_ERROR_IDLE_Iexc      = 0x8000000,
    DRIVE_POWER_ERROR_ALL            = 0xfffffff
} drive_power_error_t;

//! Тип ошибок питания привода.
typedef uint32_t drive_power_errors_t;

typedef enum _Drive_Warning {
    DRIVE_WARNING_NONE  = 0x0, //!< Нет предупреждений.
    DRIVE_WARNING_POWER = 0x1,  //!< Предупреждение по питанию.
    DRIVE_WARNING_THERMAL_OVERLOAD = 0x400000 //!< Перегрев.
} drive_warning_t;

//! Тип предупреждений привода.
typedef uint32_t drive_warnings_t;

typedef enum _Drive_Power_Warning {
    DRIVE_POWER_WARNING_NONE           = 0x0,
    DRIVE_POWER_WARNING_UNDERFLOW_Ua   = 0x1,
    DRIVE_POWER_WARNING_OVERFLOW_Ua    = 0x2,
    DRIVE_POWER_WARNING_UNDERFLOW_Ub   = 0x4,
    DRIVE_POWER_WARNING_OVERFLOW_Ub    = 0x8,
    DRIVE_POWER_WARNING_UNDERFLOW_Uc   = 0x10,
    DRIVE_POWER_WARNING_OVERFLOW_Uc    = 0x20,
    DRIVE_POWER_WARNING_UNDERFLOW_Urot = 0x40,
    DRIVE_POWER_WARNING_OVERFLOW_Urot  = 0x80,
    DRIVE_POWER_WARNING_UNDERFLOW_Ia   = 0x100,
    DRIVE_POWER_WARNING_OVERFLOW_Ia    = 0x200,
    DRIVE_POWER_WARNING_UNDERFLOW_Ib   = 0x400,
    DRIVE_POWER_WARNING_OVERFLOW_Ib    = 0x800,
    DRIVE_POWER_WARNING_UNDERFLOW_Ic   = 0x1000,
    DRIVE_POWER_WARNING_OVERFLOW_Ic    = 0x2000,
    DRIVE_POWER_WARNING_UNDERFLOW_Irot = 0x4000,
    DRIVE_POWER_WARNING_OVERFLOW_Irot  = 0x8000,
    DRIVE_POWER_WARNING_UNDERFLOW_Iexc = 0x10000,
    DRIVE_POWER_WARNING_OVERFLOW_Iexc  = 0x20000,
    DRIVE_POWER_WARNING_UNDERFLOW_Ifan = 0x40000,
    DRIVE_POWER_WARNING_OVERFLOW_Ifan  = 0x80000,
    DRIVE_POWER_WARNING_UNDERFLOW_Iref = 0x100000,
    DRIVE_POWER_WARNING_OVERFLOW_Iref  = 0x200000,
    DRIVE_POWER_WARNING_IDLE_Ia        = 0x400000,
    DRIVE_POWER_WARNING_IDLE_Ib        = 0x800000,
    DRIVE_POWER_WARNING_IDLE_Ic        = 0x1000000,
    DRIVE_POWER_WARNING_IDLE_Urot      = 0x2000000,
    DRIVE_POWER_WARNING_IDLE_Irot      = 0x4000000,
    DRIVE_POWER_WARNING_IDLE_Iexc      = 0x8000000,
    DRIVE_POWER_WARNING_ALL            = 0xfffffff
} drive_power_warning_t;

//! Тип предупреждений питания привода.
typedef uint32_t drive_power_warnings_t;

//! Перечисление состояний инициализации привода.
typedef enum _Drive_Init_State {
    DRIVE_INIT_NONE        = 0,
    DRIVE_INIT_BEGIN       = 1,
    DRIVE_INIT_WAIT_PHASES = 2,
    DRIVE_INIT_WAIT_POWER  = 3,
    DRIVE_INIT_DONE        = 4
} drive_init_state_t;

//! Перечисление состояний калибровки питания.
typedef enum _Drive_Power_Calibration {
    DRIVE_PWR_CALIBRATION_NONE    = 0, //!< Нет калибровки.
    DRIVE_PWR_CALIBRATION_BEGIN   = 1, //!< Нужно выполнить калибровку.
    DRIVE_PWR_CALIBRATION_RUNNING = 2, //!< Калибровка выполняется.
    DRIVE_PWR_CALIBRATION_DONE    = 3  //!< Калибровка выполнена.
} drive_power_calibration_t;

//! Перечисление состояний запуска привода.
typedef enum _Drive_Starting {
    DRIVE_STARTING_NONE     = 0, //!< Не запускается.
    DRIVE_STARTING_BEGIN    = 1, //!< Нужно запустить.
    DRIVE_STARTING_WAIT_EXC = 2, //!< Ожидание возбуждения.
    DRIVE_STARTING_RAMP     = 3, //!< Разгон.
    DRIVE_STARTING_DONE     = 4  //!< Запущен.
} drive_starting_t;

//! Перечисление состояний останова привода.
typedef enum _Drive_Stopping {
    DRIVE_STOPPING_NONE     = 0, //!< Не останавливается.
    DRIVE_STOPPING_BEGIN    = 1, //!< Нужно остановить.
    DRIVE_STOPPING_RAMP     = 2, //!< Торможение.
    DRIVE_STOPPING_WAIT_ROT = 3, //!< Ожидане остановки якоря.
    DRIVE_STOPPING_WAIT_EXC = 4, //!< Ожидане возвращения к нулю возбуждения.
    DRIVE_STOPPING_DONE     = 5  //!< Остановлен.
} drive_stopping_t;

//! Перечисление состояний останова привода при ошибке.
typedef enum _Drive_Err_Stopping {
    DRIVE_ERR_STOPPING_NONE     = 0, //!< Не останавливается.
    DRIVE_ERR_STOPPING_BEGIN    = 1, //!< Нужно остановить.
    DRIVE_ERR_STOPPING_WAIT_ROT = 2, //!< Ожидане остановки якоря.
    DRIVE_ERR_STOPPING_WAIT_EXC = 3, //!< Ожидане возвращения к нулю возбуждения.
    DRIVE_ERR_STOPPING_DONE     = 4//!< Остановлен.
} drive_err_stopping_t;

//! Перечисление типа останова привода.
typedef enum _Drive_Stop_Mode {
    DRIVE_STOP_MODE_NORMAL = 0, //!< Нормальный останов.
    DRIVE_STOP_MODE_FAST   = 1, //!< Быстрый останов.
    DRIVE_STOP_MODE_COAST  = 2, //!< Останов выбегом.
} drive_stop_mode_t;

//! Тип направления.
typedef enum _DriveDir {
    DRIVE_DIR_UNK = 0,
    DRIVE_DIR_FORW = 1,
    DRIVE_DIR_BACKW = 2
} drive_dir_t;


//! Тип результата проверки питания.
typedef enum _Drive_Pwr_Check_Res {
    DRIVE_PWR_CHECK_NORMAL = 0,
    DRIVE_PWR_CHECK_WARN_UNDERFLOW,
    DRIVE_PWR_CHECK_WARN_OVERFLOW,
    DRIVE_PWR_CHECK_FAULT_UNDERFLOW,
    DRIVE_PWR_CHECK_FAULT_OVERFLOW,
} drive_pwr_check_res_t;

//! Тип результата проверки тепловой защиты.
typedef enum _Drive_Top_Check_Res {
    DRIVE_TOP_CHECK_NORMAL = 0,
    DRIVE_TOP_CHECK_HEATING,
    DRIVE_TOP_CHECK_COOLING,
    DRIVE_TOP_CHECK_OVERHEAT
} drive_top_check_res_t;

//! Тип результата проверки обрыва якоря.
typedef enum _Drive_Break_Check_Res {
    DRIVE_BREAK_CHECK_NORMAL = 0,
    DRIVE_BREAK_CHECK_FAIL
} drive_break_check_res_t;

//! Тип действия при активации элемента защиты.
typedef enum _Drive_Prot_Action {
    DRIVE_PROT_ACTION_IGNORE         = 0,
    DRIVE_PROT_ACTION_WARNING        = 1,
    DRIVE_PROT_ACTION_FAST_STOP      = 2,
    DRIVE_PROT_ACTION_COAST_STOP     = 3,
    DRIVE_PROT_ACTION_EMERGENCY_STOP = 4
} drive_prot_action_t;


//! Перечисление инвертированности логического уровня сигнала.
typedef enum _Drive_Dio_Inversion {
    DRIVE_DIO_INVERSION_NONE     = 0,
    DRIVE_DIO_INVERSION_INVERTED = 1,
} drive_dio_inversion_t;

//! Перечисление типов цифровых входов.
typedef enum _Drive_Dio_Input_Type {
    DRIVE_DIO_IN_NONE           = 0,
    DRIVE_DIO_IN_START_STOP     = 1,
    DRIVE_DIO_IN_EMERGENCY_STOP = 2,
    DRIVE_DIO_IN_REFERENCE_INC  = 3,
    DRIVE_DIO_IN_REFERENCE_DEC  = 4,
    DRIVE_DIO_IN_CLEAR_ERRORS   = 5,
    DRIVE_DIO_IN_USER           = 6,
} drive_dio_input_type_t;

//! Перечисление типов цифровых выходов.
typedef enum _Drive_Dio_Output_Type {
    DRIVE_DIO_OUT_NONE    = 0,
    DRIVE_DIO_OUT_OK      = 1,
    DRIVE_DIO_OUT_READY   = 2,
    DRIVE_DIO_OUT_RUNNING = 3,
    DRIVE_DIO_OUT_ERROR   = 4,
    DRIVE_DIO_OUT_WARNING = 5,
    DRIVE_DIO_OUT_USER    = 6,
} drive_dio_output_type_t;

//! Перечисление номеров цифровых входов.
typedef enum _Drive_Dio_Input {
    DRIVE_DIO_INPUT_1 = 0,
    DRIVE_DIO_INPUT_2 = 1,
    DRIVE_DIO_INPUT_3 = 2,
    DRIVE_DIO_INPUT_4 = 3,
    DRIVE_DIO_INPUT_5 = 4,
} drive_dio_input_t;

//! Перечисление номеров цифровых выходов.
typedef enum _Drive_Dio_Output {
    DRIVE_DIO_OUTPUT_1 = 0,
    DRIVE_DIO_OUTPUT_2 = 1,
    DRIVE_DIO_OUTPUT_3 = 2,
    DRIVE_DIO_OUTPUT_4 = 3,
} drive_dio_output_t;


//! Тип ошибки.
typedef enum _DrivePhaseErr {
    PHASE_NO_ERROR     = 0,
    PHASE_INVALID      = 0x1,
    PHASE_A_ERROR      = 0x2,
    PHASE_B_ERROR      = 0x4,
    PHASE_C_ERROR      = 0x8,
    PHASE_AB_ERROR     = 0x10,
    PHASE_BC_ERROR     = 0x20,
    PHASE_AC_ERROR     = 0x40,
    PHASE_A_TIME_ERROR = 0x80,
    PHASE_B_TIME_ERROR = 0x100,
    PHASE_C_TIME_ERROR = 0x200
} drive_phase_error_t;

//! Тип ошибок фаз.
typedef uint32_t drive_phase_errors_t;

//! Тип времени между фазами.
typedef uint16_t phase_time_t;


//! Тип идентификатора события.
typedef uint8_t drive_event_id_t;

//! Тип индекса события.
typedef uint8_t drive_event_index_t;

//! Тип события.
typedef enum _Drive_Event_Type {
    DRIVE_EVENT_TYPE_ERROR = 0 //!< Ошибка.
} drive_event_type_t;

#pragma pack(push, 1)
//! Тип события.
typedef struct _Drive_Event {
    drive_event_id_t id; //!< Идентификатор события.
    uint8_t type; //!< Тип события.
    uint8_t state; //!< Состояние привода.
    uint8_t direction; //!< Направление вращения привода.
    uint8_t init_state; //!< Состояние инициализации.
    uint8_t calibration_state; //!< Состояние калибровки привода.
    uint8_t starting_state; //!< Состояние запуска привода.
    uint8_t stopping_state; //!< Состояние останова привода.
    uint8_t err_stopping_state; //!< Состояние останова по ошибке привода.
    reference_t reference; //!< Задание привода.
    drive_flags_t flags; //!< Флаги привода.
    drive_warnings_t warnings; //!< Предупреждения привода.
    drive_errors_t errors; //!< Ошибки привода.
    drive_power_warnings_t power_warnings; //!< Предупреждения питания привода.
    drive_power_errors_t power_errors; //!< Ошибки питания привода.
    drive_phase_errors_t phase_errors; //!< Ошибки фаз привода.
    phase_time_t phase_a_time; //!< Время фазы A.
    phase_time_t phase_b_time; //!< Время фазы B.
    phase_time_t phase_c_time; //!< Время фазы C.
    uint32_t time; //!< Время возникновения события.
    uint16_t crc; //!< Контрольная сумма.
} drive_event_t;
#pragma pack(pop)


//! Тип значения в осциллограмме (fixed11_5_t).
typedef int16_t osc_value_t;

//! Число бит дробной части значения осциллограммы.
#define OSC_VALUE_FRACT_BITS 5

//! Число капналов осциллограмм.
#define DRIVE_POWER_OSC_CHANNELS_COUNT 9

//! Осциллограмма Ua.
#define DRIVE_POWER_OSC_CHANNEL_Ua 0
//! Осциллограмма Ub.
#define DRIVE_POWER_OSC_CHANNEL_Ub 1
//! Осциллограмма Uc.
#define DRIVE_POWER_OSC_CHANNEL_Uc 2
//! Осциллограмма Ia.
#define DRIVE_POWER_OSC_CHANNEL_Ia 3
//! Осциллограмма Ib.
#define DRIVE_POWER_OSC_CHANNEL_Ib 4
//! Осциллограмма Ic.
#define DRIVE_POWER_OSC_CHANNEL_Ic 5
//! Осциллограмма Urot.
#define DRIVE_POWER_OSC_CHANNEL_Urot 6
//! Осциллограмма Irot.
#define DRIVE_POWER_OSC_CHANNEL_Irot 7
//! Осциллограмма Iexc.
#define DRIVE_POWER_OSC_CHANNEL_Iexc 8

//! Длина канала осциллограммы (1.75 периода до и после аварии).
#define DRIVE_POWER_OSC_CHANNEL_LEN (224) // (128 / 2) * (1.75 * 2) * 9 * 2 == 4032

//! Размер в байтах канала осциллограммы.
#define DRIVE_POWER_OSC_CHANNEL_SIZE (DRIVE_POWER_OSC_CHANNEL_LEN * sizeof(osc_value_t))

//! Время канала осциллограммы (1.75 * 2 * 20)
#define DRIVE_POWER_OSC_CHANNEL_TIME_MS (70)


#endif // DRIVE_TYPES_H
