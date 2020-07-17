// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// sAPI header
#include "sapi.h"

#include "FreeRTOSConfig.h"

#include "tareas.h"
#include "tipos.h"
#include "fsm.h"
#include "extras.h"

extern tObjeto obj_1;
extern tObjeto obj_2;


void objeto_crear(tObjeto* objeto)
{
	objeto->cola  = xQueueCreate(L_QUEUE,sizeof(char[L_MSG]));

	if( objeto->cola == NULL)
	{
		gpioWrite( LEDR, ON );
		imprimir( "Error al crear la cola." );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

}

void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad,puntero);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		imprimir( "Error al crear las tareas." );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

}
void Tarea_A( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char* mensaje = MSG_LED;
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        gpioWrite( LEDB , ON );
        post(&obj_1,mensaje);
        vTaskDelay( xPeriodicity / 2 );
        gpioWrite( LEDB , OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

void Tarea_B( void* taskParmPtr )
{
	tLedTecla* config = (tLedTecla*) taskParmPtr;
	uint8_t i;

	for ( i = 0 ; i < N_TECLAS ; i++ )
		fsmButtonInit( &(config[i]), i );

	while( TRUE )
	{
		for ( i = 0 ; i < N_TECLAS ; i++ )
		{
			fsmButtonUpdate( &(config[i]) );
			vTaskDelay( 1 / portTICK_RATE_MS );
		}
	}
}

void Tarea_C( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------
    char mensaje[L_MSG];
    while( TRUE )
    {
        get(&obj_1,mensaje);
        imprimir(mensaje);
        post(&obj_2,mensaje);

        vTaskDelay( xPeriodicity / 20 );
    }
}

void Tarea_D( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();
    char mensaje[L_MSG];
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	get(&obj_2,mensaje);

        vTaskDelay( xPeriodicity / 20 );
    }
}

