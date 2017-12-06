/**
 * @file parameters_menu.h Файл меню параметров.
 */

#ifndef PARAMETERS_MENU_H
#define PARAMETERS_MENU_H

#include "menu/menu.h"
#include "localization/localization.h"
#include "translations_ids.h"
#include "parameters_ids.h"
#include "commands_ids.h"


/*
 * Перечисления типов для выбора в меню.
 */
//! Перечисление фаз.
MENU_VALUES(menu_enum_phases,
    MAKE_MENU_VALUE_STRING("Не задано"),
    MAKE_MENU_VALUE_STRING("Фаза A"),
    MAKE_MENU_VALUE_STRING("Фаза B"),
    MAKE_MENU_VALUE_STRING("Фаза C")
);
//! Перечисление Да/Нет.
MENU_VALUES(menu_enum_bool,
    MAKE_MENU_VALUE_STRING("Нет"),
    MAKE_MENU_VALUE_STRING("Да")
);
//! Перечисление режима регулирования.
MENU_VALUES(menu_enum_regulator_mode,
    MAKE_MENU_VALUE_STRING("Поддержание скорости"),
    MAKE_MENU_VALUE_STRING("Поддержание момента")
);
//! Перечисление режима перегруза.
MENU_VALUES(menu_enum_overload_mode,
    MAKE_MENU_VALUE_STRING("Нормальный"),
    MAKE_MENU_VALUE_STRING("Тепловой")
);
//! Перечисление режима возбуждения.
MENU_VALUES(menu_enum_exc_mode,
    MAKE_MENU_VALUE_STRING("Фиксированное"),
    MAKE_MENU_VALUE_STRING("Регулируемое"),
    MAKE_MENU_VALUE_STRING("Внешнее")
);
//! Перечисление типа останова.
MENU_VALUES(menu_enum_stop_mode,
    MAKE_MENU_VALUE_STRING("Нормальный"),
    MAKE_MENU_VALUE_STRING("Быстрый"),
    MAKE_MENU_VALUE_STRING("Выбег")
);
//! Перечисление действия защиты.
MENU_VALUES(menu_enum_prot_action,
    MAKE_MENU_VALUE_STRING("Игнорировать"),
    MAKE_MENU_VALUE_STRING("Предупреждение"),
    MAKE_MENU_VALUE_STRING("Быстрый останов"),
    MAKE_MENU_VALUE_STRING("Останов выбегом"),
    MAKE_MENU_VALUE_STRING("Экстренный останов")
);
//! Перечисление типа цифровых входов.
MENU_VALUES(menu_enum_dio_in_type,
    MAKE_MENU_VALUE_STRING("Не задано"),
    MAKE_MENU_VALUE_STRING("Пуск/Стоп"),
    MAKE_MENU_VALUE_STRING("Экстренный останов"),
    MAKE_MENU_VALUE_STRING("Увеличение задания"),
    MAKE_MENU_VALUE_STRING("Уменьшение задания"),
    MAKE_MENU_VALUE_STRING("Сброс ошибок"),
    MAKE_MENU_VALUE_STRING("Пользовательское")
);
//! Перечисление типа цифровых выходов.
MENU_VALUES(menu_enum_dio_out_type,
    MAKE_MENU_VALUE_STRING("Не задано"),
    MAKE_MENU_VALUE_STRING("Привод в порядке"),
    MAKE_MENU_VALUE_STRING("Готовность"),
    MAKE_MENU_VALUE_STRING("Работа"),
    MAKE_MENU_VALUE_STRING("Ошибка"),
    MAKE_MENU_VALUE_STRING("Предупреждение"),
    MAKE_MENU_VALUE_STRING("Пользовательское")
);
//! Перечисление типа работы звукового оповещения.
MENU_VALUES(menu_enum_gui_buzzer,
    MAKE_MENU_VALUE_STRING("Звук включен"),
    MAKE_MENU_VALUE_STRING("Звук выключен"),
);
//! Перечисление выбора отображаемого измерения на плитке
MENU_VALUES(menu_enum_gui_tiles,
    MAKE_MENU_VALUE_STRING("Напряжение фазы A"),      // PARAM_ID_POWER_U_A
    MAKE_MENU_VALUE_STRING("Напряжение фазы B"),      // PARAM_ID_POWER_U_B
    MAKE_MENU_VALUE_STRING("Напряжение фазы C"),      // PARAM_ID_POWER_U_C
    MAKE_MENU_VALUE_STRING("Напряжение якоря"),    // PARAM_ID_POWER_U_ROT
    MAKE_MENU_VALUE_STRING("Ток фазы A"),      // PARAM_ID_POWER_I_A
    MAKE_MENU_VALUE_STRING("Ток фазы B"),      // PARAM_ID_POWER_I_B
    MAKE_MENU_VALUE_STRING("Ток фазы C"),      // PARAM_ID_POWER_I_C
    MAKE_MENU_VALUE_STRING("Ток якоря"),    // PARAM_ID_POWER_I_ROT
    MAKE_MENU_VALUE_STRING("Ток возбуждения"),    // PARAM_ID_POWER_I_EXC
    MAKE_MENU_VALUE_STRING("Ток вентилятора"),    // PARAM_ID_POWER_I_FAN
    MAKE_MENU_VALUE_STRING("Ток 4-20 мА"),    // PARAM_ID_POWER_I_REF
    MAKE_MENU_VALUE_STRING("Температура радиатора"),             // PARAM_ID_HEATSINK_TEMP
    MAKE_MENU_VALUE_STRING("Обороты вентилятора"),          // PARAM_ID_HEATSINK_FAN_RPM
    MAKE_MENU_VALUE_STRING("Угол открытия тиристоров"),   // PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE
    MAKE_MENU_VALUE_STRING("Угол открытия симистора возбуждения"),      // PARAM_ID_TRIAC_EXC_OPEN_ANGLE
    MAKE_MENU_VALUE_STRING("КПД двигателя"),             // PARAM_ID_MOTOR_EFF
    MAKE_MENU_VALUE_STRING("Сопротивление якоря"),           // PARAM_ID_MOTOR_R_ROT
    MAKE_MENU_VALUE_STRING("Обороты двигателя"),             // PARAM_ID_MOTOR_RPM
    MAKE_MENU_VALUE_STRING("Момент двигателя"),          // PARAM_ID_MOTOR_TORQUE
);
//! Перечисление выбора языка интерфейса
MENU_VALUES(menu_enum_gui_languages,
    MAKE_MENU_VALUE_STRING("Русский"),      // Русский
    MAKE_MENU_VALUE_STRING("Английский"),      // Английский
);
//! Перечисления состояний цифрового входа/выхода
MENU_VALUES(menu_enum_gui_digital_states,
    MAKE_MENU_VALUE_STRING("Включен"),      // Включен
    MAKE_MENU_VALUE_STRING("Выключен"),      // Выключен
);
//! Перечисления плиток для выбора отображения текста ошибок и предупреждений
MENU_VALUES(menu_enum_gui_tiles_for_warnings,
    MAKE_MENU_VALUE_STRING("Плитка 1"),      // Плитка 1
    MAKE_MENU_VALUE_STRING("Плитка 2"),      // Плитка 2
    MAKE_MENU_VALUE_STRING("Плитка 3"),      // Плитка 3
    MAKE_MENU_VALUE_STRING("Плитка 4"),      // Плитка 4
    MAKE_MENU_VALUE_STRING("Не отображать"),   // Не отображать
);

