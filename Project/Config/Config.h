#pragma once 
#include "main.h"


//*--------------------引入需要的板级支持包----------------------*//
#include "adc_bsp.h"
// #include "fdcan_bsp.h"
#include "flash_bsp.h"
#include "tim_bsp.h"
#include "uart_bsp.h"
#include "math_bsp.h"
// #include "usb_bsp.h"
//*--------------------引入需要的模组工具----------------------*//
#include "DwtClock.h"
#include "Eeprom.h"
#include "HRpwm.h"
#include "Pid.h"
#include "Sampler.h"
#include "TimedInterrupt.h"
//*++++++++++++++++++++++++++电压变换Transformation++++++++++++++++++++++++++*//
#include "Control.h"
#include "Channel.h"

extern AdcBsp_t bsp_adc1;
#define BSP_ADC1_INIT (AdcBsp_t){                                   \
    .hadc = &hadc1,                                                 \
    .ch_cnt = 1,                                                    \
    .filt_rate = 4                                                  \
}

extern TimedInterrupt_t controlStream;
#define CONTROL_STREAM_INIT (TimedInterrupt_t){                     \
    .htim = &htim7,                                                 \
    .intPeriod = 20,                                                \
    .mode = TimedInterrupt_Mode_Independent,                   	    \
}

#define CONTROL_CHANNELS_NUM 1
extern Channel_t channels[];

#define CONTROL_CHANNEL0_INIT (Channel_t) {                         \
   .transform = {                                                   \
        .buck = {                                                   \
            .hhrtim = &hhrtim,                                      \
            .isInversed = 0,                                        \
            .dutyLimit = 0.90f,                                     \
            .timerIdentifier = HRTIM_TIMERID_TIMER_A,               \
            .defaultDuty = 0.5f,                                    \
        },                                                          \
        .boost = {                                                  \
            .hhrtim = &hhrtim,                                      \
            .isInversed = 0,                                        \
            .dutyLimit = 0.90f,                                     \
            .timerIdentifier = HRTIM_TIMERID_TIMER_B,               \
            .defaultDuty = 0.5f,                                    \
        }                                                           \
    },                                                              \
    .sample = {                                                     \
        .output_voltage = {                                         \
            .analog_arr = {{.hadc = &bsp_adc1, .noc = 1}},          \
            .analog_cnt = 1,                                        \
            .map_k = 0.000755321f,                                  \
            .map_b = 0.0f                                           \
        },                                                          \
        .source_voltage = {                                         \
            .analog_arr = {{.hadc = &bsp_adc1, .noc = 1}},          \
            .analog_cnt = 1,                                        \
            .map_k = 0.0f,                                          \
            .map_b = 24.0f                                          \
        },                                                          \
    },                                                              \
    .pid = {                                                        \
		.target = 36.0f,                                            \
        .kp = 0,            .ki = 0.05,            .kd = 0,     	\
        .integral_startzone = 80.0f,   .integral_deadband = 0,      \
        .integral_max     	= 60.0f, .integral_min      = -60.0,	\
        .output_max         = 80.0f, .output_min        = -80.0 	\
    },                                                              \
}

//*++++++++++++++++++++++++++上位机通信UpperComputer++++++++++++++++++++++++++*//
#include "UpperComputer.h"
#define UPPERCOMPUTER_ISACTIVE 1
extern UpperComputer_t upperComputer;

#define UPPERCOMPUTER_INIT (UpperComputer_t){                       \
    .uartstream = (UartBsp_t) {                                     \
        .huart = &huart1,                                           \
        .frameHead = 0x5A5A,                                        \
        .frameTail = 0xA5A5                                         \
    },                                                              \
    .flashmemory = (FlashBsp_t) {                                   \
        .sector = FLASH_SECTOR_3,                                   \
		.sram_buf = {0}												\
    }                                                               \
}



