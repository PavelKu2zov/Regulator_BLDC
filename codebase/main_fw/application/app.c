/**
 * @file      app.c
 *
 * @brief     Application
 *
 * @date      2023-01-20
 *
 * @par
 * Copyright (C), 2023, YADRO Development Center, LLC («YADRO»).
 * All Rights Reserved.
 *
 * This software contains the intellectual property of YADRO
 * or is licensed to YADRO from third parties. Use of this
 * software and the intellectual property contained therein is expressly
 * limited to the terms and conditions of the License Agreement under which
 * it is provided by YADRO.
 *
 */

/** \addtogroup app
 *  @{
 */

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "app.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "bsp.h"
#include "stm32f30x.h"

/******************************************************************************
 * DEFINES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/
typedef enum
{
    start = 0,
    run,
    stop
} state_t;

typedef struct
{
    uint8_t  position; // switching position number
    uint8_t  revers;   // 1 - reverse rotation
    uint8_t  Wdt_com;  //
    uint32_t time_sence_mem[16];
    uint8_t  index_mem;
    uint8_t  use_delta_gas;
    uint32_t time_sence; // time between zeros
    uint32_t time_sence_start;
    uint32_t gas;     // throttle stick size
    uint32_t a;       // current sensor value
    uint16_t counter; // cyclic packet counter
    uint16_t start_gas_value;
    float    Vdc; // supply voltage

    // services parameters
    uint32_t k;      // GASa value recalculation factor by SPI
    uint32_t MaxGas; // maximum counter value TIM1
    uint8_t  SenceEnable;
    uint32_t Operegenie;
    uint32_t Cnt_start;
    state_t  state;
} state_BLDC_t;
/******************************************************************************
 * PRIVATE DATA
 ******************************************************************************/
state_BLDC_t state_BLDC;

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
static void Stop_BLCD(void);
static void CalculateVDC(void);

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * @brief Stop BLDC
 * 
 */
static void Stop_BLCD(void)
{
    // Disable interrupt for switching windings
    TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
    k1_close;
    k2_close;
    k3_close;
    k4_close;
    k5_close;
    k6_close;
    
    StateBLDC.state = stop;
    // CalculateVDC();
}

/**
 * @brief Calculate the power supply
 * 
 */
static void CalculateVDC(void)
{
  uint16_t tm=0;
  uint16_t max_val_adc=0; 
  for (int i=0;i<16;i++)
  {
   ADC1->ISR = ADC_ISR_JEOS; //clear JEOC flag
   ADC1->CR |= ADC_CR_JADSTART; //start convertion
   while ((ADC1->ISR & ADC_ISR_JEOS) != ADC_ISR_JEOS); 
    tm += *Threshold_Vp_div2;
  }
  tm >>=4;
  
  if (((ADC1->CFGR>>3)&3) == 0)//12 bit resolution
    max_val_adc = 4095;
  else if (((ADC1->CFGR>>3)&3) == 1)//10 bit resolution 
    max_val_adc = 1023;
  else if (((ADC1->CFGR>>3)&3) == 2)//8 bit resolution 
  max_val_adc = 255;
    else if (((ADC1->CFGR>>3)&3) == 3)//6 bit resolution 
  max_val_adc = 63;
  
  StateBLDC.Vdc = (3.3*tm)/(K_Rdiv*max_val_adc); 
  
   //����� �������� gas ��� �������
 if (StateBLDC.Vdc > 11)
   StateBLDC.start_gas_value = 2*Autoreload_Tim1/100;//2%
 else  if (StateBLDC.Vdc > 10)
   StateBLDC.start_gas_value = 3*Autoreload_Tim1/100;//3%  
 else 
   StateBLDC.start_gas_value = 6*Autoreload_Tim1/100;//6%   

}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

/**
 * @brief Application entry point
 *
 * @warning no exit from this function!
 *
 */
