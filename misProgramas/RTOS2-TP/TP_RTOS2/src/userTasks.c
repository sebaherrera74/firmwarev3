/*=============================================================================
 * Copyright (c) 2019, Agustin Curcio Berardi <acurber91@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2019/09/22
 * Version: v1.0
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/
#include "FreeRTOSConfig.h"
#include "userTasks.h"
#include "semphr.h"
#include "tipos.h"
#include "qmpool.h"
#include "extras.h"

extern char mensaje[];

/*=====[Definitions of private global variables]=============================*/
/*tObjeto obj_1;
tObjeto obj_2;

tMensaje* Pool_puntero; //puntero al segmento de memoria que albergara el pool
QMPool Pool_memoria; //memory pool (contienen la informacion que necesita la biblioteca qmpool.h
*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad,puntero);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		while(TRUE);
	}

}
void led_task( void* pvParameters )
{
	while( TRUE )
	{
		gpioToggle( LEDG );
		vTaskDelay(100/portTICK_RATE_MS);
	}
}
//checkTask recibe como parametro los datos de una UART con el driver inicializado
//se queda esperando a que llegue un paquete, lo procesa y lo envia por la UART

void uart_task( void* pvParameters )
{

	char*TXbuffer;
	driver_t *Uart_driver;
	Uart_driver = ( driver_t * )pvParameters;

	while( TRUE )
	{
		gpioToggle( LED3 );

		TXbuffer=waitPacket(Uart_driver);

		if (TXbuffer!=NULL){			///SI NO ES NULO ES PORQUE RECIBI ALGO Y SI RECBIO ALGO LO PROCESO
			check_packet(TXbuffer);    //chequeo las condiciones de la trama
			packetTX( Uart_driver , TXbuffer );

		}
	vTaskDelay(100/portTICK_RATE_MS);

		/*xSemaphoreTake( Uart_driver->permiso  , portMAX_DELAY );

		// Asigno un bloque de memoria
		p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

		if (p != NULL){

			get(&obj_1,&p);

			imprimir(p);
			//post(&obj_2,p);

		}else{
			//imprimir("Error en uart_task");
		}
*/
}

// habilito cb rx
// recibo algo
// libero semaforo en cb rx
// uart_task toma ese semaforo
// llama a packetTX -> habilita cb tx
// cb tx -> imprime


}

void check_packet(char* string)
 {

	/*BTX erroneo?  Desacrtar trama, imprimir ERROR_B
	ETX erroneo?  Descartar trama, imprimir ERROR_E
	No todos son letras ? Descartar trama, imprimir ERROR_L
	Cheksum erroneo? No imprimir Nada*/

	// "(mHOLACRC)"
	int i=2;

	int largo=string[0];

	//BTX CORRECTO
	if (string[1]!= SOM){
		strcpy(string,"ERROR_B");
		string[0]=7;	//indico cuantos bytes tiene el string
		return;
	}

	//ETX CORRECTO
	if (string[largo]!= EOM){
		strcpy(string,"ERROR_E");
		string[0]=7;   //indico cuantos bytes tiene el string
		return;
	}
  //chequeo que sean letras

	while (i+1<largo){
		//compruebo si esta entre a y z
		if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i]<= 'Z')){

		}
		else{
		     	strcpy(string,"ERROR_L");  //error que no es numerico
				string[0]=7;   //indico cuantos bytes tiene el string
				return;
		}

  }
}


/*
void packetTX( driver_t* Uart_driver, char *mensaje )
{

    //xQueueSend( Uart_driver->onTxQueue, &mensaje, portMAX_DELAY ); //Envio a la cola de transmision el blocke a transmitir
    taskENTER_CRITICAL();  //no permito que se modifique txcounter
    if ( Uart_driver->txCounter == 0 ) //si se esta enviando algo no llamo a la interrupcion para no interrumpir el delay
    {
        txInterruptEnable( Uart_driver );
    }
    taskEXIT_CRITICAL();
    uartSetPendingInterrupt( Uart_driver->uartValue );
}*/


/*void app(driver_t* Uart_driver, char c){

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	static tMensaje* p = NULL;

	gpioToggle( LED2);

	char buffer[LARGO];

	 if(c==SOM){
		 //reiniciamos el contador de recepción
		 Uart_driver -> rxLen=0;
	 }

	 // Si llegan mas bytes de los que puedo guardar, no los leo.
	 if ( Uart_driver -> rxLen < LARGO ){

		 buffer[Uart_driver->rxLen] = c;

		 // Incrementar el indice
		 Uart_driver->rxLen++;

		 //reviso si el ultimo caracter es ')'
		 if((c == EOM)){

			 buffer[Uart_driver->rxLen] = '\0';
			 // Asigno un bloque de memoria
			 p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool
			 memcpy(p , &buffer[0], sizeof(buffer));
			 post(&obj_1,p);
			 xSemaphoreGiveFromISR( Uart_driver->permiso , &xHigherPriorityTaskWoken);

		 }

	 }else{
		 Uart_driver -> rxLen = LARGO;
	 }

}

void init_pools(void){

    //	Reservo memoria para el memory pool
    Pool_puntero = ( tMensaje* ) pvPortMalloc( POOL_SIZE * sizeof( tMensaje ) );

    //	Creo el pool de memoria que va a usarse para la transmision
    QMPool_init( &Pool_memoria, ( tMensaje* ) Pool_puntero,POOL_SIZE*sizeof( tMensaje ),PACKET_SIZE ); //Tamanio del segmento de memoria reservado

    if( Pool_puntero == NULL ){
    	gpioWrite( LEDR, ON );
		printf( "Error al crear el pool" );
		while(TRUE);
    }
}*/


