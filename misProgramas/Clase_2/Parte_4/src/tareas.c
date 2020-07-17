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

#include "qmpool.h"

tObjeto obj_1;
tObjeto obj_2;

extern tMensaje Pool_puntero;
extern QMPool Pool_memoria;

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

    tMensaje* p = NULL;

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        gpioWrite( LEDB , ON );

        // Asigno un bloque de memoria
        p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

        if (p != NULL)
        {
			memcpy(p,MSG_LED, sizeof(MSG_LED));
			post(&obj_1,p);
		}
		else
		{
			imprimir(MALLOC_ERROR);
		}

        vTaskDelayUntil( &xLastWakeTime , xPeriodicity / 2 );
        gpioWrite( LEDB , OFF );

        vTaskDelayUntil( &xLastWakeTime , xPeriodicity / 2 );
    }
}

void Tarea_B( void* taskParmPtr )
{
	tLedTecla* config = (tLedTecla*) taskParmPtr;
	uint8_t i;

	bool_t estado = FALSE;

	for ( i = 0 ; i < N_TECLAS ; i++ )
		fsmButtonInit( &(config[i]), i );

	tMensaje* p = NULL;
	char buffer [PACKET_SIZE];

	while( TRUE )
	{
		for ( i = 0 ; i < N_TECLAS ; i++ )
		{
			estado = fsmButtonUpdate( &(config[i]) );

			if (estado)
			{
				// Asigno un bloque de memoria
				p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

				if (p != NULL)
				{
					sprintf (buffer, FSM_MSG , config[i].tecla+1-TEC1,config[i].tiempo_medido);
					memcpy(p,buffer, sizeof(buffer));
					post(&obj_1,p);
				}
				else
				{
					imprimir(MALLOC_ERROR);
				}
			}

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

    tMensaje* p = NULL;

    while( TRUE )
    {
    	// Asigno un bloque de memoria
    	p = ( tMensaje* ) QMPool_get( &Pool_memoria, 0 ); //pido un bloque del pool

		if (p != NULL)
		{
			get(&obj_1,&p);
			imprimir(p);
			post(&obj_2,p);
		}
		else
		{
			imprimir(MALLOC_ERROR);
		}

        vTaskDelay( xPeriodicity / 20 );
    }
}

void Tarea_D( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    tMensaje* p = NULL;
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	get(&obj_2,&p);

    	//vPortFree(p);

    	// Libero el bloque de memoria que ya fue trasmitido
    	QMPool_put( &Pool_memoria, p );

        vTaskDelay( xPeriodicity / 20 );
    }
}

