/**
 * @file      bsp.c
 *
 * @brief     Board support package
 *
 * @date      2023-02-27
 *
 */

/** \addtogroup bsp
 *  @{
 */

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "bsp.h"
#include "stm32f10x.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 ******************************************************************************/
static uint16_t array_adc_reg_channel[QTY_REGULAR_CHANNEL];
static const uint8_t array_adc_map_dma_reg_channel[QTY_REGULAR_CHANNEL] = 
{
    BSP_ADC_REG_DMA_MAP_CH0,
    BSP_ADC_REG_DMA_MAP_CH1
};

/******************************************************************************
 * PUBLIC DATA
 ******************************************************************************/

/******************************************************************************
 * EXTERNAL DATA
 ******************************************************************************/

/******************************************************************************
 * EXTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

/**
 * @brief Init board support package
 * HCLK = 56 MHz
 * PCLK1 = 28 MHz
 * PCLK2 = 56 MHz
 * ADCCLK = 14 MHz
 * @return none
 */
void bsp_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    // X_Low side
    GPIO_InitTypeDef GPIO_InitStructre;
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructre);

    // X-High Side, PWM
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    // Init pins for ADC for feedback and measurement the threshold, Vdc/2
    // PA0 - PHA_feedback
    // PA1 - PHB_feedback
    // PA2 - PHC_feedback
    // PA3 - threshold
    // PA4 - Vdc/2
    // PA5 - Control
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    // Init Tim1
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim1 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct.TIM_Pulse        = 0;
    TIM_OCInitStruct.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState  = TIM_OCIdleState_Set;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);

    TIM_OCInitStruct.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct.TIM_Pulse        = 0;
    TIM_OCInitStruct.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState  = TIM_OCIdleState_Set;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // Init Tim2
    // Event Tim2 (32 bits) generates a phase switch of the BLDC windings,
    // and use to measure the time between zero crossings
    // PCLK1 = 36000000 Mhz, Fclk_tim2 = PCLK1*2 = 72000000*/
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim2 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim2;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable); // You can change CCR1 without waiting for event
    TIM_ARRPreloadConfig(TIM2, DISABLE);

    // Init Tim3
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim6 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim6;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    TIM_ARRPreloadConfig(TIM3, DISABLE);

    // Init DMA for ADC_1 regular channel
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)ADC1->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)array_adc_reg_channel;
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize         = QTY_REGULAR_CHANNEL;
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority           = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    // Init ADC_1
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode       = ENABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel       = QTY_REGULAR_CHANNEL;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_RegularChannelConfig(ADC1, BSP_POWER_DIV2_ADC_CH, ADC_REGULAR_CHANNEL_RANK_1, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, BSP_THROTTLE_CONTROL_ADC_CH, ADC_REGULAR_CHANNEL_RANK_2, ADC_SampleTime_1Cycles5);
    ADC_InjectedSequencerLengthConfig(ADC1, QTY_INJECTED_CHANNELS_4);
    ADC_InjectedChannelConfig(ADC1, BSP_PHA_FEEDBACK_ADC_CH, ADC_INJECTED_CHANNEL_RANK_1, ADC_SampleTime_1Cycles5);
    ADC_InjectedChannelConfig(ADC1, BSP_PHB_FEEDBACK_ADC_CH, ADC_INJECTED_CHANNEL_RANK_2, ADC_SampleTime_1Cycles5);
    ADC_InjectedChannelConfig(ADC1, BSP_PHC_FEEDBACK_ADC_CH, ADC_INJECTED_CHANNEL_RANK_3, ADC_SampleTime_1Cycles5);
    ADC_InjectedChannelConfig(ADC1, BSP_THRESHOLD_ADC_CH, ADC_INJECTED_CHANNEL_RANK_4, ADC_SampleTime_1Cycles5);
    ADC_DMACmd(ADC1, ENABLE);

    // Init interrupts
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel                   = ADC1_2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = DISABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                   = TIM1_CC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                   = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief Deinit board support package
 *
 * @return int32_t
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
void bsp_deinit(void)
{
    // TODO: implement
}

/**
 * @brief Reboot MCU
 *
 * @warning No return from this function!
 *
 * @return int32_t
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
void bsp_reboot(void)
{
    // TODO: implement
}

/**
 * @brief Go to bootloader
 *
 * @warning No return from this function!
 *
 * @return int32_t
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
void bsp_goto_bootloader(void)
{
    // TODO: implement
}

/**
 * @brief Delay in microseconds
 *
 * @param delay microseconds to delay
 */
void bsp_delay_us(uint32_t delay_us)
{
    // TODO: implement
}

/**
 * @brief Start adc regular channels convertion use DMA
 * 
 */
void bsp_adc_start_DMA_ch(void)
{
    if (true == is_adc_reg_ch_converted())
    {
        DMA_Cmd(DMA1_Channel1, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel1, BSP_DMA_QTY_DATA);
        DMA_Cmd(DMA1_Channel1, ENABLE);
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    }
}

/**
 * @brief Get adc regular channel values
 *
 * @param[in] n_ch: ADC_Channel_x, x = 0..15
 */
uint16_t bsp_adc_get_DMA_ch_values(const uint8_t n_ch)
{
    return array_adc_reg_channel[array_adc_map_dma_reg_channel[n_ch]];
}

/**
 * @brief Is ADC regular channels converted
 *
 * @return true: regular channels converted
 *         false: regular channels not converted yet
 */
bool is_adc_DMA_ch_ready(void)
{
    bool rv = false;

    if (SET == DMA_GetFlagStatus(DMA1_FLAG_TC1))
    {
        rv = true;
    }

    return rv;
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
