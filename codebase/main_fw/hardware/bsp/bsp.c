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
#include "stm32f30x.h"
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
 *
 * @return int32_t
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
int32_t bsp_init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    RCC_PCLK1Config(RCC_HCLK_Div2); // PCLK1 36 MHz ref.manual page 115
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div4); // 10

    // X_Low side
    GPIO_InitTypeDef GPIO_InitStructre;
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructre);

    // X-High Side, PWM
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    // Init alternative functions x-High side to Tim_CHx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_6);

    // Init pins for ADC for feedback and for Op-amp
    // Note: GPIO_Pin_4 destroyed on board!! GPIO_Pin_7 is instead of him
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AN;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    // Init PB14 as analog for Op-amp
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AN;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructre);

#ifdef USE_SBUS
    // Init gpios USART3_RX*/
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_NOPULL; // GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructre);
    // Init alternative functions PB8
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_7);

#elif USE_FRSKY
    // Init TIM16_CH1 for capture control impulse
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructre);
    // Init alternative functions PB8
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
#endif

    /*��������� PB12 �� ����� CurrentOverflow*/
    GPIO_InitStructre.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructre.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructre);

    // Init USART3 for SBUS
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate            = 100000;
    USART_InitStruct.USART_WordLength          = USART_WordLength_9b;
    USART_InitStruct.USART_StopBits            = USART_StopBits_2;
    USART_InitStruct.USART_Parity              = USART_Parity_Even;
    USART_InitStruct.USART_Mode                = USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStruct);
    // USART_OverSampling8Cmd(USART3, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

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

    // Init Tim6
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim6 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim6;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
    TIM_ARRPreloadConfig(TIM6, DISABLE);

// Init Tim16
#ifdef USE_SBUS
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim16 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim16;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseInitStruct);
    TIM_ARRPreloadConfig(TIM16, DISABLE);

    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);

#elif USE_FRSKY
    TIM_TimeBaseInitStruct.TIM_Prescaler         = PrescalerTim16 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = Autoreload_Tim16;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseInitStruct);
    TIM_ARRPreloadConfig(TIM16, DISABLE);

    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICFilter    = 0;
    TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_BothEdge;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM16, &TIM_ICInitStruct);

    TIM_ITConfig(TIM16, TIM_IT_CC1, DISABLE);
#endif

    // Init DMA1 for USART3_RX
    DMA_DeInit(DMA1_Channel3);
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART3->RDR;
    DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)&buff_usart3_rx;
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize         = SIZE_BUFF_USART3_RX_DMA;
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Circular; // DMA_Mode_Normal
    DMA_InitStruct.DMA_Priority           = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);

    // Init OPAMP_2
    OPAMP_InitTypeDef OPAMP_InitStruct;
    OPAMP_InitStruct.OPAMP_InvertingInput    = OPAMP_InvertingInput_PGA; // OPAMP_InvertingInput_IO2;
    OPAMP_InitStruct.OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO2;
    OPAMP_Init(OPAMP_Selection_OPAMP2, &OPAMP_InitStruct);
    OPAMP_PGAConfig(OPAMP_Selection_OPAMP2, OPAMP_OPAMP_PGAGain_16, OPAMP_PGAConnect_No); // OPAMP_PGAConnect_IO2

    // Init ADC_1
    // Common settings of ADC*/
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_CommonInitStruct.ADC_Clock            = ADC_Clock_AsynClkMode; // HCLK/4
    ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_DMAMode          = ADC_DMAMode_OneShot;
    ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = 0;
    ADC_CommonInit(ADC1, &ADC_CommonInitStruct);

    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable; // continuous conversion, only for regular channel
    // Note: when changing resolution, you need to change the coefficient. to determine the supply voltage
    ADC_InitStruct.ADC_Resolution            = ADC_Resolution_12b; // ADC_Resolution_8b;
    ADC_InitStruct.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
    ADC_InitStruct.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStruct.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_OverrunMode           = ADC_OverrunMode_Enable;
    ADC_InitStruct.ADC_AutoInjMode = ADC_AutoInjec_Disable; // Injchanel with enable will automatically do the conversion immediately after
                                                            // the conversion of the reg.channel
    ADC_InitStruct.ADC_NbrOfRegChannel = 1;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_InjectedInitTypeDef ADC_InjectedInitStruct;
    ADC_InjectedInitStruct.ADC_ExternalTrigInjecConvEvent = ADC_ExternalTrigInjecConvEvent_0;
    ADC_InjectedInitStruct.ADC_ExternalTrigInjecEventEdge = ADC_ExternalTrigInjecEventEdge_None;
    ADC_InjectedInitStruct.ADC_NbrOfInjecChannel          = 4;
    ADC_InjectedInitStruct.ADC_InjecSequence1             = ADC_InjectedChannel_1; // PA0 1
    ADC_InjectedInitStruct.ADC_InjecSequence2             = ADC_InjectedChannel_2; // PA1
    ADC_InjectedInitStruct.ADC_InjecSequence3             = ADC_InjectedChannel_4; // PA3
    ADC_InjectedInitStruct.ADC_InjecSequence4             = ADC_InjectedChannel_5; // PA4  5 was ADC_InjectedChannel_5
    ADC_InjectedInit(ADC1, &ADC_InjectedInitStruct);

    ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_InjectedChannel_1, ADC_SampleTime_7Cycles5);
    ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_InjectedChannel_2, ADC_SampleTime_7Cycles5);
    ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_InjectedChannel_4, ADC_SampleTime_7Cycles5);
    ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_InjectedChannel_5, ADC_SampleTime_7Cycles5); // was ADC_InjectedChannel_5

    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5); // Rank = 1 for OPAMP2
    ADC_RegularChannelSequencerLengthConfig(ADC1, 1);                           // SequencerLength = 1

    // Init ADC_AnalogWatchdog
    ADC_AnalogWatchdog1SingleChannelConfig(ADC1, ADC_Channel_10);
    ADC_AnalogWatchdog1ThresholdsConfig(ADC1, AWD_HighThreshold, AWD_LowThreshold);

    // Init interrupts
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel                   = ADC1_IRQn;
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

    NVIC_InitStruct.NVIC_IRQChannel                   = TIM1_UP_TIM16_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = DISABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM6, ENABLE);
    TIM_Cmd(TIM16, ENABLE);

    DMA_Cmd(DMA1_Channel3, ENABLE);
    SPI_Cmd(SPI3, DISABLE);
    USART_Cmd(USART3, ENABLE);

    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

    // Calibrate the ADC_1
    // ADC_DisableCmd(ADC1);
    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
    TIM2->CNT = 0;
    TIM2->ARR = 0xFFFFFFFF;
    while (TIM2->CNT < 0xafc80)
        ; // wait 10 ms
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) != RESET)
        ;
    TIM2->CNT = 0;
    TIM2->ARR = 0xFFFFFFFF;
    while (TIM2->CNT < 0xafc80)
        ; // wait 10 ms
    ADC_Cmd(ADC1, ENABLE);
    TIM2->CNT = 0;
    TIM2->ARR = 0xFFFFFFFF;
    while (TIM2->CNT < 0xafc80)
        ; // wait 10 ms
    ADC_StartConversion(ADC1);

    // Calibrate OPAMP_2
    // Calibrate the NMOS differential pair
    OPAMP_Cmd(OPAMP_Selection_OPAMP2, ENABLE); // enable OPAMP_2
