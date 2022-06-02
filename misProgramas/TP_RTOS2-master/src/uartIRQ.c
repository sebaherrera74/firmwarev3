
#include "driver.h"
#include "userTasks.h"
#include "semphr.h"
#include "qmpool.h"
#include "sapi.h"


void onTxCallback( void* );
void onRxCallback( void* );


//rxInterruptEnable(driverConfig_t* Uart_driver) Habilita la interrupcion de la UART para recepcion
//indicada por selectedUart
bool_t rxInterruptEnable( driver_t* Uart_driver )
{
	switch( Uart_driver-> uartValue)
	{
		case UART_GPIO:
		case UART_USB:
		case UART_232:
		{
			uartCallbackSet( Uart_driver->uartValue, UART_RECEIVE, onRxCallback, (void*)Uart_driver );
			break;
		}
		default:
		{
			return (FALSE); // No se eligio una UART correcta
		}
	}

	return TRUE;
}

//txInterruptEnable(driverConfig_t* Uart_driver) Habilita la interrupcion de la UART para transmision
//indicada por selectedUart
 bool_t txInterruptEnable( driver_t* Uart_driver )
 {
	 switch( Uart_driver-> uartValue)
	{
		case UART_GPIO:
		case UART_USB:
		case UART_232:
		{
			uartCallbackSet( Uart_driver->uartValue, UART_TRANSMITER_FREE, onTxCallback, (void*)Uart_driver );
			break;
		}
		default:
		{
			return (FALSE); // No se eligio una UART correcta
		}
	}

	return TRUE;

 }

/*=====[Implementations of interrupt functions]==============================*/

// Callback para la recepcion
 void onRxCallback( void *param ){

	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	 UBaseType_t uxSavedInterrumptStatus;

	 tMensaje* p = NULL;
	 // Elegir los datos que necesita el cb

	 driver_t* Uart_driver = param;

	 //abrir seccion critica
	 uxSavedInterrumptStatus = taskENTER_CRITICAL_FROM_ISR();


	 // Leemos el byte de la UART seleccionado

	if (Uart_driver->rxLen < PACKET_SIZE)
	{
		Uart_driver->dato_rx[Uart_driver->rxLen]=uartRxRead( Uart_driver->uartValue );
	}
	else
	{
		Uart_driver->rxLen = PACKET_SIZE + 1;
	}

	 // Inicializamos el timer.
	xTimerStartFromISR(Uart_driver->onRxTimeOut, &xHigherPriorityTaskWoken);

	Uart_driver->rxLen++;

	 //cerrar seccion critica
	 taskEXIT_CRITICAL_FROM_ISR( uxSavedInterrumptStatus );

	 portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
 }


//	uartSendDataCRC transmite los bloques que llegan por una cola y los envia por la UART seleccionada
//	Todas las callback de interrupcion de UART llaman a esta función para hacer la transmisión.
 void onTxCallback( void*param ){

	 driver_t* Uart_driver;
	 UBaseType_t uxSavedInterrumptStatus;
	 BaseType_t xTaskWokenByReceive = pdFALSE;

	 Uart_driver = ( driver_t* ) param;

	 // Armo una seccion critica para proteger tx_counter
	 uxSavedInterrumptStatus = taskENTER_CRITICAL_FROM_ISR();

	 //	Si el contador esta en cero tengo que tomar un bloque de la cola, calcular CRC
	 if ( Uart_driver->txCounter == 0 )
	 {
	         //	Tomo el bloque a transmitir de la cola
	         xQueueReceiveFromISR( Uart_driver->onTxQueue, ( void * ) &Uart_driver->dato_tx ,portMAX_DELAY );

	  }

	 appC2Tx(Uart_driver);

	 taskEXIT_CRITICAL_FROM_ISR( uxSavedInterrumptStatus );

	 // Evaluamos el pedido de cambio de contexto necesario
	 if ( xTaskWokenByReceive != pdFALSE )
	 {
		 taskYIELD();
	 }

 }

 // Callback para el timeout definido en PROTOCOL_TIMEOUT.
 void onRxTimeOutCallback( TimerHandle_t xTimer ){


	 UBaseType_t uxSavedInterrumptStatus;

     int crcTemp;

     // Obtenemos los datos de la UART, aprovechando el campo reservado para el Timer ID.
     driver_t* selectedUart = ( driver_t* ) pvTimerGetTimerID( xTimer );

	 uxSavedInterrumptStatus = taskENTER_CRITICAL_FROM_ISR();

	 appC2Rx(selectedUart);

	 taskEXIT_CRITICAL_FROM_ISR( uxSavedInterrumptStatus );
 }


 //	onTxTimeOutCallback
 // Timer para la separacion de paquetes

 void onTxTimeOutCallback( TimerHandle_t xTimer )
 {
     BaseType_t xHigherPriorityTaskWoken;
     xHigherPriorityTaskWoken = pdFALSE;

     // Obtenemos los datos de la UART seleccionada, aprovechando el campo reservado para el Timer ID.
     driver_t* selectedUart = ( driver_t* ) pvTimerGetTimerID( xTimer );

     //Inicio seccion critica
     taskENTER_CRITICAL();

     uartTxWrite( selectedUart->uartValue,'\r' );
     //uartTxWrite( selectedUart->uartValue,'\n' );

     // Libero el bloque de memoria que ya fue trasmitido
     QMPool_put( &selectedUart->Pool_memoria, selectedUart->dato_tx );

     selectedUart->txCounter = 0; //Reinicio el contador de bytes transmitidos para la siguiente transmision

     taskEXIT_CRITICAL(); // Fin de seccion critica

     //si hay mensajes esperando vuelvo a habilitar la interrupción y la disparo
     if ( uxQueueMessagesWaiting( selectedUart->onTxQueue ) )
     {
         txInterruptEnable( selectedUart );
         uartSetPendingInterrupt( selectedUart->uartValue );
     }

 }















