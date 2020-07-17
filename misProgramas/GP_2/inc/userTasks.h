/*============================================================================
 * Copyright (c) 2020, TP RTOS II - CESE -UBA
 * Miguel del Valle <m.e.delvallecamino@ieee.org>
 * Leandro Torrent <leandrotorrent92@gmail.com>
 * Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * 
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/04/24
 * Version: 2.1.2
 *===========================================================================*/
/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef __USER_TASKS_H__
#define __USER_TASKS_H__
/*=====[Inclusions of public function dependencies]==========================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
/*=====[C++ - begin]=========================================================*/
#ifdef __cplusplus
extern "C" {
#endif
/*=====[Definition macros of public constants]===============================*/
/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/
/*=====[Prototypes (declarations) of public functions]=======================*/
void tickTask(void* taskParmPtr);  // Task declaration
void app(void* pvParameters);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/
/*=====[C++ - end]===========================================================*/
#ifdef __cplusplus
}
#endif
/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* __USER_TASKS_H__ */
