/*
 * UartDriver.h
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */

#ifndef PROGRAMS_RTOS2_INC_UARTDRIVER_H_
#define PROGRAMS_RTOS2_INC_UARTDRIVER_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

#include "sapi.h"
#include "Layer2.h"
#include "qmpool.h"

#define SIZE_PACKET			50
#define QUANTITY_BLOCK		5
#define SIZE_MEMORY_POOL	SIZE_PACKET*QUANTITY_BLOCK


typedef struct
{
    uartMap_t typeUart; 			/**< Number of UART used */
    uint32_t baudRate; 				/**< Baud rate of UART */
    TimerHandle_t onRxTimeOut; 		/**< Timeout used to reception */
    Message_t *dataTx; 				/*< Block of memory used to transmit */
    Message_t *dataRx; 				/*< Block of memory used to receive */
    uint8_t txCounter; 				/*< Counter of byte transmitted */
    uint8_t txLength; 				/*< Length of packet to transmit */
    uint8_t rxLength; 				/*< Length of packet received */
    //Layer2_t layer;					/*< Object of layer 2 used to verify message */
    //QMPool *memoryBlock;			/*< Pointer to object used to allocate and release block of memory dynamic */
    //SemaphoreHandle_t wakeup;		/*< Enabled task used to process message */
    QueueHandle_t onTxQueue; 		/*< Transmition queue */
    QueueHandle_t onRxQueue; 		/*< Reception queue */

}UartDriver_t;


/**
 * @brief Initialization of UART and interruption to receive data
 *
 * @param[in] uart Pointer to structure where set all parameters need to received
 *
 * \return Return 1 is operation was success or 0 in other case.
 */
uint8_t UartDriver_Init(UartDriver_t *uart);


/**
 * @brief Initialization of interruption UART for transmit data
 *
 * @param[in] uart Pointer to structure where set all parameters needed to transmit
 *
 * \return Return 1 is operation was success or 0 in other case.
 */
uint8_t txInterruptEnable(UartDriver_t *uart);

#endif /* PROGRAMS_RTOS2_INC_UARTDRIVER_H_ */