//! Перечисление .
//MENU_VALUES(menu_enum_,
//    MAKE_MENU_VALUE_STRING(""), MAKE_MENU_VALUE_STRING("")
//);

// Макрос для определения числа элементов перечисления.
#define MENU_ENUM_LEN(MVARR) (sizeof(MVARR)/sizeof(menu_value_t))

/*
 * Значения перечислений элементов меню.
 */
MENU_VALUE_ENUM(menu_val_phase, 0, MENU_ENUM_LEN(menu_enum_phases), menu_enum_phases);
MENU_VALUE_ENUM(menu_val_bool, 0, MENU_ENUM_LEN(menu_enum_bool), menu_enum_bool);
MENU_VALUE_ENUM(menu_val_exc_mode, 0, MENU_ENUM_LEN(menu_enum_exc_mode), menu_enum_exc_mode);
MENU_VALUE_ENUM(menu_val_regulator_mode, 0, MENU_ENUM_LEN(menu_enum_regulator_mode), menu_enum_regulator_mode);
MENU_VALUE_ENUM(menu_val_overload_mode, 0, MENU_ENUM_LEN(menu_enum_overload_mode), menu_enum_overload_mode);
MENU_VALUE_ENUM(menu_val_stop_mode, 0, MENU_ENUM_LEN(menu_enum_stop_mode), menu_enum_stop_mode);
MENU_VALUE_ENUM(menu_val_prot_action, 0, MENU_ENUM_LEN(menu_enum_prot_action), menu_enum_prot_action);
MENU_VALUE_ENUM(menu_val_dio_in_type, 0, MENU_ENUM_LEN(menu_enum_dio_in_type), menu_enum_dio_in_type);
MENU_VALUE_ENUM(menu_val_dio_out_type, 0, MENU_ENUM_LEN(menu_enum_dio_out_type), menu_enum_dio_out_type);
MENU_VALUE_ENUM(menu_val_gui_buzzer, 0, MENU_ENUM_LEN(menu_enum_gui_buzzer), menu_enum_gui_buzzer);
MENU_VALUE_ENUM(menu_val_gui_tiles, 0, MENU_ENUM_LEN(menu_enum_gui_tiles), menu_enum_gui_tiles);
MENU_VALUE_ENUM(menu_val_gui_languages, 0, MENU_ENUM_LEN(menu_enum_gui_languages), menu_enum_gui_languages);
MENU_VALUE_ENUM(menu_val_gui_digital_states, 0, MENU_ENUM_LEN(menu_enum_gui_digital_states), menu_enum_gui_digital_states);
MENU_VALUE_ENUM(menu_val_gui_tiles_for_warnings, 0, MENU_ENUM_LEN(menu_enum_gui_tiles_for_warnings), menu_enum_gui_tiles_for_warnings);

