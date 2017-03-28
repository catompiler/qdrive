/**
 * @file translation_ru.h Файл перевода на русский язык.
 */

#ifndef TRANSLATION_RU_H
#define TRANSLATION_RU_H

#include "localization/localization.h"
#include "translations_ids.h"

/**
 * Русский перевод.
 */
TEXT_TRS_BEGIN(text_trs_ru)
TEXT_TR(TR_ID_UNITS_V, "В")
TEXT_TR(TR_ID_UNITS_A, "А")
TEXT_TR(TR_ID_UNITS_S, "с")
TEXT_TR(TR_ID_UNITS_MS, "мс")
TEXT_TR(TR_ID_UNITS_US, "мкс")
TEXT_TR(TR_ID_UNITS_PERCENT, "%")
TEXT_TR(TR_ID_UNITS_GRAD, "°")
TEXT_TR(TR_ID_ENUM_PHASE_UNK, "Не задано")
TEXT_TR(TR_ID_ENUM_PHASE_A, "Фаза A")
TEXT_TR(TR_ID_ENUM_PHASE_B, "Фаза B")
TEXT_TR(TR_ID_ENUM_PHASE_C, "Фаза C")
TEXT_TR(TR_ID_ENUM_NO, "Нет")
TEXT_TR(TR_ID_ENUM_YES, "Да")
TEXT_TR(TR_ID_ENUM_EXC_MODE_FIXED, "Фиксированное")
TEXT_TR(TR_ID_ENUM_EXC_MODE_REGULATED, "Регулируемое")
TEXT_TR(TR_ID_ENUM_STOP_MODE_NORMAL, "Нормальный")
TEXT_TR(TR_ID_ENUM_STOP_MODE_FAST, "Быстрый")
TEXT_TR(TR_ID_ENUM_STOP_MODE_COAST, "Выбег")
TEXT_TR(TR_ID_ENUM_PROT_ACTION_IGNORE, "Игнорировать")
TEXT_TR(TR_ID_ENUM_PROT_ACTION_WARNING, "Предупреждение")
TEXT_TR(TR_ID_ENUM_PROT_ACTION_FAST_STOP, "Быстрый останов")
TEXT_TR(TR_ID_ENUM_PROT_ACTION_COAST_STOP, "Останов выбегом")
TEXT_TR(TR_ID_ENUM_PROT_ACTION_EMERGENCY_STOP, "Экстренный останов")
TEXT_TR(TR_ID_ENUM_DIO_IN_NONE, "Не подключен")
TEXT_TR(TR_ID_ENUM_DIO_IN_START_STOP, "Запуск/Останов")
TEXT_TR(TR_ID_ENUM_DIO_IN_EMERGENCY_STOP, "Экстренный останов")
TEXT_TR(TR_ID_ENUM_DIO_IN_REFERENCE_INC, "Инкремент задания")
TEXT_TR(TR_ID_ENUM_DIO_IN_REFERENCE_DEC, "Декремент задания")
TEXT_TR(TR_ID_ENUM_DIO_IN_CLEAR_ERRORS, "Сброс ошибок")
TEXT_TR(TR_ID_ENUM_DIO_IN_USER, "Пользовательский")
TEXT_TR(TR_ID_ENUM_DIO_OUT_NONE, "Не подключен")
TEXT_TR(TR_ID_ENUM_DIO_OUT_OK, "Привод в порядке")
TEXT_TR(TR_ID_ENUM_DIO_OUT_READY, "Готовность")
TEXT_TR(TR_ID_ENUM_DIO_OUT_RUNNING, "Работа")
TEXT_TR(TR_ID_ENUM_DIO_OUT_ERROR, "Ошибка")
TEXT_TR(TR_ID_ENUM_DIO_OUT_WARNING, "Предупреждение")
TEXT_TR(TR_ID_ENUM_DIO_OUT_USER, "Пользовательский")
TEXT_TR(TR_ID_MENU_POWER, "Питание")
TEXT_TR(TR_ID_MENU_POWER_IN, "Сеть")
TEXT_TR(TR_ID_MENU_U_NOM, "Ном. U сети")
TEXT_TR(TR_ID_MENU_I_NOM, "Ном. I сети")
TEXT_TR(TR_ID_MENU_ROTOR, "Якорь")
TEXT_TR(TR_ID_MENU_U_ROT_NOM, "Ном. U якоря")
TEXT_TR(TR_ID_MENU_I_ROT_NOM, "Ном. I якоря")
TEXT_TR(TR_ID_MENU_EXCITATION, "Возбуждение")
TEXT_TR(TR_ID_MENU_I_EXC, "I возбуждения")
TEXT_TR(TR_ID_MENU_EXC_PHASE, "Фаза возбуждения")
TEXT_TR(TR_ID_MENU_EXC_MODE, "Режим возбуждения")
TEXT_TR(TR_ID_MENU_RAMP, "Разгон и торможение")
TEXT_TR(TR_ID_MENU_RAMP_START_TIME, "Время разгона 0-100%")
TEXT_TR(TR_ID_MENU_RAMP_STOP_TIME, "Время торможения 100-0%")
TEXT_TR(TR_ID_MENU_RAMP_FAST_STOP_TIME, "Время быстрого торможения 100-0%")
TEXT_TR(TR_ID_MENU_RAMP_REFERENCE_TIME, "Время изменения задания 0-100%")
TEXT_TR(TR_ID_MENU_RAMP_STOP_MODE, "Режим останова")
TEXT_TR(TR_ID_MENU_PID, "ПИД регулятор")
TEXT_TR(TR_ID_MENU_PID_ROT, "ПИД якоря")
TEXT_TR(TR_ID_MENU_PID_EXC, "ПИД возбуждения")
TEXT_TR(TR_ID_MENU_PID_K_P, "Коэффициент П")
TEXT_TR(TR_ID_MENU_PID_K_I, "Коэффициент И")
TEXT_TR(TR_ID_MENU_PID_K_D, "Коэффициент Д")
TEXT_TR(TR_ID_MENU_START_STOP, "Запуск и останов")
TEXT_TR(TR_ID_MENU_ROT_STOP_TIME, "Время останова якоря")
TEXT_TR(TR_ID_MENU_EXC_STOP_TIME, "Время останова возбуждения")
TEXT_TR(TR_ID_MENU_EXC_START_TIME, "Время запуска возбуждения")
TEXT_TR(TR_ID_MENU_PHASES_CHECK_TIME, "Время ожидания фаз")
TEXT_TR(TR_ID_MENU_ZERO_SENSOR_TIME, "Время срабатывания датчиков нуля")
TEXT_TR(TR_ID_MENU_TRIACS, "Тиристоры")
TEXT_TR(TR_ID_MENU_TRIACS_OPEN_TIME, "Время открытия")
TEXT_TR(TR_ID_MENU_TRIACS_PAIRS_OPEN_TIME, "Силовые")
TEXT_TR(TR_ID_MENU_TRIAC_EXC_OPEN_TIME, "Возбуждения")
TEXT_TR(TR_ID_MENU_PROTECTION, "Защита")
TEXT_TR(TR_ID_MENU_CUTOFF, "Отсечка")
TEXT_TR(TR_ID_MENU_PROT_LEVEL, "Отклонение")
TEXT_TR(TR_ID_MENU_PROT_ENABLE, "Включено")
TEXT_TR(TR_ID_MENU_PROT_LEVEL_TIME_MS, "Время отклонения")
TEXT_TR(TR_ID_MENU_PROT_LATCH_ENABLE, "Защёлка")
TEXT_TR(TR_ID_MENU_PROT_ACTION, "Действие")
TEXT_TR(TR_ID_MENU_PROT_OVERFLOW, "Повышение")
TEXT_TR(TR_ID_MENU_PROT_UNDERFLOW, "Понижение")
TEXT_TR(TR_ID_MENU_PROT_ERROR_LEVEL, "Уровень ошибки")
TEXT_TR(TR_ID_MENU_PROT_WARNING_LEVEL, "Уровень предупреждения")
TEXT_TR(TR_ID_MENU_THERMAL_OVERLOAD_PROT, "Тепловая защита")
TEXT_TR(TR_ID_MENU_THERMAL_OVERLOAD_PROT_TIME_6I, "Время работы при перегрузе 6x")
TEXT_TR(TR_ID_MENU_EMERGENCY_STOP, "Экстренный останов")
TEXT_TR(TR_ID_MENU_POWER_VOLTAGE, "Напряжение сети")
TEXT_TR(TR_ID_MENU_POWER_CURRENT, "Ток сети")
TEXT_TR(TR_ID_MENU_ROTOR_VOLTAGE, "Напряжение якоря")
TEXT_TR(TR_ID_MENU_ROTOR_CURRENT, "Ток якоря")
TEXT_TR(TR_ID_MENU_EXC_CURRENT, "Ток возбуждения")
TEXT_TR(TR_ID_MENU_ZERO_DRIFT, "Отклонения нулей")
TEXT_TR(TR_ID_MENU_CALCULATION, "Вычисления")
TEXT_TR(TR_ID_MENU_CALC_PHASE_CURRENT, "Вычислять ток для фазы")
TEXT_TR(TR_ID_MENU_COMMUNICATION, "Коммуникация")
TEXT_TR(TR_ID_MENU_MODBUS_BAUD, "Скорость Modbus RTU")
TEXT_TR(TR_ID_MENU_DIGITAL_INPUTS, "Цифровые входа")
TEXT_TR(TR_ID_MENU_DIGITAL_OUTPUTS, "Цифровые выхода")
TEXT_TR(TR_ID_MENU_DIO_TYPE, "Тип")
TEXT_TR(TR_ID_MENU_DIO_INVERSION, "Инверсия")
TEXT_TR(TR_ID_MENU_DIGITAL_IO, "Цифровые входа/выхода")
TEXT_TR(TR_ID_MENU_DIGITAL_IO_SETTINGS, "Параметры цифровых входов/выходов")
TEXT_TR(TR_ID_MENU_DIGITAL_IO_DEADTIME_MS, "Время игнорирования изменения состояния входа")
TEXT_TR(TR_ID_MENU_DIO_INPUT1, "Вход 1")
TEXT_TR(TR_ID_MENU_DIO_INPUT2, "Вход 2")
TEXT_TR(TR_ID_MENU_DIO_INPUT3, "Вход 3")
TEXT_TR(TR_ID_MENU_DIO_INPUT4, "Вход 4")
TEXT_TR(TR_ID_MENU_DIO_INPUT5, "Вход 5")
TEXT_TR(TR_ID_MENU_DIO_OUTPUT1, "Выход 1")
TEXT_TR(TR_ID_MENU_DIO_OUTPUT2, "Выход 2")
TEXT_TR(TR_ID_MENU_DIO_OUTPUT3, "Выход 3")
TEXT_TR(TR_ID_MENU_DIO_OUTPUT4, "Выход 4")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULTS, "Коэффициенты АЦП")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ua, "Коэффициент Ua")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ub, "Коэффициент Ub")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Uc, "Коэффициент Uc")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Urot, "Коэффициент Urot")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ia, "Коэффициент Ia")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ib, "Коэффициент Ib")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ic, "Коэффициент Ic")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Irot, "Коэффициент Irot")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Iexc, "Коэффициент Iexc")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Iref, "Коэффициент Iref")
TEXT_TR(TR_ID_MENU_ADC_VALUE_MULT_Ifan, "Коэффициент Ifan")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA, "Калибровочные данные")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ua, "Значение калибровки Ua")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ub, "Значение калибровки Ub")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Uc, "Значение калибровки Uc")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Urot, "Значение калибровки Urot")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ia, "Значение калибровки Ia")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ib, "Значение калибровки Ib")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ic, "Значение калибровки Ic")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Irot, "Значение калибровки Irot")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Iexc, "Значение калибровки Iexc")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Iref, "Значение калибровки Iref")
TEXT_TR(TR_ID_MENU_CALIBRATION_DATA_Ifan, "Значение калибровки Ifan")
TEXT_TR(TR_ID_MENU_PHASE_SYNC, "Синхронизация с фазами")
TEXT_TR(TR_ID_MENU_PHASE_SYNC_ACCURACY, "Точность синхронизации")
TEXT_TR(TR_ID_MENU_PHASE_SYNC_PID, "ПИД-регулятор ФАПЧ")
//TEXT_TR(TR_ID_MENU_, "")
//TEXT_TR(TR_ID_ENUM_, "")
//TEXT_TR(TR_ID_, "")
TEXT_TRS_END()


#endif /* TRANSLATION_RU_H */