void app_entry_point(void)
{
    bsp_init();

    k1_close;
    k2_close;
    k3_close;
    k4_close;
    k5_close;
    k6_close;

    StateBLDC.MaxGas        = Autoreload_Tim1;
    StateBLDC.SenceEnable   = 1;
    StateBLDC.Operegenie    = OperegenieDefine;
    StateBLDC.counter       = 0;
    StateBLDC.revers        = 0;
    StateBLDC.index_mem     = 0;
    StateBLDC.use_delta_gas = 1;
    for (int i = 0; i < 16; i++)
        StateBLDC.time_sence_mem[i] = 0;

    GPIOB->BSRR = GPIO_BSRR_BS_12;
    TIM2->ARR   = 0xFFFFFFFF;
    TIM2->CNT   = 0;
    while (TIM2->CNT < 24000000)
        ;
    GPIOB->BSRR = GPIO_BSRR_BR_12;
    TIM2->ARR   = Autoreload_Tim2;
    TIM2->CNT   = 0;

    TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC4, DISABLE); // ENABLE

    Stop_BLCD();

    for (;;)
    {
        TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

        if (TIM6->CNT > MS_1)
        {

            TIM6->CNT = 0;
            if (timer_read_adc > 0)
                timer_read_adc--;
            if (timer_read_spi > 0)
                timer_read_spi--;
        }

        // overcurrent
        if (ADC_ch10_index >= 10)
        {
            ADC_ch10       = ADC_ch10_sum / ADC_ch10_index;
            ADC_ch10_index = 0;
            ADC_ch10_sum   = 0;
        }

        ADC_ch10_sum += ADC_GetConversionValue(ADC1);
        ADC_ch10_index++;

        if (0) //(ADC_ch10 > 1800)// 1000~ 5A; ���� 1800
        {
            GPIOB->BSRR = GPIO_BSRR_BS_12;
            Stop_BLCD();
            while (StateBLDC.gas > 50)
                ;
            GPIOB->BSRR = GPIO_BSRR_BR_12;
        }
        //    else
        //    {
        //       GPIOB->BSRR = GPIO_BSRR_BR_12;
        //    }

        // rotation analysis available or not
        if (timer_read_adc == 0)
        {
            timer_read_adc = 500;
            if ((*phasa_A + *phasa_B + *phasa_C) < 1) //(StateBLDC.gas < 50)
            {
                StateBLDC.state = stop;
                // GPIOB->BSRR = GPIO_BSRR_BS_12;
                // CalculateVDC();
            }
        }
        if ((StateBLDC.state == stop) && (StateBLDC.gas > 50))
        {
            // Start_BLDC();

            TIM2->ARR           = 0xffffffff;
            TIM2->CCR1          = 0x5;
            TIM2->CNT           = 0;
            StateBLDC.Cnt_start = 1000;
            StateBLDC.state     = run;
            TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

            // GPIOB->BSRR = GPIO_BSRR_BR_12;
        }
    }
}

/**
 * @brief TIM1 interrupt handler. Check phase feedback and capture time between commutations.
 *
 */
