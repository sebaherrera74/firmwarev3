/*==================[inclusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// sAPI header
#include "sapi.h"

#include "FreeRTOSConfig.h"
#include "tipos.h"
#include "tareas.h"
#include "extras.h"

#include "qmpool.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

tLedTecla tecla_led_config[N_TECLAS];

tMensaje* Pool_puntero; //puntero al segmento de memoria que albergara el pool
QMPool Pool_memoria; //memory pool (contienen la informacion que necesita la biblioteca qmpool.h

extern tObjeto obj_1;
extern tObjeto obj_2;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();

    // Inicializo UART debug
    uartConfig( UART_USB, 115200 );
    imprimir( "### Ejercicio de modularizacion ###" );

    //	Reservo memoria para el memory pool
    Pool_puntero = ( tMensaje* ) pvPortMalloc( POOL_SIZE * sizeof( tMensaje ) );

    //	Creo el pool de memoria que va a usarse para la transmision
    QMPool_init( &Pool_memoria, ( tMensaje* ) Pool_puntero,POOL_SIZE*sizeof( tMensaje ),PACKET_SIZE ); //Tamanio del segmento de memoria reservado

    if ( Pool_puntero == NULL )
    {
    	gpioWrite( LEDR, ON );
		imprimir( "Error al crear el pool" );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
    }

    // Creacion Y validacion de tareas
    tarea_crear(Tarea_A,"Tarea_A",2,0,1,0);
    tarea_crear(Tarea_B,"Tarea_B",2,tecla_led_config,1,0);
    tarea_crear(Tarea_C,"Tarea_C",2,0,1,0);
    tarea_crear(Tarea_D,"Tarea_D",2,0,1,0);

    // Creacion Y validacion de objetos
    objeto_crear(&obj_1);
    objeto_crear(&obj_2);

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
    return FALSE;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
