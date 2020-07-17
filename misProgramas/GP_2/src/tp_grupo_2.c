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
/*=====[Inclusions of function dependencies]=================================*/
#include "tp_grupo_2.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/* App & Driver */
#include "sapi.h"

#include "FreeRTOSConfig.h"
#include "tp_uart.h"
#include "tp_uart_config.h"
#include "userTasks.h"
/*=====[Definition macros of private constants]==============================*/
/*=====[Definitions of extern global variables]==============================*/
/*=====[Definitions of public global variables]==============================*/
/*=====[Definitions of private global variables]=============================*/
/*=====[Main function, program entry point after power on or reset]==========*/
int main(void) {
	static Uart xUart; /* multiple instances of Uart */

	/*static Uart yUart;*/
	boardInit();
	/* Call uart constructor  */
	/* sapi-uart init         */
	/* freertos queues create */
	/* freertos timers create */
	/* QMPool object create   */
	int32_t result = Uart_ctor(&xUart, UART_USB, BAUD_RATE);
	if (result == OK) {
		/* enable uart rx & tx interrups */
		Uart_init(&xUart);
		BaseType_t xReturnedTickTask;
		BaseType_t xReturnedApp;
		/* Heartbeat */
		xReturnedTickTask = xTaskCreate(tickTask, (const char *) "tickTask", /* Text name for the task. */
		configMINIMAL_STACK_SIZE * 2, /* Stack size in words, not bytes. */
		(void *) result, /* Heartbeat - Frequency in milliseconds. */
		tskIDLE_PRIORITY + 1, /* Priority at which the task is created. */
		0 /* Pointer to the task created in the system */
		);
		/* C3 layer */
		xReturnedApp = xTaskCreate(app, /* App that capitalize or minuscule. */
		(const char *) "c3 layer", /* Text name for the task. */
		configMINIMAL_STACK_SIZE * 2, /* Stack size in words, not bytes. */
		(void *) &xUart, /* Parameter passed into the task. */
		tskIDLE_PRIORITY + 2, /* Priority at which the task is created. */
		0 /* Pointer to the task created in the system */
		);/* parse error work in progress */
		if (xReturnedTickTask == pdPASS && xReturnedApp == pdPASS)
			/* The task was created. */
			vTaskStartScheduler(); /* Initialize scheduler */
	} else {
		/* error parser work in progress */
		BaseType_t xReturnedErrorParser;
		xReturnedErrorParser = xTaskCreate(tickTask, (const char *) "tickTask", /* Text name for the task. */
		configMINIMAL_STACK_SIZE * 2, /* Stack size in words, not bytes. */
		(void *) result, /* error */
		tskIDLE_PRIORITY + 1, /* Priority at which the task is created. */
		0 /* Pointer to the task created in the system */
		);
		if (xReturnedErrorParser == pdPASS) {
			vTaskStartScheduler(); /* Initialize scheduler */
		}
	}

	for (;;)
		; // If reach heare it means that the scheduler could not start

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}