void TIM1_CC_IRQHandler(void)
{

    TIM1->SR &= ~TIM_SR_CC1IF;
    if (state_BLDC.SenceEnable == 0)
    {
        do
        {
            ADC1->ISR = ADC_ISR_JEOS;    // clear JEOC flag
            ADC1->CR |= ADC_CR_JADSTART; // start convertion
            while ((ADC1->ISR & ADC_ISR_JEOS) != ADC_ISR_JEOS)
                ;

            if (*sence1 > *sence2)
            {
                //            if (( GPIOB->ODR & GPIO_ODR_12) == GPIO_ODR_12)
                //            {GPIOB->BSRR = GPIO_BSRR_BR_12;}
                //            else
                //            {GPIOB->BSRR = GPIO_BSRR_BS_12;}
                for (int i = 0; i < 10; i++)
                    ; // without this delay does not start why it is not clear??????

                //        state_BLDC.time_sence =  TIM2->CNT>>1;
                if (state_BLDC.state == run)
                {

                    if (state_BLDC.index_mem > 15)
                        state_BLDC.index_mem = 0;

                    state_BLDC.time_sence_mem[state_BLDC.index_mem++] = TIM2->CNT >> 1;
                    state_BLDC.time_sence                             = TIM2->CNT >> 1;
                    TIM2->CNT                                         = 0;
                    // state_BLDC.time_sence=0;

                    //               for (int i=0;i<16;i++)
                    //                  state_BLDC.time_sence += state_BLDC.time_sence_mem[i];

                    TIM2->CCR1 = state_BLDC.time_sence; //-state_BLDC.Operegenie;
                }
                TIM1->DIER &= ~TIM_DIER_CC4IE;
                TIM1->SR = ~TIM_SR_CC4IF;

                //             if ((state_BLDC.time_sence < 18000) && (Threshold != Threshold_Vp_div2))// 2000 ��/��� 18000
                //             {
                //                TIM1->CCR4 = 200;
                //                Threshold = Threshold_Vp_div2;
                //
                //             }
                if (state_BLDC.time_sence < 12000) //
                    state_BLDC.use_delta_gas = 0;
                break;
            }

        } while ((TIM1->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF);
    }
    else
        state_BLDC.SenceEnable--;

    TIM1->SR = ~TIM_SR_CC4IF;
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
}

/**
 * @brief TIM2 interrupt handler. Commutation phases.
 *
 */
void TIM2_IRQHandler()
{

    CountRate++;
    //////////////////////////////////////////////////////////////////////////////////////new
    if (state_BLDC.state == start)
        TIM2->CNT = 0;
    //////////////////////////////////////////////////////////////////////////////////////new

    if (state_BLDC.position == 5)
        state_BLDC.position = 0;
    else
        state_BLDC.position++;

    // disable the interrupt globally so that when switching does not go into an overcurrent interrupt if it occurs
    void irq_disable();
    if (!state_BLDC.revers)
    {
        switch (state_BLDC.position)
        {
            case 0:
                k6_close;
                delay();
                k5_open;
                sence1 = phasa_C;
                sence2 = Threshold;
                break; // phasa_C free � + sence PA3
            case 1:
                k1_close;
                delay();
                k3_open;
                sence2 = phasa_A;
                sence1 = Threshold;
                break; // phasa_A free � - sence PA0
            case 2:
                k5_close;
                delay();
                k4_open;
                sence1 = phasa_B;
                sence2 = Threshold;
                break; // phasa_B free � + sence PA1
            case 3:
                k3_close;
                delay();
                k2_open;
                sence2 = phasa_C;
                sence1 = Threshold;
                break; // phasa_C free � - sence PA3
            case 4:
                k4_close;
                delay();
                k6_open;
                sence1 = phasa_A;
                sence2 = Threshold;
                break; // phasa_A free � + sence PA0
            case 5:
                k2_close;
                delay();
                k1_open;
                sence2 = phasa_B;
                sence1 = Threshold;
                break; // phasa_B free � - sence PA1
            default:
                k1_close;
                k2_close;
                k3_close;
                k4_close;
                k5_close;
                k6_close;
                break;
        }
    }
    else // reverse
    {
        switch (state_BLDC.position)
        {
            case 0:
                k6_close;
                delay();
                k4_open;
                sence1 = phasa_C;
                sence2 = Threshold;
                break; // phasa_C free � + sence PA3
            case 1:
                k2_close;
                delay();
                k3_open;
                sence2 = phasa_B;
                sence1 = Threshold;
                break; // phasa_B free � - sence PA1
            case 2:
                k4_close;
                delay();
                k5_open;
                sence1 = phasa_A;
                sence2 = Threshold;
                break; // phasa_A free � + sence PA0
            case 3:
                k3_close;
                delay();
                k1_open;
                sence2 = phasa_C;
                sence1 = Threshold;
                break; // phasa_C free � - sence PA3
            case 4:
                k5_close;
                delay();
                k6_open;
                sence1 = phasa_B;
                sence2 = Threshold;
                break; // phasa_B free � + sence PA1
            case 5:
                k1_close;
                delay();
                k2_open;
                sence2 = phasa_A;
                sence1 = Threshold;
                break; // phasa_A free � - sence PA0
            default:
                k1_close;
                k2_close;
                k3_close;
                k4_close;
                k5_close;
                k6_close;
                break;
        }
    }

    void irq_enable();

    if (0) //(state_BLDC.state == start)
    {
        while (TIM2->CNT < 60000)
            ;
        switch (state_BLDC.position)
        {
            case 0:
                k1_close;
                break;
            case 1:
                k5_close;
                break;
            case 2:
                k3_close;
                break;
            case 3:
                k4_close;
                break;
            case 4:
                k2_close;
                break;
            case 5:
                k6_close;
                break;
        }
    }

    if (state_BLDC.Cnt_start < CNT_START)
        state_BLDC.Cnt_start++;

    state_BLDC.SenceEnable = 2; // 1

    TIM_ClearFlag(TIM2, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    TIM1->SR = ~TIM_SR_CC4IF;
    TIM1->DIER |= TIM_DIER_CC4IE;
    NVIC->ICPR[0] = 1 << TIM2_IRQn;
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
