
#include "userTasks.h"

/*=====[Implementations of public functions]=================================*/

void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero){

	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad,puntero);  // Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL){

		gpioWrite( LEDR, ON );
		while(TRUE);
	}

}


void led_task( void* pvParameters ){

	while( TRUE ){

		gpioToggle( LEDG );
		vTaskDelay(100/portTICK_RATE_MS);
	}
}

//checkTask recibe como parametro los datos de una UART con el driver inicializado
//se queda esp#define xQueueSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken ) xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), ( pxHigherPriorityTaskWoken ), queueSEND_TO_BACK )erando a que llegue un paquete, lo procesa y lo envia por la UART

void uart_task( void* pvParameters ){

	driver_t *Uart_driver;
	Uart_driver = ( driver_t * )pvParameters;

	while( TRUE ){

		appC3(Uart_driver);
	}
}




