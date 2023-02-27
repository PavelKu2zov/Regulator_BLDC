/**
  * @file      bsp.h
  *
  * @brief     Board support package
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

/** \addtogroup bsp
*  @{
*/

#ifndef __BSP_H_
#define __BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include <stdint.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/



/******************************************************************************
 * PUBLIC TYPES
 ******************************************************************************/

/** Button enumeration */
typedef enum
{
    BSP_BUTTON_1,                   /**< BSP button 1 */
    BSP_BUTTON_2,                   /**< BSP button 2 */

    BSP_BUTTON_MAX_VAL              /**< Max value for checking */
} bsp_button_t;

/** Button state enumeration */
typedef enum 
{
    BSP_BUTTON_STATE_PRESSED,       /**< Button state - Pressed */
    BSP_BUTTON_STATE_RELEASED,      /**< Button state - Released */

    BSP_BUTTON_STATE_MAX_VAL        /**< Button state - Maximum value */
} bsp_button_state_t;

/** LED enumeration */
typedef enum
{
    BSP_LED_1,                      /**< LED - 1 */
    BSP_LED_2,                      /**< LED - 2 */

    BSP_LED_MAX_VAL                 /**< LED - Maximum value */
} bsp_led_t;

/** LED state enumeration */
typedef enum
{
    BSP_LED_STATE_ON,               /**< LED State - On */
    BSP_LED_STATE_OFF,              /**< LED State - Off */
    BSP_LED_STATE_BLINK_5HZ,        /**< LED State - Blink 5 Hz */

    BSP_LED_STATE_MAX_VAL           /**< LED State - Maximum value */
} bsp_led_state_t;

/** GPIO enumeration */
typedef enum
{
    BSP_GPIO_1,                     /**< GPIO - 1 */
    BSP_GPIO_2,                     /**< GPIO - 2 */

    BSP_GPIO_MAX_VAL                /**< GPIO - Maximum value */
} bsp_gpio_t;

/** GPIO state enumeration */
typedef enum
{
    BSP_GPIO_STATE_ACTIVE,          /**< GPIO State - Active */
    BSP_GPIO_STATE_INACTIVE,        /**< GPIO State - Inactive */

    BSP_GPIO_STATE_MAX_VAL          /**< GPIO State - Maximum value */
} bsp_gpio_state_t;

/** BSP callback context structure*/
typedef union bsp_cb_ctx_s
{
    struct button                   /**< Button structure */
    {
        bsp_button_t source;        /**< Source - button */
        bsp_button_state_t state;   /**< State - button */
    };
    struct gpio                     /**< GPIO structure */
    {
        bsp_gpio_t source;          /**< Source - GPIO */
        bsp_gpio_state_t state;     /**< State - GPIO*/
    };
} bsp_cb_ctx_t;

typedef void (*bsp_cb_t)(const bsp_cb_ctx_t * const p_ctx);     /**< BSP callback function pointer typedef */

/******************************************************************************
 * INLINE FUNCTIONS
 ******************************************************************************/



/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

int32_t bsp_init                (void);
int32_t bsp_deinit              (void);

int32_t bsp_reboot              (void);
int32_t bsp_goto_bootloader     (void);

void    bsp_delay_ms            (uint32_t delay_ms);

int32_t bsp_led_state_get       (bsp_led_t led, bsp_led_state_t * p_state);
int32_t bsp_led_state_set       (bsp_led_t led, bsp_led_state_t state);

int32_t bsp_button_state_get    (bsp_button_t button, bsp_button_state_t * p_state);
int32_t bsp_button_cb_set       (bsp_button_t button, bsp_cb_t func_cb);

int32_t bsp_gpio_state_get      (bsp_gpio_t gpio, bsp_gpio_state_t * p_state);
int32_t bsp_gpio_state_set      (bsp_gpio_t gpio, bsp_gpio_state_t state);
int32_t bsp_gpio_cb_set         (bsp_gpio_t gpio, bsp_cb_t func_cb);

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __BSP_H_

/** @}*/
