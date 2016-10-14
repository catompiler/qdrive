/**
 * @file parameters_list.h Список параметров.
 */

#ifndef PARAMETERS_LIST_H
#define	PARAMETERS_LIST_H

#include "parameters_ids.h"

//! Макрос для сокращения декларации дробного значения fixed32.
#define F32(A, B) fixed32_make_from_fract(A, B)
//! Макрос для сокращения декларации целого значения fixed32.
#define F32I(I) fixed32_make_from_int(I)


// Число реальных параметров.
#define PARAMETERS_REAL_COUNT 64
// Число виртуальных параметров.
#define PARAMETERS_VIRT_COUNT 19
// Общее число параметров.
#define PARAMETERS_COUNT (PARAMETERS_REAL_COUNT + PARAMETERS_VIRT_COUNT)

// Дескрипторы параметров.
PARAM_DESCRS(parameters_descrs, PARAMETERS_COUNT) {
    PARAM_DESCR(PARAM_ID_U_NOM,           PARAM_TYPE_UINT,      0,       1000,      220,        0),
    PARAM_DESCR(PARAM_ID_U_NOM_ALLOW_VAR, PARAM_TYPE_UINT,      0,       10,        5,          0), // %
    PARAM_DESCR(PARAM_ID_U_NOM_CRIT_VAR,  PARAM_TYPE_UINT,      0,       20,        10,         0), // %
    PARAM_DESCR(PARAM_ID_I_IN_CUTOFF,     PARAM_TYPE_UINT,      0,       1000,      200,        0),
    PARAM_DESCR(PARAM_ID_U_ZERO_NOISE,    PARAM_TYPE_FRACT_100, F32I(0), F32I(10),  F32I(10),   0),
    PARAM_DESCR(PARAM_ID_I_ZERO_NOISE,    PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(1, 10), 0),
    PARAM_DESCR(PARAM_ID_I_ROT_ZERO_NOISE,PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32I(5),    0),
    PARAM_DESCR(PARAM_ID_I_EXC_ZERO_NOISE,PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(5,10),  0),
    PARAM_DESCR(PARAM_ID_U_ROT_NOM,       PARAM_TYPE_UINT,      0,       600,       400,        0),
    PARAM_DESCR(PARAM_ID_U_ROT_ALLOW_VAR, PARAM_TYPE_UINT,      0,       10,        2,          0), // %
    PARAM_DESCR(PARAM_ID_U_ROT_CRIT_VAR,  PARAM_TYPE_UINT,      0,       20,        5,          0), // %
    PARAM_DESCR(PARAM_ID_I_ROT_NOM,       PARAM_TYPE_FRACT_10,  F32I(0), F32I(200), F32I(10),   0),
    PARAM_DESCR(PARAM_ID_I_ROT_CUTOFF_MULT, PARAM_TYPE_UINT,    0,       100,       8,          0),
    PARAM_DESCR(PARAM_ID_I_ROT_ALLOW_VAR, PARAM_TYPE_UINT,      0,       100,       5,          0), // %
    PARAM_DESCR(PARAM_ID_I_ROT_CRIT_VAR,  PARAM_TYPE_UINT,      0,       100,       10,         0), // %
    PARAM_DESCR(PARAM_ID_I_EXC,           PARAM_TYPE_FRACT_100, F32I(0), F32I(20),  F32(65,100),    0), //F32I(8)
    PARAM_DESCR(PARAM_ID_I_EXC_ALLOW_VAR, PARAM_TYPE_UINT,      0,       100,       10,         0), // %
    PARAM_DESCR(PARAM_ID_I_EXC_CRIT_VAR,  PARAM_TYPE_UINT,      0,       100,       20,         0), // %
    
    PARAM_DESCR(PARAM_ID_EXC_PHASE,       PARAM_TYPE_UINT,      PHASE_A, PHASE_C,   PHASE_B,      0),
    PARAM_DESCR(PARAM_ID_EXC_MODE,        PARAM_TYPE_UINT, DRIVE_TRIACS_EXC_FIXED, DRIVE_TRIACS_EXC_REGULATED, DRIVE_TRIACS_EXC_FIXED, 0),
    
    PARAM_DESCR(PARAM_ID_RAMP_TIME,       PARAM_TYPE_UINT, RAMP_TIME_MIN, RAMP_TIME_MAX, RAMP_TIME_DEFAULT, 0),
    
    PARAM_DESCR(PARAM_ID_ROT_PID_K_P,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(10, 100),   0), // Kp.
    PARAM_DESCR(PARAM_ID_ROT_PID_K_I,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(50, 100),   0), // Ki.
    PARAM_DESCR(PARAM_ID_ROT_PID_K_D,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32I(0),      0), // Kd.
    PARAM_DESCR(PARAM_ID_EXC_PID_K_P,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(10, 1),   0), // Kp.
    PARAM_DESCR(PARAM_ID_EXC_PID_K_I,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32(10, 1),   0), // Ki.
    PARAM_DESCR(PARAM_ID_EXC_PID_K_D,     PARAM_TYPE_FRACT_100, F32I(0), F32I(100), F32I(0),      0), // Kd.
    
    PARAM_DESCR(PARAM_ID_ROT_STOP_TIME,   PARAM_TYPE_UINT,      0,       100,       10,           0),
    PARAM_DESCR(PARAM_ID_EXC_STOP_TIME,   PARAM_TYPE_UINT,      0,       100,       5,            0),
    PARAM_DESCR(PARAM_ID_EXC_START_TIME,  PARAM_TYPE_UINT,      0,       100,       5,            0),
    
    PARAM_DESCR(PARAM_ID_TRIACS_PAIRS_OPEN_TIME,   PARAM_TYPE_UINT,      0,         1000,         100,     0),
    PARAM_DESCR(PARAM_ID_TRIAC_EXC_OPEN_TIME,      PARAM_TYPE_UINT,      0,         1000,         200,     0),
    
    PARAM_DESCR(PARAM_ID_THERMAL_OVERLOAD_PROT_TIME_6I, PARAM_TYPE_FRACT_100, 0, F32I(100), F32I(5), 0),
            
    PARAM_DESCR(PARAM_ID_CALC_PHASE_CURRENT,     PARAM_TYPE_UINT,  0,    3,    0,   0),
    
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ua,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ub,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Uc,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Urot,  PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ia,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ib,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ic,    PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Irot,  PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Iexc,  PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Iref,  PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    PARAM_DESCR(PARAM_ID_CALIBRATION_DATA_Ifan,  PARAM_TYPE_UINT,  0,    4096,     2048,    0),
    
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_1_TYPE,      PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_1_INVERSION, PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_2_TYPE,      PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_2_INVERSION, PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_3_TYPE,      PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_3_INVERSION, PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_4_TYPE,      PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_4_INVERSION, PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_5_TYPE,      PARAM_TYPE_UINT,  0,    5,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_IN_5_INVERSION, PARAM_TYPE_UINT,  0,    5,    0,   0),
    
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_1_TYPE,      PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_1_INVERSION, PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_2_TYPE,      PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_2_INVERSION, PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_3_TYPE,      PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_3_INVERSION, PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_4_TYPE,      PARAM_TYPE_UINT,  0,    4,    0,   0),
    PARAM_DESCR(PARAM_ID_DIGITAL_OUT_4_INVERSION, PARAM_TYPE_UINT,  0,    4,    0,   0),
    
    PARAM_DESCR(PARAM_ID_MODBUS_BAUD,     PARAM_TYPE_UINT,      1200,    57600,     9600,         0),
    
    // Отладка.
    PARAM_DESCR(PARAM_ID_DEBUG_0, PARAM_TYPE_INT,        0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_1, PARAM_TYPE_INT,        0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_2, PARAM_TYPE_UINT,       0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_3, PARAM_TYPE_UINT,       0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_4, PARAM_TYPE_FRACT_10,   0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_5, PARAM_TYPE_FRACT_10,   0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_6, PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_7, PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_8, PARAM_TYPE_FRACT_1000, 0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_DEBUG_9, PARAM_TYPE_FRACT_1000, 0, 0, 0, PARAM_FLAG_VIRTUAL),
    
    // Питание.
    PARAM_DESCR(PARAM_ID_POWER_U_A,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_U_B,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_U_C,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_U_ROT, PARAM_TYPE_FRACT_10,   0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_I_A,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_I_B,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_I_C,   PARAM_TYPE_FRACT_100,  0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_I_ROT, PARAM_TYPE_FRACT_10,   0, 0, 0, PARAM_FLAG_VIRTUAL),
    PARAM_DESCR(PARAM_ID_POWER_I_EXC, PARAM_TYPE_FRACT_1000, 0, 0, 0, PARAM_FLAG_VIRTUAL),
};

#undef F32I
#undef F32

#endif	/* PARAMETERS_LIST_H */
