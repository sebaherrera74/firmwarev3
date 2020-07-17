/*==================[inclusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// sAPI header
#include "sapi.h"

#include "FreeRTOSConfig.h"
#include "tipos.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

//QueueHandle_t cola_1;
tLedTecla tecla_led_config[N_TECLAS];

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void Tarea_A( void* taskParmPtr );
void Tarea_B( void* taskParmPtr );
void Tarea_C( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();

    // UART for debug messages
    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "Ejercicio de modularizacion.\r\n" );

    tecla_led_config[0].led 	= LED1;
	tecla_led_config[0].tecla 	= TEC1;
	tecla_led_config[0].sem_tec_pulsada 	= xSemaphoreCreateBinary();
	tecla_led_config[0].contFalling = 0;
	tecla_led_config[0].contRising = 0;
	tecla_led_config[0].fsmButtonState = STATE_BUTTON_UP;  // Set initial state

	tecla_led_config[1].led 	= LED2;
	tecla_led_config[1].tecla 	= TEC2;
	tecla_led_config[1].sem_tec_pulsada 	= xSemaphoreCreateBinary();
	tecla_led_config[1].contFalling = 0;
	tecla_led_config[1].contRising = 0;
	tecla_led_config[1].fsmButtonState = STATE_BUTTON_UP;  // Set initial state

    // Crear tarea en freeRTOS
    BaseType_t res_A =
    xTaskCreate(
        Tarea_A,                     	// Funcion de la tarea a ejecutar
        ( const char * )"Tarea_A",   	// Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        0,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        0                          		// Puntero a la tarea creada en el sistema
    );

    BaseType_t res_B =
        xTaskCreate(
            Tarea_B,                     	// Funcion de la tarea a ejecutar
            ( const char * )"Tarea_B",   	// Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
            0,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            0                          		// Puntero a la tarea creada en el sistema
        );

    BaseType_t res_C =
        xTaskCreate(
            Tarea_C,                     	// Funcion de la tarea a ejecutar
            ( const char * )"Tarea_C",   	// Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
            0,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            0                          		// Puntero a la tarea creada en el sistema
        );

    cola_1  = xQueueCreate(10,sizeof(char[50]));

    // Gestion de errores
    if(!(res_A && res_B && res_C))
    {
    	gpioWrite( LEDR, ON );
    	debugPrintlnString( "Error al crear las tareas.\r\n" );
    	while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
    }

    if( cola_1 == NULL)
	{
		gpioWrite( LEDR, ON );
		debugPrintlnString( "Error al crear la cola.\r\n" );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

    if( (tecla_led_config[0].sem_tec_pulsada  == NULL) || (tecla_led_config[1].sem_tec_pulsada  == NULL) )
	{
		gpioWrite( LEDR, ON );
		debugPrintlnString( "Error al crear semaforos.\r\n" );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

    // Iniciar scheduler
    vTaskStartScheduler(); // Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void Tarea_A( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char* mensaje = "LEDB ON";
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        gpioWrite( LEDB , ON );
        post(mensaje,cola_1);
        //xQueueSend( cola_1 , mensaje , portMAX_DELAY );
        vTaskDelay( 500 / portTICK_RATE_MS );
        gpioWrite( LEDB , OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

void Tarea_B( void* taskParmPtr )
{
	uint8_t i;

	while( TRUE )
	{
		for ( i = 0 ; i < N_TECLAS ; i++ )
		{
			fsmButtonUpdate( &(tecla_led_config[i]) );
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
    char mensaje[50];
    while( TRUE )
    {
        get(mensaje,cola_1);
        //xQueueReceive( cola_1 , &mensaje ,portMAX_DELAY );

        debugPrintlnString( mensaje )

        vTaskDelay( 50 / portTICK_RATE_MS );
    }
}



/*==================[fin del archivo]========================================*/
