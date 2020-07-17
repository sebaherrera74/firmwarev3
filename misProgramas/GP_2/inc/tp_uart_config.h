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
#ifndef TP_UART_CONFIG_H
#define TP_UART_CONFIG_H
/*=====[Inclusions of public function dependencies]==========================*/
#include <stdint.h>
#include <stddef.h>

#include "qmpool.h"
/* Using FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"
#include "timers.h"

#include "sapi.h"
#include "supporting_functions.h"
#include "string.h"
/* Using QMPOOL defined */

/*=====[C++ - begin]=========================================================*/
#ifdef __cplusplus
extern "C" {
#endif
/*=====[Definition macros of public constants]===============================*/
/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/
//#include "sapi_datatypes.h"
//#include "tp_grupo_2.h"
#define UART_ENTER_CRITICAL( ) portENTER_CRITICAL( );
#define UART_EXIT_CRITICAL( ) portEXIT_CRITICAL( );
#define QueueWaitingISR( R ) uxQueueMessagesWaiting( R ); see
#define SendToApp(S,T) xQueueSendToBackFromISR(S,T,0);
#define GetFromApp(V,W) xQueueReceiveFromISR(V,W,pdFALSE);
#define MAX_INC_FRAME 64 /* '(' & ')' don't count */
#define MIN_INC_FRAME 4  /* 1C + 1DATA + 2CRC */
#define QUEUE_SIZE 5
#define BAUD_RATE 115200
#define POOL_STO 256
#define BLOCK_SIZE 64
#define MIN_ASCII_CHAR 0x1F /* printable ascii characters */
#define MAX_ASCII_CHAR 0x7F /* printable ascii characters */
#define FRAME_INIT '('   /* packet initiator */
#define FRAME_FINI ')' /* packet finisher */
#define MSB   1		/* crc8 cMLSB*/
#define LSB   0		/* crc8 c8LSB */
#define RX_TIMEOUT pdMS_TO_TICKS(50)
#define TX_TIMEOUT pdMS_TO_TICKS(50)

/* if DEBUG */
#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif
/* if DEBUG */
#ifdef QMPOOL
#define QMPOOL_TEST 1
#else
#define QMPOOL_TEST 0
#endif
/* end if DEBUG */
#define DebugPrint(S) \
            if (DEBUG_TEST) vPrintString(S);
/* #define tp_queue_t QueueHandle_t */
/* #define tp_timer_t TimerHandle_t */
/*=====[Prototypes (declarations) of public functions]=======================*/
uint8_t * GetMemory(uint8_t intoIsr);
void ClearMemory(uint8_t * block, uint8_t intoIsr);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/
/*=====[C++ - end]===========================================================*/
#ifdef __cplusplus
}
#endif
/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* TP_UART_CONFIG_H */
