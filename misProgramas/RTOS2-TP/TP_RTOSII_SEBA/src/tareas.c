// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// sAPI header
#include "sapi.h"
#include "FreeRTOSConfig.h"
#include "tareas.h"

//#include "tipos.h"
//#include "extras.h"

//#include "qmpool.h"

//tObjeto obj_1;
//tObjeto obj_2;

//extern tMensaje Pool_puntero;
//extern QMPool Pool_memoria;

void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad,puntero);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		//imprimir( "Error al crear las tareas." );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

}
void TareaLed( void* taskParmPtr ) //tarea tonta de prende y apaga led cada 1 seg
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();


    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        gpioToggle(LEDB);

        /*// Asigno un bloque de memoria
        p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

        if (p != NULL)
        {
			memcpy(p,MSG_LED, sizeof(MSG_LED));
			post(&obj_1,p);
		}
		else
		{
			imprimir(MALLOC_ERROR);
		}*/


        vTaskDelayUntil( &xLastWakeTime , xPeriodicity / 2 );
    }
}


void TareaUART( void* taskParmPtr )
{/*
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------

    tMensaje* p = NULL;*/

    while( TRUE )
    {
    	// Asigno un bloque de memoria
    	/*p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

		if (p != NULL)
		{
			get(&obj_1,&p);
			imprimir(p);
			post(&obj_2,p);
		}
		else
		{
			imprimir(MALLOC_ERROR);
		}*/

        vTaskDelay( 1000 );
    }
}