//MENU_VALUE_ENUM(menu_val_, 0, MENU_ENUM_LEN(menu_enum_), menu_enum_);

#undef MENU_ENUM_LEN


/*
 * Дескрипторы элементов меню.
 */
MENU_DESCRS(menu_descrs) {
    // Статус привода
    MENU_DESCR(0, 0, "Состояние", NULL, 0, 0, 0, 0),
        // Измерения
        MENU_DESCR(1, 0, "Измерения", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_U_A, "Напряжение фазы A", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_U_B, "Напряжение фазы B", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_U_C, "Напряжение фазы C", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_U_ROT, "Напряжение якоря", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_A, "Ток фазы A", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_B, "Ток фазы B", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_C, "Ток фазы C", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_ROT, "Ток якоря", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_EXC, "Ток возбуждения", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_FAN, "Ток вентилятора", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_I_REF, "Ток 4-20 мА", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_POWER_U_WIRES, "Напряжение на проводах", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_E, "ЭДС двигателя", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_HEATSINK_TEMP, "Температура радиатора", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_HEATSINK_FAN_RPM, "Обороты вентилятора", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE, "Угол открытия тиристоров", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIAC_EXC_OPEN_ANGLE, "Угол открытия симистора возбуждения", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            // Мотор
            MENU_DESCR(2, PARAM_ID_MOTOR_EFF, "КПД двигателя", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_E_NOM, "Номинальная ЭДС", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_M_NOM, "Номинальный момент", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_R_ROT, "Сопротивление якоря", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_L_ROT, "Индуктивность якоря", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_R_EXC, "Сопротивление возбуждения", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_RPM, "Обороты двигателя", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_TORQUE, "Момент двигателя", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            MENU_DESCR(2, PARAM_ID_MOTOR_CUR_RPM_MAX, "Тек. макс. обороты двигателя", NULL, 0, MENU_FLAG_VALUE, 0, 0),
        // Цифровые входа
        MENU_DESCR(1, 0, "Цифровые входа", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_1_STATE, "Цифровой вход 1", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_2_STATE, "Цифровой вход 2", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_3_STATE, "Цифровой вход 3", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_4_STATE, "Цифровой вход 4", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IN_5_STATE, "Цифровой вход 5", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
        // Цифровые выхода
        MENU_DESCR(1, 0, "Цифровые выхода", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_1_STATE, "Цифровой выход 1", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_2_STATE, "Цифровой выход 2", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_3_STATE, "Цифровой выход 3", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
            MENU_DESCR(2, PARAM_ID_DIGITAL_OUT_4_STATE, "Цифровой выход 4", NULL, 0, MENU_FLAG_VALUE, 0, &menu_val_gui_digital_states),
        // Наработка привода
        MENU_DESCR(1, 0, "Наработка", NULL, 0, 0, 0, 0),
            // Время работы после последнего включения
            MENU_DESCR(2, PARAM_ID_LAST_RUNTIME, "Время работы после включения", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            // Время работы
            MENU_DESCR(2, PARAM_ID_RUNTIME, "Общее время работы", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            // Время работы вентиляторов
            MENU_DESCR(2, PARAM_ID_FAN_RUNTIME, "Время работы вентилятора", NULL, 0, MENU_FLAG_VALUE, 0, 0),
            // Время во вкл.состоянии (включая готовность)
            MENU_DESCR(2, PARAM_ID_LIFETIME, "Общее время включения", NULL, 0, MENU_FLAG_VALUE, 0, 0),

    // Сеть.
    MENU_DESCR(0, 0, "Сеть", "Параметры сети", 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_U_NOM, "Напряжение", "Номинальное напряжение", 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_I_NOM, "Ток", "Максимальный ток через тиристоры", 0, MENU_FLAG_DATA, 0, 0),

    // Двигатель.
    MENU_DESCR(0, 0, "Двигатель", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_P_NOM, "Номинальная мощность", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_RPM_NOM, "Номинальные обороты", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_RPM_MAX, "Максимальные обороты", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_POLES, "Число полюсов", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_U_ROT_NOM, "Номинальное напряжения якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_I_ROT_NOM, "Номинальный ток якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_I_ROT_MAX, "Максимальный ток якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_U_EXC_NOM, "Номинальное напряжение возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_I_EXC_NOM, "Номинальный ток возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_EFF_NOM, "КПД двигателя", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_R_ROT_NOM, "Сопротивление якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_L_ROT_NOM, "Индуктивность якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_R_EXC_NOM, "Сопротивление возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MOTOR_R_WIRES, "Сопротивление проводов", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_U_ROT_MAX, "Максимальное выходное напряжение", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Возбуждение.
    MENU_DESCR(0, 0, "Возбуждение", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_I_EXC, "Ток", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_EXC_PHASE, "Фаза", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_phase),
        MENU_DESCR(1, PARAM_ID_EXC_MODE, "Режим", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_exc_mode),
        MENU_DESCR(1, 0, "Отключение при нулевом задании", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_ZERO_SPEED_EXC_OFF_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_ZERO_SPEED_EXC_OFF_TIMEOUT, "Время до отключения", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Разгон.
    MENU_DESCR(0, 0, "Разгон", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_START_TIME, "Время разгона 0-100%", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_STOP_TIME, "Время торможения 100-0%", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_FAST_STOP_TIME, "Время быстрого торможения 100-0%", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_REFERENCE_TIME, "Время изменения задания 0-100%", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_RAMP_STOP_MODE, "Режим останова", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_stop_mode),

    // Синхронизация с фазами.
    MENU_DESCR(0, 0, "Синхронизация с фазами", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_PHASE_SYNC_ACCURACY, "Точность", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, 0, "ПИД", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_PHASE_SYNC_PLL_PID_K_P, "Коэффициент П", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PHASE_SYNC_PLL_PID_K_I, "Коэффициент И", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PHASE_SYNC_PLL_PID_K_D, "Коэффициент Д", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Регулятор.
    MENU_DESCR(0, 0, "Регулятор", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_REGULATOR_MODE, "Режим", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_regulator_mode),
        MENU_DESCR(1, PARAM_ID_REGULATOR_IR_COMPENSATION, "IR-компенсация", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        MENU_DESCR(1, 0, "Перегруз", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_MODE, "Режим", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_overload_mode),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_BASE_CURRENT, "Продолжительный ток", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_MAX_CURRENT, "Ток перегрузки", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_TIME, "Время перегруза", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_PERIOD, "Период перегруза", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_REGULATOR_OVERLOAD_DEAD_ZONE, "Зона нечувствительности", NULL, 0, MENU_FLAG_DATA, 0, 0),
        // ПИД.
        MENU_DESCR(1, 0, "ПИД", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Регулятор скорости", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_SPD_PID_K_P, "Коэффициент П", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_SPD_PID_K_I, "Коэффициент И", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_SPD_PID_K_D, "Коэффициент Д", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, 0, "Регулятор тока якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_ROT_PID_K_P, "Коэффициент П", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_ROT_PID_K_I, "Коэффициент И", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_ROT_PID_K_D, "Коэффициент Д", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, 0, "Регулятор тока возбуждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_EXC_PID_K_P, "Коэффициент П", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_EXC_PID_K_I, "Коэффициент И", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_EXC_PID_K_D, "Коэффициент Д", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Запуск и останов.
    MENU_DESCR(0, 0, "Запуск и останов", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_ROT_STOP_TIME, "Время ожидания останова якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_EXC_STOP_TIME, "Время ожидания останова возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_EXC_START_TIME, "Время ожидания запуска возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_PHASES_CHECK_TIME, "Время ожидания сети", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ZERO_SENSOR_TIME, "Время срабатывания датчиков нуля", NULL, 0, MENU_FLAG_DATA, 0, 0),

        MENU_DESCR(1, 0, "Самозапуск", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_SELFSTART_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_SELFSTART_PERIOD, "Период", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_SELFSTART_DELAY, "Задержка", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_SELFSTART_CLEAR_ERRORS_ENABLED, "Очищать ошибки", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_SELFSTART_CLEAR_ERRORS_ATTEMPTS, "Попытки очистки", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_SELFSTART_CLEAR_ERRORS_PERIOD, "Период очистки", NULL, 0, MENU_FLAG_DATA, 0, 0),


    // Тиристоры.
    MENU_DESCR(0, 0, "Тиристоры", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, 0, "Время открытия", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIACS_PAIRS_OPEN_TIME, "Силовых тиристоров", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_TRIAC_EXC_OPEN_TIME, "Симистора возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, 0, "Углы открытия", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Силовые тиристоры", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_ANGLE_MIN, "Минимальный угол", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_ANGLE_MAX, "Максимальный угол", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, 0, "Симистор возбуждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_ANGLE_MIN, "Минимальный угол", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_ANGLE_MAX, "Максимальный угол", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Защита.
    MENU_DESCR(0, 0, "Защита", NULL, 0, 0, 0, 0),
        // Тепловая защита.
        MENU_DESCR(1, 0, "Тепловая защита", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_THERMAL_OVERLOAD_PROT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_THERMAL_OVERLOAD_PROT_TIME_2I, "Время срабатывания при перегрузе 2х", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_THERMAL_OVERLOAD_PROT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, PARAM_ID_THERMAL_OVERLOAD_PROT_DEAD_ZONE, "Зона нечувствительности", NULL, 0, MENU_FLAG_DATA, 0, 0),
        // Грибок.
        MENU_DESCR(1, 0, "Экстренный останов", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_EMERGENCY_STOP_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Обрыв якоря.
        MENU_DESCR(1, 0, "Обрыв якоря", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_BREAK_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_BREAK_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_BREAK_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_BREAK_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_BREAK_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Обрыв измерения напряжения якоря.
        MENU_DESCR(1, 0, "Обрыв измерения якоря", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_MEASURE_BREAK_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_MEASURE_BREAK_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_MEASURE_BREAK_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_MEASURE_BREAK_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_ROT_MEASURE_BREAK_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Защита фаз.
        MENU_DESCR(1, 0, "Защита фаз", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Состояние фаз", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_PHASES_STATE_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_PHASES_STATE_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_PHASES_STATE_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Углы между фазами", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_FAULT_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_FAULT_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_FAULT_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_WARN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_WARN_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_WARN_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_ANGLES_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Синхронизация с фазами", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_FAULT_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_FAULT_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_FAULT_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_WARN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_WARN_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_WARN_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_PHASES_SYNC_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        MENU_DESCR(1, 0, "Температура радиатора", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_FAULT_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_FAULT_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_WARN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_WARN_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_HEATSINK_TEMP_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Тиристоры.
        MENU_DESCR(1, 0, "Защита тиристоров", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_MIN_ANGLE, "Минимальный угол", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_MIN_CURRENT, "Минимальный ток", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, PARAM_ID_PROT_TRIACS_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Датчики.
        MENU_DESCR(1, 0, "Защита датчиков", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Напряжение сети", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_ADC_RANGE_MIN, "Нижняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_ADC_RANGE_MAX, "Верхняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_EMULATION_ENABLED, "Программное вычисление", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_IN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Ток сети", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_ADC_RANGE_MIN, "Нижняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_ADC_RANGE_MAX, "Верхняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_EMULATION_ENABLED, "Программное вычисление", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_IN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Напряжение якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_ADC_RANGE_MIN, "Нижняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_ADC_RANGE_MAX, "Верхняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_EMULATION_ENABLED, "Программное вычисление", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_U_ROT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Ток якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_ADC_RANGE_MIN, "Нижняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_ADC_RANGE_MAX, "Верхняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_EMULATION_ENABLED, "Программное вычисление", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_ROT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            MENU_DESCR(2, 0, "Ток возбуждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_ENABLED, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_ADC_RANGE_MIN, "Нижняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_ADC_RANGE_MAX, "Верхняя граница значений АЦП", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_EMULATION_ENABLED, "Программное вычисление", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                MENU_DESCR(3, PARAM_ID_PROT_SENSORS_I_EXC_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Защита питания.
        MENU_DESCR(1, 0, "Защита питания", NULL, 0, 0, 0, 0),
            // Напряжение сети.
            MENU_DESCR(2, 0, "Напряжение сети", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Отсечка", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_IN_CUTOFF_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, 0, "Повышение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_OVF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Понижение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_IN_UDF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Ток сети.
            MENU_DESCR(2, 0, "Ток сети", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Отсечка", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_CUTOFF_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, 0, "Повышение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_IN_OVF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Напряжение якоря.
            MENU_DESCR(2, 0, "Напряжение якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Отсечка", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_CUTOFF_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, 0, "Повышение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_U_ROT_OVF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Ток якоря.
            MENU_DESCR(2, 0, "Ток якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Отсечка", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_CUTOFF_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, 0, "Повышение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_ROT_OVF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Ток возбуждения.
            MENU_DESCR(2, 0, "Ток возбуждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Отсечка", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_CUTOFF_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, 0, "Повышение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_OVF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Понижение", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                    MENU_DESCR(4, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                        MENU_DESCR(5, PARAM_ID_PROT_I_EXC_UDF_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
        // Отклонения нулевых значений.
        MENU_DESCR(1, 0, "Отклонения нулевых значений", NULL, 0, 0, 0, 0),
            // Токи сети.
            MENU_DESCR(2, 0, "Ток сети", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_IN_IDLE_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Напряжение якоря.
            MENU_DESCR(2, 0, "Напряжение якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_U_ROT_IDLE_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Ток якоря.
            MENU_DESCR(2, 0, "Ток якоря", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_ROT_IDLE_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
            // Ток возбуждения.
            MENU_DESCR(2, 0, "Ток возбуждения", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, 0, "Уровень ошибки", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_FAULT_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_FAULT_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_FAULT_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_FAULT_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_FAULT_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),
                MENU_DESCR(3, 0, "Уровень предупреждения", NULL, 0, 0, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_WARN_ENABLE, "Включено", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_WARN_LEVEL_VALUE, "Отклонение", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_WARN_LEVEL_TIME_MS, "Время отклонения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_WARN_LATCH_ENABLE, "Защёлка", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
                    MENU_DESCR(4, PARAM_ID_PROT_I_EXC_IDLE_WARN_ACTION, "Действие", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_prot_action),

    // Вычисления.
    MENU_DESCR(0, 0, "Вычисления", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_CALC_PHASE_CURRENT, "Ток фазы", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_phase),
        MENU_DESCR(1, PARAM_ID_CALC_PHASE_VOLTAGE, "Напряжение фазы", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_phase),
        MENU_DESCR(1, PARAM_ID_CALC_ROT_CURRENT, "Ток якоря", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        MENU_DESCR(1, PARAM_ID_CALC_ROT_VOLTAGE, "Напряжение якоря", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        MENU_DESCR(1, PARAM_ID_CALC_EXC_CURRENT, "Ток возбуждения", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),

    // Коммуникация.
    MENU_DESCR(0, 0, "Коммуникация", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_MODBUS_BAUD, "Скорость Modbus", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_MODBUS_ADDRESS, "Адрес Modbus", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Управление вентиляторами.
    MENU_DESCR(0, 0, "Управление вентиляторами", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_CONTROL_ENABLE, "Разрешение управления", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        MENU_DESCR(1, PARAM_ID_FAN_TEMP_MIN, "Температура минимальных оборотов", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_TEMP_MAX, "Температура максимальных оборотов", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_RPM_MIN, "Минимальные обороты вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_ECO_MODE_ENABLE, "Разрешение эко-режима", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        MENU_DESCR(1, PARAM_ID_FAN_ECO_COOLING_TIME, "Время охлаждения в эко-режиме", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_I_NOM, "Номинальный ток вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_I_ZERO_NOISE, "Шум нуля тока вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_PROT_TIME, "Время срабатывания защиты вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_FAN_PROT_OVF_LEVEL, "Уровень предупреждения повышения тока вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Цифровые входа/выхода.
    MENU_DESCR(0, 0, "Цифровые входа/выхода", NULL, 0, 0, 0, 0),
        // Цифровые входа.
        MENU_DESCR(1, 0, "Цифровые входа", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Вход 1", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_1_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_in_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_1_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Вход 2", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_2_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_in_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_2_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Вход 3", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_3_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_in_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_3_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Вход 4", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_4_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_in_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_4_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Вход 5", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_5_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_in_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_IN_5_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
        // Цифровые выхода.
        MENU_DESCR(1, 0, "Цифровые выхода", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, 0, "Выход 1", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_1_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_out_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_1_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Выход 2", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_2_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_out_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_2_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Выход 3", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_3_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_out_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_3_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),
            MENU_DESCR(2, 0, "Выход 4", NULL, 0, 0, 0, 0),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_4_TYPE, "Тип", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_dio_out_type),
                MENU_DESCR(3, PARAM_ID_DIGITAL_OUT_4_INVERSION, "Инверсия", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_bool),

        // Параметры цифровых входов/выходов.
        MENU_DESCR(1, 0, "Параметры цифровых входов/выходов", NULL, 0, 0, 0, 0),
            MENU_DESCR(2, PARAM_ID_DIGITAL_IO_DEADTIME_MS, "Время игнорирования изменения состояния входа", NULL, 0, MENU_FLAG_DATA, 0, 0),

    // Настройки интерфейса
    MENU_DESCR(0, 0, "Графический интерфейс", NULL, 0, MENU_FLAG_ROOT, 0, 0),
        // Пароль администратора
        MENU_DESCR(1, PARAM_ID_GUI_PASSWORD_ADMIN, "Пароль администратора", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, 0),
        // Пароль ROOT
        MENU_DESCR(1, PARAM_ID_GUI_PASSWORD_ROOT, "Пароль root", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ROOT, 0, 0),
        // Звуковые оповещения
        MENU_DESCR(1, PARAM_ID_GUI_BUZZER, "Звук", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, &menu_val_gui_buzzer),
        // Язык
        MENU_DESCR(1, PARAM_ID_GUI_LANGUAGE, "Язык", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_gui_languages),
        // Плитки
        MENU_DESCR(1, 0, "Плитки", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, 0),
            MENU_DESCR(2, PARAM_ID_GUI_TILE_1, "Плитка 1", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, &menu_val_gui_tiles),
            MENU_DESCR(2, PARAM_ID_GUI_TILE_2, "Плитка 2", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, &menu_val_gui_tiles),
            MENU_DESCR(2, PARAM_ID_GUI_TILE_3, "Плитка 3", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, &menu_val_gui_tiles),
            MENU_DESCR(2, PARAM_ID_GUI_TILE_4, "Плитка 4", NULL, 0, MENU_FLAG_DATA | MENU_FLAG_ADMIN, 0, &menu_val_gui_tiles),
        // Граничные значения цветовых предупреждений на плитках
        MENU_DESCR(1, 0, "Значения на плитках", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Напряжение фазы
            MENU_DESCR(2, 0, "Напряжение сети", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Напряжение якоря
            MENU_DESCR(2, 0, "Напряжение якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ROT_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ROT_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ROT_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_U_ROT_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Ток фазы
            MENU_DESCR(2, 0, "Ток сети", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Ток якоря
            MENU_DESCR(2, 0, "Ток якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ROT_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ROT_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ROT_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_ROT_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Ток возбуждения
            MENU_DESCR(2, 0, "Ток возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_EXC_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_EXC_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_EXC_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_EXC_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Температура радиатора
            MENU_DESCR(2, 0, "Температура радиатора", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_TEMP_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_TEMP_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_TEMP_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_TEMP_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Ток вентилятора.
            MENU_DESCR(2, 0, "Ток вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_FAN_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_FAN_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_FAN_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_FAN_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Ток 4-20.
            MENU_DESCR(2, 0, "Ток 4-20 мА", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_REF_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_REF_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_REF_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_POWER_I_REF_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Обороты вентилятора радиатора.
            MENU_DESCR(2, 0, "Обороты вентилятора", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_FAN_RPM_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_FAN_RPM_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_FAN_RPM_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_HEATSINK_FAN_RPM_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Угол открытия тиристоров.
            MENU_DESCR(2, 0, "Угол открытия тиристоров", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIACS_PAIRS_OPEN_ANGLE_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Угол открытия возбуждения.
            MENU_DESCR(2, 0, "Угол открытия симистора возбуждения", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_OPEN_ANGLE_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_OPEN_ANGLE_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_OPEN_ANGLE_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_TRIAC_EXC_OPEN_ANGLE_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            /////////// Мотор ////////////
            // Вычисленный коэффициент полезного действия.
            MENU_DESCR(2, 0, "КПД двигателя", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_EFF_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_EFF_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_EFF_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_EFF_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Вычисленное сопротивление якоря.
            MENU_DESCR(2, 0, "Сопротивление якоря", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_R_ROT_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_R_ROT_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_R_ROT_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_R_ROT_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Вычисленные обороты.
            MENU_DESCR(2, 0, "Обороты двигателя", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_RPM_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_RPM_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_RPM_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_RPM_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
            // Вычисленный момент.
            MENU_DESCR(2, 0, "Момент двигателя", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_TORQUE_ALARM_MIN, "Уровень ошибки по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_TORQUE_WARN_MIN, "Уровень предупреждения по понижению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_TORQUE_WARN_MAX, "Уровень предупреждения по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
                MENU_DESCR(3, PARAM_ID_MOTOR_TORQUE_ALARM_MAX, "Уровень ошибки по превышению", NULL, 0, MENU_FLAG_DATA, 0, 0),
        // Отображение текста ошибок и предупреждений на плитке
        MENU_DESCR(1, PARAM_ID_MENU_GUI_TILE_WARNINGS, "Текст события на плитке", NULL, 0, MENU_FLAG_DATA, 0, &menu_val_gui_tiles_for_warnings),

    // Коэффициенты АЦП.
    MENU_DESCR(0, 0, "Коэффициенты АЦП", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ua, "Коэффициент Ua", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ub, "Коэффициент Ub", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Uc, "Коэффициент Uc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Urot, "Коэффициент Urot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ia, "Коэффициент Ia", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ib, "Коэффициент Ib", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ic, "Коэффициент Ic", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Irot, "Коэффициент Irot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Iexc, "Коэффициент Iexc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Iref, "Коэффициент Iref", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_VALUE_MULTIPLIER_Ifan, "Коэффициент Ifan", NULL, 0, MENU_FLAG_DATA, 0, 0),
    // Калибровочные данные.
    MENU_DESCR(0, 0, "Калибровочные данные", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ua, "Значение калибровки Ua", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ub, "Значение калибровки Ub", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Uc, "Значение калибровки Uc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Urot, "Значение калибровки Urot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ia, "Значение калибровки Ia", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ib, "Значение калибровки Ib", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ic, "Значение калибровки Ic", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Irot, "Значение калибровки Irot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Iexc, "Значение калибровки Iexc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Iref, "Значение калибровки Iref", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_ADC_CALIBRATION_DATA_Ifan, "Значение калибровки Ifan", NULL, 0, MENU_FLAG_DATA, 0, 0),
    // Множители значений.
    MENU_DESCR(0, 0, "Множители значений", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ua, "Множитель Ua", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ub, "Множитель Ub", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Uc, "Множитель Uc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Urot, "Множитель Urot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ia, "Множитель Ia", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ib, "Множитель Ib", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ic, "Множитель Ic", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Irot, "Множитель Irot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Iexc, "Множитель Iexc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Iref, "Множитель Iref", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Ifan, "Множитель Ifan", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_VALUE_MULTIPLIER_Erot, "Множитель Erot", NULL, 0, MENU_FLAG_DATA, 0, 0),
    // Усреднение значений.
    MENU_DESCR(0, 0, "Время усреднения значений", NULL, 0, 0, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ua, "Время усреднения Ua", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ub, "Время усреднения Ub", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Uc, "Время усреднения Uc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Urot, "Время усреднения Urot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ia, "Время усреднения Ia", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ib, "Время усреднения Ib", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ic, "Время усреднения Ic", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Irot, "Время усреднения Irot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Iexc, "Время усреднения Iexc", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Iref, "Время усреднения Iref", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Ifan, "Время усреднения Ifan", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_Erot, "Время усреднения Erot", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_RPM, "Время усреднения оборотов", NULL, 0, MENU_FLAG_DATA, 0, 0),
        MENU_DESCR(1, PARAM_ID_AVERAGING_TIME_TORQUE, "Время усреднения момента", NULL, 0, MENU_FLAG_DATA, 0, 0),
    //MENU_DESCR(0, 0, "", NULL, 0, 0, 0, 0)
};

#endif // PARAMETERS_MENU_H
