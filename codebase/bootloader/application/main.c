/**
  * @file      main.c
  *
  * @brief     Entry point
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

/** \addtogroup main
*  @{
*/

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "app.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

/**
 * @brief Firmware entry point
 * 
 * @warning This function shouldn't reach return. Inside this function there are infinite loop
 * 
 * @return int always return 0
 */
int main (void)
{
    app_entry_point();

    // NOTE: Unreachable.
    // TODO: add assert
    return 0;
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
