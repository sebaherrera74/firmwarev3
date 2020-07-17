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
/*=====[Inclusion of own header]=============================================*/
#include "tp_uart.h" /* Uart class interface */

#include <stdio.h>
#include "string.h"
#include "sapi.h"
//#include "sapi_uart.h"

#include "crc8.h"
/*=====[Definition macros of private constants]==============================*/
/*=====[Private function-like macros]========================================*/
/*=====[Definitions of private data types]===================================*/
/*=====[Definitions of external public global variables]=====================*/
/*=====[Definitions of private global variables]=============================*/
/*=====[Prototypes (declarations) of private functions]======================*/
/* Check incoming uart packages */
uint8_t Uart_checkCrc8(uint8_t * buff, uint8_t index);
uint8_t Uart_addCrc8(uint8_t c8, uint8_t val);
/* Reset rx frame parameters: index & fsm state  */
void Uart_resetFsm(Uart *, uint8_t option);
/*=====[Implementations of public functions]=================================*/
/* constructor implementation */
int32_t Uart_ctor(Uart * const me, uartMap_t uart, uint32_t baudrate) {
	/* result is for error parser */
	int32_t result;
	me->uart = uart;
	me->baudrate = baudrate;
	/* sapi-uart init         */
	uartInit(me->uart, me->baudrate);
	/* freertos queues create */
	/* queues are required for xUart instantiation */
	me->rxQueue = xQueueCreate(QUEUE_SIZE, sizeof(Message));
	me->txQueue = xQueueCreate(QUEUE_SIZE, sizeof(Message));
	if (me->rxQueue == NULL || me->txQueue == NULL) {
		return result = QUEUE_ERROR;
	}
	/* freertos timers create */
	/* timers are required for xUart instantiation */
	me->rxTimeOut = xTimerCreate("rx Time Out", RX_TIMEOUT,
	pdFALSE, (void *) me, Uart_rxTimeout);
	me->txTimeOut = xTimerCreate("tx Time Out", TX_TIMEOUT,
	pdFALSE, (void *) me, Uart_txTimeout);
	if (me->rxTimeOut == NULL || me->txTimeOut == NULL) {
		return result = TIMER_ERROR;
	}
	/* QMPool-init                      */
	/* if define QMPOOL                 */
	/* x blocks = POOL_STO / BLOCK_SIZE */
	QMPool_init(&me->rxMemPool, me->rxMemPoolSto, sizeof(me->rxMemPoolSto),
	BLOCK_SIZE);
	QMPool_init(&me->txMemPool, me->txMemPoolSto, sizeof(me->txMemPoolSto),
	BLOCK_SIZE);
	return result = OK;
}
/* Init and set UART callback */
void Uart_init(Uart * const me) {
	uartCallbackSet(me->uart, UART_RECEIVE, Uart_rxCallback, (void*) me);
	uartCallbackSet(me->uart, UART_TRANSMITER_FREE, Uart_txCallback,
			(void*) me);
	uartInterrupt(me->uart, true);
	uartCallbackClr(me->uart, UART_TRANSMITER_FREE);
	me->rxPtr = QMPool_get(&me->rxMemPool, 0, 0);
	/* default values - index & state */
	Uart_resetFsm(me, RX);
	Uart_resetFsm(me, TX);
}
/*=====[Implementations of interrupt functions]==============================*/
/* Uart rx Callback (fsm)     */
/* check delimiters '(' & ')' */
/* check index overflow       */
/* check min incoming frame   */
void Uart_rxCallback(void *param) {
	Uart * me = param;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UBaseType_t uxSavedInterruptStatus;
	uint8_t incChar = uartRxRead(me->uart);
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR ( );
	/* check overflow */
	if (me->rxIndex < ( MAX_INC_FRAME + 1)) {
		/* reset timer        */
		if ( xTimerResetFromISR ( me->rxTimeOut,
				&xHigherPriorityTaskWoken ) != pdPASS) {
			/* if timer got error */
			/* reset frame */
			/* see configTIMER_QUEUE_LENGTH */
			Uart_resetFsm(me, RX);
		} else {
			/* fsm */
			switch (me->rxState) {
			case standByState:
				/* init fsm */
				if (incChar == FRAME_INIT) {
					me->rxState = busyState;
				}
				break;
			case busyState:
				if (incChar > MIN_ASCII_CHAR && incChar < MAX_ASCII_CHAR) {
					/* ASCII is printable */
					if ((incChar == FRAME_FINI)
							&& me->rxIndex > ( MIN_INC_FRAME - 1)) {
						/* fsm finish */
						me->rxState = finishState;
						me->rxIndex--;
						/* packet reception is disabled      */
						uartCallbackClr(me->uart, UART_RECEIVE);
					} else {
						/* write on rxPtr [rxIndex] */
						me->rxPtr[me->rxIndex] = incChar;
						me->rxIndex++;
					}
				} else {
					/* ASCII is not printable */
					/* discard frame */
					/* reset frame */
					Uart_resetFsm(me, RX);
				}
				break;
			default:
				break;
			}
		}
	} else {
		/* if overflow */
	}
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
} /* end rx Callback */
/* Uart rx TimeOut                    */
/* Check fsm state = fineshed         */
/* Check crc                          */
/* Send to upper layer using rxQueue  */
void Uart_rxTimeout(TimerHandle_t xTimer) {
	Uart* me = (Uart *) pvTimerGetTimerID(xTimer);
	if (me->rxState == finishState) {
		/* get frame ptr */
		uint8_t *buf = me->rxPtr;
		/* get frame size */
		uint8_t index = me->rxIndex; /* number of positions */
		/* if get memory enable rx */
		me->rxPtr = QMPool_get(&me->rxMemPool, 0, 1);
		if (me->rxPtr != NULL) {
			/* reset rx fsm */
			Uart_resetFsm(me, RX);
			/* enable uart ASAP */
			uartCallbackSet(me->uart, UART_RECEIVE, Uart_rxCallback,
					(void*) me);
		} else {
			/* send error */
		}
		/* check crc */
		if (crc8_init()) {
			/* check crc8 */
			uint8_t c8 = Uart_checkCrc8(buf, index);
			/*from crcIndex = 0 to crcIndex = index-2 */
			if (crc8_calc(0, buf, index - 1) == c8) {
				/* rx crc is OK */
				buf[index - 1] = '\0';
				int32_t status = SendToApp(me->rxQueue, &buf)
				;
				if (!status) {
					DebugPrint("Could not send to the Rx queue.\r\n");
				}
			} else {
				/* rx crc is not OK */
				/* send error */
			}
		}
	} else {
		/* rx fsm not finished */
		/* reset frame */
		Uart_resetFsm(me, RX);
	} /* end check crc */
}
/* Uart tx Callback (fsm)   */
/* incoming app packages    */
/* add delimiters '(' & ')' */
/* check uart tx state      */
/* crc8 calc                */
/* send to uart             */
void Uart_txCallback(void * param) {
	Uart * me = (Uart *) param;
	BaseType_t xTaskWokenByReceive = pdFALSE;
	UBaseType_t uxSavedInterruptStatus;
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR( );
	switch (me->txState) {
	case standByState:
		xQueueReceiveFromISR(me->txQueue, (void *) &me->txPtr,
				&xTaskWokenByReceive);
		uartTxWrite(me->uart, '(');
		uartTxWrite(me->uart, me->txPtr[me->txIndex]);
		me->txCrc8 = crc8_calc(0, (me->txPtr + me->txIndex), 1);
		me->txIndex++;
		me->txState = busyState;
		break;
	case busyState:
		uartTxWrite(me->uart, me->txPtr[me->txIndex]);
		me->txCrc8 = crc8_calc(me->txCrc8, (me->txPtr + me->txIndex), 1);
		me->txIndex++;
		if (me->txPtr[me->txIndex] == '\0') {
			me->txState = finishState;
			uartTxWrite(me->uart, Uart_addCrc8 (me->txCrc8,MSB));
			uartTxWrite(me->uart, Uart_addCrc8 (me->txCrc8,LSB));
			uartTxWrite(me->uart, ')');
			uartTxWrite(me->uart, '\r');
			uartTxWrite(me->uart, '\n');
			uartCallbackClr(me->uart, UART_TRANSMITER_FREE);
			xTimerStartFromISR(me->txTimeOut, &xTaskWokenByReceive);
		}
		break;
	case finishState:
		break;
	case errorState:
		break;
	}
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
	if (xTaskWokenByReceive != pdFALSE) {
		taskYIELD ( )
		;
	}
}
/* Uart tx Time Out                      */
/* Check fsm state = fineshed            */
/* uartCallbackSet UART_TRANSMITER_FREE  */
void Uart_txTimeout(TimerHandle_t xTimer) {
	Uart * me = (Uart *) pvTimerGetTimerID(xTimer);
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	if (me->txState == finishState) {
		if (uxQueueMessagesWaiting(me->txQueue)) {
			uartCallbackSet(me->uart, UART_TRANSMITER_FREE, Uart_txTimeout,
					(void *) me);
			uartSetPendingInterrupt(me->uart);
		}
		QMPool_put(&me->txMemPool, me->txPtr, 1);
		/* reset tx fsm */
		Uart_resetFsm(me, TX);
	} else {
		me->txState = errorState;
	}
}
/*=====[Implementations of private functions]================================*/
/* Check incoming uart packages */
uint8_t Uart_checkCrc8(uint8_t * buf, uint8_t index) {
	uint8_t c8LSB = buf[index];
	uint8_t c8MSB = buf[index - 1];
	if (c8LSB > 0x2F && c8LSB < 0x3A) {
		/* is a valid number (0 to 9) */
		c8LSB = c8LSB & 0x0F;
	} else {
		/* another hexa */
		c8LSB = (c8LSB & 0x0F) + 0x09;
	}
	if (c8MSB > 0x2F && c8MSB < 0x3A) {
		/* is a valid number (0 to 9) */
		c8MSB = c8MSB & 0x0F;
	} else {
		/* another hexa */
		c8MSB = (c8MSB & 0x0F) + 0x09;
	}
	/* rx CRC */
	uint8_t c8 = c8LSB | (c8MSB << 4);
	return c8;
}
uint8_t Uart_addCrc8(uint8_t c8, uint8_t val) {
	uint8_t c8LSB ;
	uint8_t c8MSB ;
	uint8_t aux[] = "0123456789ABCDEF";
	if(val == LSB){ 	   /*LSB*/
		c8LSB = c8 & 0x0F;
		return aux[c8LSB];
	}else{				   /*MSB*/
		c8MSB = (c8 >> 4) & 0x0F;
		return aux[c8MSB];
	}
}

/* Reset (fsm) */
void Uart_resetFsm(Uart *me, uint8_t option) {
	switch (option) {
	case RX:
		me->rxIndex = 0;
		/* default state value is standByState     */
		me->rxState = standByState;
		break;
	case TX:
		me->txState = standByState;
		me->txIndex = 0;
		break;
	}
}
