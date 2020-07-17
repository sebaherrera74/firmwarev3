/*
 * Task.c
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include <string.h>
#include "sapi.h"
#include "Layer2.h"
#include "UartDriver.h"
#include "Task.h"
#include "types.h"
/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/


/* Functionality of task used to transmit response for UART */


/* Functionality of task used to receive data for UART */
static void ReceiveMessage(void *taskParmeters);

/* Function to process data's message depend of command received */
static void ProccessData(Layer2_t *message);



/*=====[Implementations of private functions]=================================*/
static void ProccessData(Layer2_t *message)
{
	uint32_t i;

	// Ask if all letter must be on uppercase or lowercase
	switch(message->code)
	{
		case 'M':
		{
			for(i = 1; i < strlen((char *)message->message); i++)
			{
				if ( *(message->message + i) >= 'a' && *(message->message + i) <= 'z' )
				{
					*(message->message + i) -= 32; // Convert to uppercase
				}
			}
		}
		break;

		case 'm':
		{
			for(i = 1; i < strlen((char *)message->message); i++)
			{
				if ( *(message->message + i) >= 'A' && *(message->message + i) <= 'Z' )
				{
					*(message->message + i) += 32; // Convert to lowercase
				}
			}
		}
		break;

		default:
		{
			strncpy(message->message, "Command incorrect", strlen("Command incorrect"));
		}
	}
}

static void ReceiveMessage(void *taskParmeters)
{
	UartDriver_t *uart = (UartDriver_t *)taskParmeters;
	Layer2_t msg;
	uint32_t length;
	uint32_t index;
	while(TRUE)
	{
		xQueueReceive(uart->onRxQueue, &msg, portMAX_DELAY);

		ProccessData(&msg);

		length = strlen((char*)msg.message);

		if(uart->dataTx == NULL)
			uart->dataTx = (Message_t *)QMPool_get(memoryBlock, 0);//pedimos memoria

		/*uart->dataTx[0] = msg.code;
		uart->dataTx++;

		for(index = 0; index < length; index++)
		{
			*(uart->dataTx + index) = *(msg.message + index);
		}*/

		//strncpy((char*)uart->dataTx, (char*)msg.message, strlen((char*)msg.message) );


		/* Send message for interruption UART */
		taskENTER_CRITICAL();  // Avoid modify txCounter

		uart->txLength = strlen(uart->dataTx);
		// If send for UART happen so I don't send enything
		if ( uart->txCounter == 0 )
		{
			txInterruptEnable(uart);
		}
		taskEXIT_CRITICAL();

		uartSetPendingInterrupt(uart->typeUart);

		Layer2_destructor(&msg, HANDLER_MODE_OFF);
	}
}


void alive_task(void* taskParmPtr) {
	TickType_t xPeriodicity = 250 / portTICK_RATE_MS;	// Tarea periodica cada 250ms
	while (TRUE) {
		gpioToggle(LEDB);
		vTaskDelay(xPeriodicity);
	}
}


uint8_t rxUART_Start(void *parameters)
{
	if ( xTaskCreate(ReceiveMessage, "rxUART",2*configMINIMAL_STACK_SIZE, parameters, tskIDLE_PRIORITY+2, 0) == pdFAIL)
	{
		return 0;
	}

	return 1;
}

uint8_t Alive_Task(void *parameters)
{
	if ( xTaskCreate(alive_task, "alive_task",2*configMINIMAL_STACK_SIZE, parameters, tskIDLE_PRIORITY+2, 0) == pdFAIL)
	{
		return 0;
	}

	return 1;
}