#ifdef 0
    OPAMP_OffsetTrimModeSelect(OPAMP_Selection_OPAMP2, OPAMP_Trimming_User);  // Enable the user offset trimming by setting the USERTRIM bit
    OPAMP_StartCalibration(OPAMP_Selection_OPAMP2, ENABLE);                   // Connect VM and VP to the internal reference voltage
    OPAMP_VrefConfig(OPAMP_Selection_OPAMP2, OPAMP_Vref_90VDDA);              // OPAMP internal reference =0.9 x VDDA
    OPAMP_OffsetTrimConfig(OPAMP_Selection_OPAMP2, OPAMP_Input_Inverting, 0); // TRIMOFFSETN bits set zero
    TIM2->CNT = 0;
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CNT = 0;
    while (TIM2->CNT < 0xafc80)
        ; //������� 10 ms
    uint32_t tm = 0;
    while (OPAMP_GetOutputLevel(OPAMP_Selection_OPAMP2) != OPAMP_OutputLevel_Low)
    {
        tm++;
        OPAMP_OffsetTrimConfig(OPAMP_Selection_OPAMP2, OPAMP_Input_Inverting, (tm & 0x1f));
        TIM2->CNT = 0;
        while (TIM2->CNT < 0xafc80)
            ; //������� 10 ms
    }

    // Calibrate the PMOS differential pair
    OPAMP_VrefConfig(OPAMP_Selection_OPAMP2, OPAMP_Vref_10VDDA); // OPAMP internal reference =0.9 x VDDA
    tm = 0;

    OPAMP_OffsetTrimConfig(OPAMP_Selection_OPAMP2, OPAMP_Input_NonInverting, 0); // TRIMOFFSETN bits set zero
    TIM2->CNT = 0;
    while (TIM2->CNT < 0xafc80)
        ; //������� 10 ms
    while (OPAMP_GetOutputLevel(OPAMP_Selection_OPAMP2) != OPAMP_OutputLevel_Low)
    {
        tm++;
        OPAMP_OffsetTrimConfig(OPAMP_Selection_OPAMP2, OPAMP_Input_NonInverting, (tm & 0x1f));
        TIM2->CNT = 0;
        while (TIM2->CNT < 0xafc80)
            ; //������� 10 ms
    }

    OPAMP_StartCalibration(OPAMP_Selection_OPAMP2, DISABLE); // calibration mode disabled
    ADC_ClearFlag(ADC1, ADC_IT_AWD1);
    ADC_ITConfig(ADC1, ADC_IT_AWD1, ENABLE);
#endif
}

/**
 * @brief Deinit board support package
 *
 * @return int32_t
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
int32_t bsp_deinit(void)
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
int32_t bsp_reboot(void)
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
int32_t bsp_goto_bootloader(void)
{
    // TODO: implement
}

/**
 * @brief Delay in milliseconds
 *
 * @param delay_ms milliseconds to delay
 */
void bsp_delay_ms(uint32_t delay_ms)
{
    // TODO: implement
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
