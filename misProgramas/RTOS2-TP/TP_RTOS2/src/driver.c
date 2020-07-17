/*=============================================================================
 * Copyright (c) 2019, Gonzalo Gontad <gonzalogontad@gmail.com>
 * All rights reserved.
 * License: MIT (see LICENSE.txt)
 * Date: 2019/09/29
 * Version: v1.0
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "driver.h"
#include "uartIRQ.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Inicializacion de parte de transmision del driver
// Requiere que se le pase una variable del tipo driverConfig_t
// con los siguientes datos:
// 	selectedUart.uartValue = (por ej:UART_USB);
//	selectedUart.baudRate = (por ej: 115200);
bool_t driverInit( driver_t* Uart_driver )
{
   // Inicializamos la UART,funcion de la sapi
   uartConfig(Uart_driver->uartValue,Uart_driver->baudRate);

   //Aqui irian los timers de los otros requerimientos

   //Creamos una cola para señalizar la recepcion rx del dato

   Uart_driver->onRxQueue=xQueueCreate(POOL_TOTAL_BLOCKS,sizeof(char*));

   //Creamos una cola para señalizar la transmision tx del dato

    Uart_driver->onTxQueue=xQueueCreate(POOL_TOTAL_BLOCKS,sizeof(char*));

   //Reservar memoria para el Pool
    Uart_driver->txPoolSto=(char*)pvPortMalloc(POOL_SIZE *sizeof(char));
   //Creo el pool de memoria a utilizar
    QMPool_init(&Uart_driver->txPool,(void*)Uart_driver->txPoolSto,POOL_SIZE *sizeof(char),BLOCK_SIZE);

    //chequeo que se crearon las colas y la memoria para el pool correctamente

    if (( Uart_driver->onRxQueue==NULL)||
    	( Uart_driver->onTxQueue==NULL)||
		(Uart_driver->txPoolSto==NULL))
		{
			return 0;
		}

    Uart_driver->txCounter  = 0;	// ya transmiti
	Uart_driver->rxLen      = 0;    //
	Uart_driver->txLen      = 0;	// tengo que tx
    Uart_driver->rxChecksum = 0;

    //Asigno memoria para la primera recepcion de la UART
    Uart_driver->dato_rx=(char*)QMPool_get(&Uart_driver->txPool,0);

	// Configuramos el cb de rx
	if (( rxInterruptEnable( Uart_driver )))
	{}
	else
	{
		return FALSE;
	}

	// NO OLVIDAR: Habilitar interrupciones
	uartInterrupt(Uart_driver -> uartValue, TRUE);

	return TRUE;
}

//Transmite el bloque txblock por la UART indicada

void packetTX( driver_t* Uart_driver, char *txBlock )
{
	//mandar por cola
    xQueueSend( Uart_driver->onTxQueue, &txBlock, portMAX_DELAY ); //Envio a la cola de transmision el blocke a transmitir
    taskENTER_CRITICAL();  //no permito que se modifique txcounter

    if ( Uart_driver->txCounter == 0 ) //si se esta enviando algo no llamo a la interrupcion para no interrumpir el delay
    {
        txInterruptEnable( Uart_driver );  //habilito la transmision para la transmision
    }
    taskEXIT_CRITICAL();
    uartSetPendingInterrupt( Uart_driver->uartValue );
}
//Se queda esperando un paquete de la uart seleccionada y devuelve un puntero al bloque de memoria
char* waitPacket	( driver_t* Uart_driver){
	char * rxBlock;
	xQueueReceive(Uart_driver->onRxQueue,(void*)&rxBlock,portMAX_DELAY);
	return rxBlock;

}



