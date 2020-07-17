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
#ifndef TP_UART_H
#define TP_UART_H
/*=====[Inclusions of public function dependencies]==========================*/
//#include "sapi_peripheral_map.h"
#include "tp_uart_config.h" /* OSAL interface */
/*=====[C++ - begin]=========================================================*/
#ifdef __cplusplus
extern "C" {
#endif
/*=====[Definition macros of public constants]===============================*/
#define ISR 0 /* critical from ISR        */
#define PORT 1 /* when critical from PORT */
#define C_FIELD_POS 0 /* ptr[C_FIELD_POS] */
#define TX 101
#define RX 102
#define OK 0
#define QUEUE_ERROR -1
#define TIMER_ERROR -2
/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/
/* for recv & send fsm  */
typedef enum {
	standByState, busyState, finishState, errorState
} tp_state_t;
/* message rx & tx */
typedef uint8_t * Message;
/* Uart's attributes... */
typedef struct {
	uartMap_t uart; /* UART_GPIO/UART_485/UART_USB/UART_ENET/UART_232 */
	uint32_t baudrate;
	QueueHandle_t rxQueue;
	QueueHandle_t txQueue;
	TimerHandle_t rxTimeOut;
	TimerHandle_t txTimeOut;
	QMPool rxMemPool; /* memory pool object rx */
	QMPool txMemPool; /* memory pool object tx */
	tp_state_t rxState;
	tp_state_t txState;
	uint8_t rxMemPoolSto[ POOL_STO];
	uint8_t txMemPoolSto[ POOL_STO];
	uint8_t *rxPtr;
	uint8_t *txPtr;
	uint8_t rxIndex;
	uint8_t txIndex;
	uint8_t txCrc8;
} Uart;
/*=====[Prototypes (declarations) of public functions]=======================*/
/* Uart's operations (Uart's interface)... */
int32_t Uart_ctor(Uart * const me, uartMap_t uart, uint32_t baudrate);
void Uart_init(Uart * const me);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/
void Uart_rxCallback(void *);
void Uart_txCallback(void *);
void Uart_rxTimeout(TimerHandle_t xTimer);
void Uart_txTimeout(TimerHandle_t xTimer);
/*=====[C++ - end]===========================================================*/
#ifdef __cplusplus
}
#endif
/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* TP_UART_H */
