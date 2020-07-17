/*
 * UartDriver.c
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */

#include "UartDriver.h"
#include "types.h"

/* ----------- Global private variable ----------- */
#define QUEUE_SIZE		10

/* -------- Prototype of private function -------- */

/**
 * @brief Callback function for transmission interrupt.
 */
static void onTxCallback(void *parameters);

/**
 * @brief Callback function for reception interrupt.
 */
static void onRxCallback(void *parameters);

/**
 * @brief Setup interruption to receive data for UART.
 *
 * @param[in] uart Pointer of UartDriver_t that contain information about
 * 				   which is the UART selected.
 *
 * \return Return 1 is operation was success or 0 in other case.
 */
static uint8_t RxInterrup_Init(UartDriver_t *uart);
static void vTimerCallback(xTimerHandle pxTimer);

/* ------ Implementation of private function ----- */
static void onTxCallback(void *parameters) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UartDriver_t *uart;

	uart = (UartDriver_t *) parameters;

	xHigherPriorityTaskWoken = taskENTER_CRITICAL_FROM_ISR();
	// Message to UART
	if (uart->txCounter < uart->txLength) {
		uartTxWrite(uart->typeUart, *(uart->dataTx + uart->txCounter));
		uart->txCounter++;
	}

	// Is final of message?
	if (uart->txCounter == uart->txLength) {
		uartTxWrite(uart->typeUart, '\r');
		uartTxWrite(uart->typeUart, '\n');
		uartCallbackClr(uart->typeUart, UART_TRANSMITER_FREE); // Disable interruption for Tx
		//Layer2_destructor(&message, HANDLER_MODE_ON);
		QMPool_put_fromISR(memoryBlock, (Message_t *) uart->dataTx);
		uart->dataTx = NULL;
		uart->txLength = 0;
		uart->txCounter = 0;
	}

	taskEXIT_CRITICAL_FROM_ISR(xHigherPriorityTaskWoken);

	// Evaluate asked for of necessary change context
	if (xHigherPriorityTaskWoken != pdFALSE) {
		taskYIELD()
		;
	}
}

static void onRxCallback(void *parameters) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UartDriver_t *uart;
	uint8_t character, finish = 0;
	Layer2_t message;
	message.message = NULL;

	uart = (UartDriver_t *) parameters;

	// Evaluate if we need ask block memory
	if (uart->dataRx == NULL) {
		uart->dataRx = (Message_t *) QMPool_get_fromISR(memoryBlock, 0);

		if (uart->dataRx == NULL) {
			uartWriteString(uart->typeUart, "Insufficient memory\r\n");
			uart->rxLength = SIZE_PACKET;
		}
	}

	/*
	 * Evaluate length of byte received, if bigger than we can storage,
	 * if that so we don't storage it
	 */
	if (uart->rxLength < SIZE_PACKET) {
		// Read data
		character = uartRxRead(uart->typeUart);

		switch (character) {
		// First character must be '('
		case '(': {
			if (uart->rxLength == 0) {
				*(uart->dataRx + uart->rxLength++) = character;	// Store character '(' on string
			} else {
				uart->rxLength = 0;				// Reset index of vector message
				*(uart->dataRx + uart->rxLength++) = character;	// Store character '(' on string
			}
			xTimerStartFromISR(uart->onRxTimeOut, &xHigherPriorityTaskWoken);
		}
			break;

		case ')': {
			*(uart->dataRx + uart->rxLength++) = character; // Store character ')' on string
			*(uart->dataRx + uart->rxLength) = '\0';// close string with character null
			finish = 1;
			xTimerStopFromISR(uart->onRxTimeOut, &xHigherPriorityTaskWoken);
		}
			break;

		default: {
			if (uart->rxLength != 0) {
				*(uart->dataRx + uart->rxLength++) = character;	// Storage character on string
			}
			xTimerResetFromISR(uart->onRxTimeOut, &xHigherPriorityTaskWoken);
		}
		}
	} else {
		uart->rxLength = 0;
	}

	if (finish == 1) {
		if (Layer2_constructor(&message, HANDLER_MODE_ON)) {
			if (!Deserialize_ParseMessage(&message, uart->dataRx)) {
				Layer2_destructor(&message, HANDLER_MODE_ON);
				uartWriteString(uart->typeUart, "Error of message\r\n");
			} else {
				QMPool_put_fromISR(memoryBlock, (Message_t *) uart->dataRx);
				uart->dataRx = NULL;
				xQueueSendFromISR(uart->onRxQueue, &message,
						&xHigherPriorityTaskWoken);

			}
		}
		uart->txLength = 0;
		uart->rxLength = 0;
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static uint8_t RxInterrup_Init(UartDriver_t *uart) {

	// Setup callback function to attend reception interrupt
	switch (uart->typeUart) {
	case UART_GPIO:
	case UART_USB:
	case UART_232: {
		uartCallbackSet(uart->typeUart, UART_RECEIVE, onRxCallback,
				(void*) uart);
	}
		break;

	default:
		return 0; // Uart selection is incorrect

	}

	return 1;
}

/* ------ Implementation of public function ------ */
uint8_t UartDriver_Init(UartDriver_t *uart) {
	// Initialization of UART
	uartInit(uart->typeUart, uart->baudRate);

	uart->txCounter = 0;
	uart->txLength = 0;
	uart->rxLength = 0;

	uart->onTxQueue = xQueueCreate(QUEUE_SIZE, sizeof(Layer2_t));

	if (uart->onTxQueue == NULL) {
		return 0;
	}

	uart->onRxQueue = xQueueCreate(QUEUE_SIZE, sizeof(Layer2_t));

	if (uart->onRxQueue == NULL) {
		return 0;
	}

	//timer config
	uart->onRxTimeOut = xTimerCreate("timer", /* name */
						pdMS_TO_TICKS(50), /* period/time */
						pdFALSE, /* auto reload */
						(void*)uart, /* timer ID */
						vTimerCallback); /* callback */

	if (uart->onRxTimeOut == NULL) {
		return 0;
	}

	// Setup RX Interruption
	if (!RxInterrup_Init(uart)) {
		return 0;
	}

	// Enabled interruption UART
	uartInterrupt(uart->typeUart, TRUE);

	return 1;
}

static void vTimerCallback(xTimerHandle pxTimer) {
	UartDriver_t *uart = (UartDriver_t *) pvTimerGetTimerID(pxTimer);
	taskENTER_CRITICAL();
	uart->rxLength = 0;
	taskEXIT_CRITICAL();
}

uint8_t txInterruptEnable(UartDriver_t *uart) {
	switch (uart->typeUart) {
	case UART_GPIO:
	case UART_USB:
	case UART_232: {
		uartCallbackSet(uart->typeUart, UART_TRANSMITER_FREE, onTxCallback,
				(void*) uart);
		break;
	}
	default: {
		return 1; // UART selected was incorrect
	}
	}

	return 1;
}
