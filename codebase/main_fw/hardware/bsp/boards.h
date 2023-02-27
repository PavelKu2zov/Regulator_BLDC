/**
  * @file      boards.h
  *
  * @brief     Boards module definition
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

#ifndef __BOARDS_H_
#define __BOARDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#if defined(CONFIG_BOARD_NUCLEO_F103RB)
    #include "brd_nucleo_f103rb.h"
#elif defined(CONFIG_BOARD_BLUEPILL_F103RB)
    #error "Board support not implemented"
#else
    #error "Board is not defined. Please define the target board as a compiler option. See boards.h"
#endif

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __BOARDS_H_

/** @}*/
