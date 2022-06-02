
#include "capa2.h"
#include <stdlib.h>
#include <string.h>


/*=====[Prototypes (declarations) of private functions]======================*/

uint8_t  crc8_check(tMensaje* data, uint8_t len);


/*=====[Implementations of public functions]=================================*/

void appC2Rx(driver_t* Uart_driver)
{

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	tMensaje* p = NULL;


	 //control de longitud de mensaje   <----------------------------------------------------------------------					R_C2-4
	if (Uart_driver->rxLen < LARGO)
	{
		if((Uart_driver->dato_rx[0]==SOM)&&(Uart_driver->dato_rx[Uart_driver->rxLen-1]==EOM))
		{
			if(crc8_check(Uart_driver->dato_rx, Uart_driver->rxLen))//<---------------------------------------------------------R_C2-9
			{
				 Uart_driver->dato_rx[Uart_driver->rxLen-3] = '\0';//retiro el EOM y el checksum

				 p = &(*(Uart_driver->dato_rx + 1));//retiro el SOM

				 // Se escribe en la cola el buffer completo y a su vez avisamos a la aplicación de que un paquete correcto se debe procesar.
				 xQueueSend(Uart_driver->onRxQueue, &p,&xHigherPriorityTaskWoken);

				 //Una vez que el bloque esta en la cola pido otro bloque para el siguiente paquete. Es responsabilidad de
		         //la aplicacion liberar el bloque mediante una transmision o con la funcion putBuffer()
				 Uart_driver->dato_rx = (tMensaje*) QMPool_get(&Uart_driver->Pool_memoria,0); //pido un nuevo bloque del pool R_C2-5

				 //Chequeo si tengo un bloque de memoria, sino anulo la recepcion de paquetes
		         if ( Uart_driver->dato_rx  == NULL )//<--------------R_C2-6
		         {
		        	 uartCallbackClr( Uart_driver->uartValue, UART_RECEIVE );
		         }
			}
			else
			{
				imprimir(MENSAJENOACK);//mensaje al emisor informando que no esta bien el ack
			}
		}
		else
		{
			imprimir("ERROR EOM O SOM");
			Uart_driver->rxLen=0;
		}


	}
	else
	{

		 imprimir(MENSAJEOVERFLOW);//mensaje al emisor informando que supera la longitud
	}

	Uart_driver -> txLen = Uart_driver -> rxLen - 4;
	Uart_driver->rxLen=0;

}

void appC2Tx(driver_t* Uart_driver)
{
	    BaseType_t xTaskWokenByReceive = pdFALSE;

		uint8_t crcCalculated = 0;
		volatile uint8_t seed_crc = 0;//valor inicial para el crc
		char crcAux[3] = {0};
		uint8_t crcCalH = 0;
		uint8_t crcCalL = 0;
		static char num[] = "0123456789ABCDEF";

		 // Si el contador es 0 -> puedo tomar un paquete de la cola
		 if ( Uart_driver -> txCounter == 0){

			 uartTxWrite(Uart_driver->uartValue, '(');
			 uartTxWrite(Uart_driver->uartValue, Uart_driver->dato_tx[Uart_driver->txCounter]);

			  // Calculo el CRC y lo pongo al final
			 seed_crc = 0;
			 crcCalculated = crc8_calc(seed_crc, Uart_driver->dato_tx, Uart_driver->txLen);//calculo el crc apartir de la trama
			 crcCalH = crcCalculated / 16;
			 crcCalL = crcCalculated % 16;
			 crcAux[1] = num[crcCalL];
			 crcAux[2] = num[crcCalH];

			 //	Pongo el CRC  al final
			 Uart_driver->dato_tx[Uart_driver->txLen] = crcAux[0];
			 Uart_driver->dato_tx[Uart_driver->txLen+1] = crcAux[1];
			 Uart_driver->dato_tx[Uart_driver->txLen+2] = ')';
			 Uart_driver->dato_tx[Uart_driver->txLen+3] = '\r';
			 Uart_driver->dato_tx[Uart_driver->txLen+4] = '\n';

			 Uart_driver->txCounter ++;

		 }else if( Uart_driver->txCounter <= Uart_driver->txLen + 5) //	Si no llegué al final de la cadena transmito el dato
		 {

			uartTxWrite(Uart_driver->uartValue, Uart_driver->dato_tx[Uart_driver->txCounter]);
			Uart_driver->txCounter ++;

		 }else
		 {
			 //	Cuando termino de transmitir todos los bytes tengo que deshabilitar la interrupción e iniciar el timeout.
			 uartCallbackClr(Uart_driver->uartValue, UART_TRANSMITER_FREE); // Deshabilito la interrupcion de Tx
			 xTimerStartFromISR( Uart_driver->onTxTimeOut, &xTaskWokenByReceive ); //Inicio el timer de Timeout

		 }
}



uint8_t  crc8_check(tMensaje* data, uint8_t len){

	uint8_t retVal = FALSE;
	char crcAux[3] = {0};
	volatile uint8_t seed_crc = 0;//valor inicial para el crc
	uint8_t crcReceived = 0;
	uint8_t crcCalculated = 0;

	strncpy(crcAux, data + len - 3, 2);//los 2 bytes que correponden al crc

	crcReceived = (uint8_t)strtoul(crcAux, NULL, 16);//transformo los dos bytes a uint8_t
	crcCalculated = crc8_calc(seed_crc, data + 1 , len - 4);//calculo el crc apartir de la trama

	if(crcReceived == crcCalculated)//comparo el valor calculado con el recibido en la trama
		retVal = TRUE;
	else
		retVal = FALSE;

	return retVal;
}



