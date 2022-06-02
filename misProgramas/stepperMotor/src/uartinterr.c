#include "uartinterr.h"

//Interrupción por la recepción de un carácter
void onRx(void *nosado) {

	static uint8_t index = 0;
	char command;
	if (buffer != BUFFER_FULL) {

		command = uartRxRead(UART_232);
		uartWriteByte(UART_232,command);
		//printf("Se recibio :%c\n", command);
		bufferRx[index] = command;

		if (bufferRx[index] == '\n') {
			bufferRx[index + 1] = '\0';
			index = 0;
			buffer = BUFFER_FULL;
		} else {
			index++;
			buffer = BUFFER_INCOMPLETE;
		}
	} else {
		printf("El buffer de recepcion se encuentra lleno\n");
	}

}
void configuraUart(void) {
	//configuro el Uart 232 con interrupción de recepción
	uartConfig(UART_232, BUAD_RATE_9600);
	uartCallbackSet(UART_232, UART_RECEIVE, onRx, NULL);
	uartInterrupt(UART_232, TRUE);

	//configuro uart_USB
	uartConfig(UART_USB, BUAD_RATE_115200);

}

