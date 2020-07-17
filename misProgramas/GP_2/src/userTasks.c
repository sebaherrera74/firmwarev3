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
#include "userTasks.h"

#include "tp_grupo_2.h"
#include "tp_uart.h"
#include "tp_uart_config.h"
/*=====[Definition macros of private constants]==============================*/
/*=====[Private function-like macros]========================================*/
/*=====[Definitions of private data types]===================================*/
/* Active Object message's attributes */
typedef struct {
	QueueHandle_t aoBuffer; /* for ao events */
	Message rxMessage; /* ptr to rxMessage */
	Message txMessage; /* ptr to txMessage */
} AoMessage;
/* for AO callback */
typedef void (*tp_callback_t)(AoMessage*);
/* Active Object's attributes */
typedef struct {
	TaskFunction_t taskPtr;
	QueueHandle_t aoQueue;
	tp_callback_t aoCallback; /* callback Active Object */
	uint8_t aoActive; /* Active or Inactive */
} Ao;
/*=====[Definitions of external public global variables]=====================*/
/*=====[Definitions of public global variables]==============================*/
/*=====[Definitions of private global variables]=============================*/
/*=====[Prototypes (declarations) of private functions]======================*/
/* Ao's functions */
/* Constructor */
void Ao_ctor(Ao * const me);
/* Create AO */
uint8_t Ao_create(Ao * const me, tp_callback_t aoCallback, TaskFunction_t);
/* AO task */
void Ao_task(void* taskParmPtr);
/* Send rx & tx messages to AO */
void Ao_sendToBuffer(Ao* me, AoMessage* aomessage);
/* AoMessages's functions */
/* C3 Error parser  */
uint8_t Ao_error(AoMessage* me,  uint8_t position);
/* app functions */
/* Comand "C" verifier */
uint8_t appTask(Message);
/* aoCallbacks */
void capitalize(AoMessage* me);
void minuscule(AoMessage* me);
/*=====[Implementations of public functions]=================================*/
void tickTask(void * taskParmPtr) {
	int32_t error = (int32_t *) taskParmPtr;
	vTaskDelay(1000 / portTICK_RATE_MS);
	static gpioMap_t tp_led;
	switch (error) {
	case 0:
		tp_led = LED3; /* Uart_ctor OK */
		break;
	case -1:
		tp_led = LED2; /* QUEUE_ERROR */
		break;
	case -2:
		tp_led = LED1; /* TIMER_ERROR */
		break;
	default:
		tp_led = LEDR; /* Default ERROR */
		break;
	}
	gpioWrite(tp_led, ON);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpioWrite(tp_led, OFF);
	TickType_t xPeriodicity = (1000) / portTICK_RATE_MS;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (TRUE) {
		gpioToggle(tp_led);
		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
	}
}
void app(void * taskParmPtr) {
	Uart *xUart;
	xUart = (Uart *) taskParmPtr;
	BaseType_t xStatus;
	uint8_t events;
	/* Multiples instances of Ao */
	Ao minuAo;
	Ao capiAo;
	/* constructor Ao */
	Ao_ctor(&minuAo);
	Ao_ctor(&capiAo);
	static AoMessage aomessage;
	aomessage.aoBuffer = xQueueCreate(QUEUE_SIZE, sizeof(AoMessage));
	if (aomessage.aoBuffer != NULL) {
		/* Task repeat for ever */
		while ( TRUE) { /* while true */
			/* for debug purposes remove for stable version */
			const TickType_t xTicksToWait = pdMS_TO_TICKS(5050UL);
			if (uxQueueMessagesWaiting(xUart->rxQueue) != 0) {
				vPrintString("Queue should have been empty!\r\n");
			}
			xStatus = xQueueReceive(xUart->rxQueue,
					(void *) &aomessage.rxMessage, portMAX_DELAY);
			if (xStatus == pdPASS) { /* if queue receive */
				/* Data was successfully received from the queue. */
				aomessage.txMessage = QMPool_get(&xUart->txMemPool, 0, 0);
				if (aomessage.txMessage != (void *) 0) { /* if ptr create */
					uint8_t result = appTask(aomessage.rxMessage);
					uint8_t created = 0;
					switch (result) {
					case 'M':
						if (!capiAo.aoActive) {
							created = Ao_create(&capiAo, capitalize, Ao_task);
						}
						if (created)
							Ao_sendToBuffer ( &capiAo, &aomessage );
						break;
					case 'm':
						if (!minuAo.aoActive) {
							created = Ao_create(&minuAo, minuscule, Ao_task);
						}
						if (created)
							Ao_sendToBuffer ( &minuAo, &aomessage );
						break;
					default:
						created = Ao_error(&aomessage,0);
						break;
					}
					if (xQueueReceive(aomessage.aoBuffer, &aomessage,
							(TickType_t) 1)) {
						QMPool_put(&xUart->rxMemPool, aomessage.rxMessage, 1);
						xStatus = xQueueSendToBack(xUart->txQueue,
								(void * ) &aomessage.txMessage, portMAX_DELAY);
						if (xStatus != pdPASS) { /* if queue send */
							vPrintString("Could not send to the Tx queue.\r\n");
						} /* end if queue send */
						taskENTER_CRITICAL();
						if (xUart->txState == standByState) {
							uartCallbackSet(xUart->uart, UART_TRANSMITER_FREE,
									Uart_txCallback, (void*) xUart);
						}
						taskEXIT_CRITICAL();
						uartSetPendingInterrupt(xUart->uart);
					}
				} /* end if ptr create */
				/* end if queue receive */
			} else { /* else */
				vPrintString("Could not receive from Rx the queue.\r\n");
			} /* end else */
		} /* while true */
	}
}
/*=====[Implementations of interrupt functions]==============================*/
/*=====[Implementations of private functions]================================*/
void Ao_ctor(Ao * const me) {
	me->aoActive = false;
}
uint8_t Ao_create(Ao * const me, tp_callback_t aoCallback, TaskFunction_t name) {
	me->aoQueue = xQueueCreate(QUEUE_SIZE, sizeof(AoMessage));
	me->taskPtr = name;
	BaseType_t xStatus;
	if (me->aoQueue != NULL) {
		me->aoCallback = aoCallback;
		xStatus = xTaskCreate(me->taskPtr, (const char *) "oa Task",
		configMINIMAL_STACK_SIZE * 2, me,
		tskIDLE_PRIORITY + 2, 0);
	}
	if (xStatus == pdPASS) {
		me->aoActive = true;
		return (true);
	} else {
		return (false);
	}
}
void Ao_sendToBuffer(Ao * const me, AoMessage * aomessage) {
	xQueueSend(me->aoQueue, aomessage, 0);
}
/* function that is called when new ao is created */
void Ao_task(void* taskParmPtr) {
	Ao* me = (Ao*) taskParmPtr;
	AoMessage aomessage;
	BaseType_t xStatus;
	while ( TRUE) {
		if (uxQueueMessagesWaiting(me->aoQueue)) {
			xStatus = xQueueReceive(me->aoQueue, &aomessage, portMAX_DELAY);
			if (xStatus == pdPASS) {
				(me->aoCallback)(&aomessage);
			}
		} else {
			me->aoActive = false;
			vQueueDelete(me->aoQueue);
			vTaskDelete( NULL);
		}
	}
}
uint8_t appTask(Message rxMessage) {
	if (rxMessage[0] == 'M') {
		return rxMessage[0];
	} else if (rxMessage[0] == 'm') {
		return rxMessage[0];
	} else
		return 0; /* see error treatment */
}
void capitalize(AoMessage* me) {
	me->txMessage[0] = me->rxMessage[0];
	uint8_t aux = 0;
	uint8_t err = 0;
	for (uint8_t i = 1; me->rxMessage[i] != '\0'; i++) {
		if ((me->rxMessage[i] >= 'a') && (me->rxMessage[i] <= 'z')) {
			me->txMessage[i] = me->rxMessage[i] + ('A' - 'a');
			aux = i;
		} else if ((me->rxMessage[i] >= 'A') && (me->rxMessage[i] <= 'Z')) {
			me->txMessage[i] = me->rxMessage[i];
			aux = i;
		} else {
			me->rxMessage[i + 1] = '\0'; /* must quit for */
			err = Ao_error(me,i);		/* sent error  */
		}
	}
	if (err != true) {
		me->txMessage[aux + 1] = '\0';
		xQueueSend(me->aoBuffer, me, 0);
	}
}
void minuscule(AoMessage* me) {
	me->txMessage[0] = me->rxMessage[0];
	uint8_t aux = 0;
	uint8_t err = 0;
	for (uint8_t i = 1; me->rxMessage[i] != '\0'; i++) {
		if ((me->rxMessage[i] >= 'A') && (me->rxMessage[i] <= 'Z')) {
			me->txMessage[i] = me->rxMessage[i] - ('A' - 'a');
			aux = i;
		} else if ((me->rxMessage[i] >= 'a') && (me->rxMessage[i] <= 'z')) {
			me->txMessage[i] = me->rxMessage[i];
			aux = i;
		} else {
			me->rxMessage[i + 1] = '\0'; /* must quit for */
			err = Ao_error(me,i);		/* sent error  */
		}
	}
	if ( err != true) {
		me->txMessage[aux + 1] = '\0';
		xQueueSend(me->aoBuffer, me, 0);
	}
}
uint8_t Ao_error(AoMessage* me,  uint8_t position){
	/* Error En */
	uint8_t * Error = "ErrorEn";
	strcpy(me->txMessage,Error);
	me->txMessage[7] = me->rxMessage[position];
	me->txMessage[8] = '\0';
	/* Sent queue ao */
	xQueueSend(me->aoBuffer, me, 0);
	return true;
}





