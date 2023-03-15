/**
 * @file      bsp.h
 *
 * @brief     Board support package
 *
 * @date      2023-01-20
 *
 */

/** \addtogroup bsp
 *  @{
 */

#ifndef __BSP_H_
#define __BSP_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************
     * INCLUDES
     ******************************************************************************/

#include <stdint.h>
#include "bsp_cfg.h"

/******************************************************************************
 * DEFINES
 ******************************************************************************/
// Timer1
// 0x40012C00
#define TIM1_CNT  (*(uint32_t *)0x40012C24)
#define TIM1_SR   (*(uint32_t *)0x40012C10)
#define TIM1_DIER (*(uint32_t *)0x40012C0C)
#define TIM1_CCER (*(uint32_t *)0x40012C20)
#define TIM1_CCR1 (*(uint32_t *)0x40012C34)
#define TIM1_CCR2 (*(uint32_t *)0x40012C38)
#define TIM1_CCR3 (*(uint32_t *)0x40012C3C)
#define TIM1_CCR4 (*(uint32_t *)0x40012C40)

/*Timer 2*/
#define TIM2_CNT  (*(uint32_t *)0x40000024)
#define TIM2_CCR1 (*(uint32_t *)0x40000034)
#define TIM2_ARR  (*(uint32_t *)0x4000002C)

#define k1_open  (TIM1->CCER |= (1 << 0));
#define k1_close (TIM1->CCER &= ~(1 << 0));
#define k2_open  (TIM1->CCER |= (1 << 4));
#define k2_close (TIM1->CCER &= ~(1 << 4));
#define k3_open  (TIM1->CCER |= (1 << 8));
#define k3_close (TIM1->CCER &= ~(1 << 8));
#define k4_close (GPIOB->BSRR = GPIO_BSRR_BR1);
#define k5_close (GPIOB->BSRR = GPIO_BSRR_BR7);
#define k6_close (GPIOB->BSRR = GPIO_BSRR_BR6);
#define k4_open  (GPIOB->BSRR = GPIO_BSRR_BS1);
#define k5_open  (GPIOB->BSRR = GPIO_BSRR_BS7);
#define k6_open  (GPIOB->BSRR = GPIO_BSRR_BS6);

    /******************************************************************************
     * PUBLIC TYPES
     ******************************************************************************/
    

    /******************************************************************************
     * INLINE FUNCTIONS
     ******************************************************************************/


    /******************************************************************************
     * PUBLIC FUNCTION PROTOTYPES
     ******************************************************************************/

    void bsp_init(void);
    void bsp_deinit(void);
    void bsp_reboot(void);
    void bsp_goto_bootloader(void);
    void bsp_delay_us(uint32_t delay_us);
    void bsp_adc_start_DMA_ch(void);
    uint16_t bsp_adc_get_DMA_ch_values(const uint8_t n_ch);
    bool is_adc_DMA_ch_ready(void);

    




/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __BSP_H_

/** @}*/
