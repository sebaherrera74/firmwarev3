
#include "driver.h"

// Inicializacion de parte de transmision del driver
bool_t driverInit( driver_t* Uart_driver ){

    //Inicializacion del driver de UART

	Uart_driver->uartValue = UART_USB;
    Uart_driver->baudRate = 115200;

	uartConfig(Uart_driver->uartValue,Uart_driver->baudRate);	// Inicializamos la UART

	Uart_driver->txCounter  = 0;	// ya transmiti
	Uart_driver->rxLen      = 0;
	Uart_driver->txLen      = 0;	// tengo que tx

	Uart_driver->onTxQueue = xQueueCreate(POOL_TOTAL_BLOCKS,sizeof(tMensaje *));

	if( Uart_driver->onTxQueue == NULL){
			gpioWrite( LEDR, ON );
			imprimir( "Error al crear la cola." );
			while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

	Uart_driver->onRxQueue= xQueueCreate(POOL_TOTAL_BLOCKS,sizeof(tMensaje * ));

	if( Uart_driver->onRxQueue == NULL){
			gpioWrite( LEDR, ON );
			imprimir( "Error al crear la cola." );
			while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

    // Creamos el timer que cumplirá la función de time out y poder separar los paquetes.

    //Uart_driver->onTxTimeOut = xTimerCreate( "TX Time Out", PROTOCOL_TIMEOUT,pdFALSE, ( void* ) Uart_driver, onTxTimeOutCallback );
  Uart_driver->onRxTimeOut = xTimerCreate ("RX Time Out", PROTOCOL_TIMEOUT,
					   pdFALSE, (void*) Uart_driver,
					   onRxTimeOutCallback);

	// Configuramos el cb de rx
	if (( rxInterruptEnable( Uart_driver )))
	{}
	else{
		return FALSE;
	}

     //	Reservo memoria para el memory pool
     Uart_driver->Pool_puntero = ( tMensaje* ) pvPortMalloc( POOL_SIZE * sizeof( tMensaje ) );

     //	Creo el pool de memoria que va a usarse para la transmision
     QMPool_init( &(Uart_driver->Pool_memoria), ( tMensaje* ) Uart_driver->Pool_puntero,POOL_SIZE*sizeof( tMensaje ),PACKET_SIZE ); //Tamanio del segmento de memoria reservado

     if( Uart_driver->Pool_puntero == NULL ){
     	gpioWrite( LEDR, ON );
 		printf( "Error al crear el pool" );
 		while(TRUE);
     }

	Uart_driver->rxLen = 0; //contador de tamaño Rx en 0
	Uart_driver->txLen = 0; //contador de tamaño Tx en 0

	Uart_driver->dato_rx = (tMensaje*) QMPool_get(&Uart_driver->Pool_memoria,0); //pido un bloque del pool

	// Habilito las interrupciones
	uartInterrupt(Uart_driver -> uartValue, TRUE);

	return TRUE;

}

void packetTX( driver_t* Uart_driver, tMensaje* mensaje){

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueSend(Uart_driver->onTxQueue, &mensaje, &xHigherPriorityTaskWoken); //Envio a la cola de transmision el blocke a transmitir

    taskENTER_CRITICAL();  //no permito que se modifique txcounter

    //si se esta enviando algo no llamo a la interrupcion para no interrumpir el delay
    if( Uart_driver->txCounter == 0 ){
        txInterruptEnable( Uart_driver );
    }

    taskEXIT_CRITICAL();
    uartSetPendingInterrupt( Uart_driver->uartValue );
}



void imprimir(char* mensaje){

	uartWriteString( UART_USB, mensaje );
	uartWriteString( UART_USB, "\r\n" );
}


