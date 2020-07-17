/*
 * main.c
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */

/*=====[Inclusions of function dependencies]=================================*/

#include "main.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "qmpool.h"
#include "UartDriver.h"
#include "Task.h"
#include "types.h"
/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
Message_t *memory1 = NULL;	 // Segment of memory used to memory pool library
QMPool memoryPool1;	 	 	 // Memory pool (necessary to library);
Message_t *memory2 = NULL;	 // Segment of memory used to memory pool library
QMPool memoryPool2;	 	 	 // Memory pool (necessary to library);
/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	volatile static UartDriver_t uart1;

	boardInit();

	// Initialization of UART to transmit and receive for ISR
	uart1.typeUart = UART_USB;
	uart1.baudRate = 115200;

	if (!UartDriver_Init((UartDriver_t *)&uart1))
	{
		gpioWrite(LEDR, TRUE);
		while(1);
	}

	// Allocate memory for memory pool
	memory1 = (Message_t *)pvPortMalloc(SIZE_MEMORY_POOL*sizeof(Message_t));

	if (memory1 == NULL)
	{
		gpioWrite(LEDG, TRUE);
		while(1);
	}

	// Create memory pool used on task and UART
	QMPool_init(&memoryPool1, (Message_t *)memory1, SIZE_MEMORY_POOL*sizeof(Message_t), SIZE_PACKET);

	// Block of memory UART point of memory pool
	memoryBlock = &memoryPool1;

	// Allocate memory for memory pool
	memory2 = (Message_t *)pvPortMalloc(SIZE_MEMORY_POOL*sizeof(Message_t));

	if (memory2 == NULL)
	{
		gpioWrite(LEDG, TRUE);
		while(1);
	}

	// Create memory pool used on task and UART
	QMPool_init(&memoryPool2, (Message_t *)memory2, SIZE_MEMORY_POOL*sizeof(Message_t), SIZE_PACKET);

	// Block of memory layer2 contructor point of memory pool
	memoryBlock_aux = &memoryPool2;

	// Creation task
	if (!rxUART_Start((void *)&uart1))
	{
		gpioWrite(LEDB, TRUE);
		while(1);
	}


	// Creation task
	if (!Alive_Task(0))
	{
		gpioWrite(LEDB, TRUE);
		while(1);
	}

	vTaskStartScheduler(); // Initialize scheduler

	while( true ); // If reach heare it means that the scheduler could not start

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}
