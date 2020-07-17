#include "FreeRTOS.h"
#include "task.h"

#include "tipos.h"

void post(tObjeto* objeto,char* mensaje)
{
	xQueueSend( objeto->cola, mensaje, portMAX_DELAY );
}

void get(tObjeto* objeto,char* mensaje)
{
	xQueueReceive( objeto->cola, mensaje ,portMAX_DELAY );
}

void imprimir(char* mensaje)
{
	uartWriteString( UART_USB, mensaje );
	uartWriteString( UART_USB, "\r\n" );
}
