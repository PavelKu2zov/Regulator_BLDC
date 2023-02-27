/**
  * @file      bsp.c
  *
  * @brief     Board support package
  *
  * @date      2023-01-23
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

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "bsp.h"
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
int32_t bsp_init (void)
{
    // TODO: implement
}

/**
 * @brief Deinit board support package
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
int32_t bsp_deinit (void)
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
int32_t bsp_reboot          (void)
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
int32_t bsp_goto_bootloader (void)
{
    // TODO: implement
}

/**
 * @brief Delay in milliseconds
 * 
 * @param delay_ms milliseconds to delay
 */
void bsp_delay_ms (uint32_t delay_ms)
{
    // TODO: implement
}

/**
 * @brief Get LED state
 * 
 * @param[in] led target LED
 * @param[out] p_state pointer to holding led state
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings
 */
int32_t bsp_led_state_get (bsp_led_t led, bsp_led_state_t * p_state)
{
    // TODO: implement
}

/**
 * @brief Set LED state
 * 
 * @param[in] led target LED
 * @param[in] state target state
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_led_state_set (bsp_led_t led, bsp_led_state_t state)
{
    // TODO: implement
}

/**
 * @brief Get button state
 * 
 * @param[in] button target button
 * @param[out] p_state pointer to holding button state
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_button_state_get (bsp_button_t button, bsp_button_state_t * p_state)
{
    // TODO: implement
}

/**
 * @brief Set button callback
 * 
 * @param[in] button target button
 * @param[in] func_cb callback function pointer
 *
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_button_cb_set (bsp_button_t button, bsp_cb_t func_cb)
{
    // TODO: implement
}

/**
 * @brief Get GPIO state
 * 
 * @param[in] gpio taget GPIO
 * @param[out] p_state pointer to holding GPIO state
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_gpio_state_get (bsp_gpio_t gpio, bsp_gpio_state_t * p_state)
{
    // TODO: implement
}

/**
 * @brief Set GPIO state
 * 
 * @param[in] gpio target GPIO
 * @param[in] state target state
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_gpio_state_set (bsp_gpio_t gpio, bsp_gpio_state_t state)
{
    // TODO: implement
}

/**
 * @brief Set GPIO callback
 * 
 * @param[in] gpio target GPIO
 * @param[in] func_cb pointer to a callback function
 * 
 * @return int32_t 
 * @return 0 if OK
 * @return < 0 if there are errors
 * @return > 0 if there are warnings 
 */
int32_t bsp_gpio_cb_set (bsp_gpio_t gpio, bsp_cb_t func_cb)
{
    // TODO: implement
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
