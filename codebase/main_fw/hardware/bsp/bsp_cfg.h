/**
 * @file      bsp_cfg.h
 *
 * @brief     Board config support package
 *
 * @date      2023-01-20
 *
 */

/** \addtogroup bsp
 *  @{
 */

#ifndef __BSP_CFG_H_
#define __BSP_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************
     * INCLUDES
     ******************************************************************************/

#include <stdint.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/
//#define DBG
#define PCLK2          (72000000UL)
#define PCLK1          (36000000UL) // PCLK1 can max freq 36 MHz ref.manual page 115
#define PrescalerTim1  (1U) // Fclk div
#define PrescalerTim2  (2U)
#define PrescalerTim6  (3600U)
#define PrescalerTim16 (6U)
#define PrescalerTim16_SBUS
#define Fpwm                    (48000U) // 36000                         // Set PWM freq in Hz
#define Autoreload_Tim1         (PCLK2) / (PrescalerTim1 * Fpwm)
#define GAS_5                   (Autoreload_Tim1 * 5) / 100
#define Autoreload_Tim2         (0xffffffffUL)
#define Autoreload_Tim6         (0xffffU)
#define Autoreload_Tim16        (0xea60U)
#define Spi_message_value_max   (0xffffU)
#define AWD_HighThreshold       (39U) // 2 ampers for 12bit resolution ADC
#define AWD_LowThreshold        (0)
#define OperegenieDefine        (0x360U)
#define SIZE_BUFF_SPI_DMA       (3U)
#define SIZE_BUFF_USART3_RX_DMA (100U)
#define MS_1                    (20U)
#define DELTA_GAS               (90U) // 4,5%; max throttle gain 4.5% at 10 Hz for soft start
#define K_Rdiv                  (0.1176) // coeff for div power supply

#define T_MAX_IMPULS_FRSKY (2000U) // us
#define T_MID_IMPULS_FRSKY (1500U) // us
#define T_MIN_IMPULS_FRSKY (1000U) // us
#define IMPULS_MAX_FRSKY   (((PCLK2 / 1000000) / PrescalerTim16) * T_MAX_IMPULS_FRSKY)
#define IMPULS_MID_FRSKY   (((PCLK2 / 1000000) / PrescalerTim16) * T_MID_IMPULS_FRSKY)
#define IMPULS_MIN_FRSKY   (((PCLK2 / 1000000) / PrescalerTim16) * T_MIN_IMPULS_FRSKY)
//#define USE_FRSKY
#define USE_SBUS
#define SBUS_message_GASvalue_max 0xffff

// ADC_1
#define phasa_A           ((uint16_t *)0x50000080) // ADC_JDR1   phasa_A PA0   ADC_Channel_1
#define phasa_B           ((uint16_t *)0x50000084) // ADC_JDR2   phasa_B PA1   ADC_Channel_2
#define phasa_C           ((uint16_t *)0x50000088) // ADC_JDR3   phasa_C PA3   ADC_Channel_4
#define Threshold_Vp_div2 ((uint16_t *)0x5000008C) // ADC_JDR4   Threshold PA4 ADC_Channel_5


#define CNT_START 650

#define QTY_INJECTED_CHANNELS_4     (3U)
#define ADC_INJECTED_CHANNEL_RANK_1 (1U)
#define ADC_INJECTED_CHANNEL_RANK_2 (2U)
#define ADC_INJECTED_CHANNEL_RANK_3 (3U)
#define ADC_INJECTED_CHANNEL_RANK_4 (4U)
#define ADC_REGULAR_CHANNEL_RANK_1  (1U)
#define ADC_REGULAR_CHANNEL_RANK_2  (2U)

#define QTY_REGULAR_CHANNEL         (2U)
#define BSP_PHA_FEEDBACK_ADC_CH     (ADC_Channel_0)
#define BSP_PHB_FEEDBACK_ADC_CH     (ADC_Channel_1)
#define BSP_PHC_FEEDBACK_ADC_CH     (ADC_Channel_2)
#define BSP_THRESHOLD_ADC_CH        (ADC_Channel_3)
#define BSP_POWER_DIV2_ADC_CH       (ADC_Channel_4)
#define BSP_THROTTLE_CONTROL_ADC_CH (ADC_Channel_5)

#define BSP_DMA_QTY_DATA            (QTY_REGULAR_CHANNEL)


#define BSP_ADC_REG_DMA_MAP_CH0         (BSP_POWER_DIV2_ADC_CH)
#define BSP_ADC_REG_DMA_MAP_CH1         (BSP_THROTTLE_CONTROL_ADC_CH)

#define BSP_THROTTLE_RAW_MAX            (1023U)

    /******************************************************************************
     * PUBLIC TYPES
     ******************************************************************************/
    

    /******************************************************************************
     * INLINE FUNCTIONS
     ******************************************************************************/

    /******************************************************************************
     * PUBLIC FUNCTION PROTOTYPES
     ******************************************************************************/

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __BSP_CFG_H_

/** @}*/
