#include "driver.h"
#include "extras.h"
#include "FreeRTOSConfig.h"
#include "tipos.h"
#include "userTasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

#include "semphr.h"

extern tObjeto obj_1;
extern tObjeto obj_2;

extern void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);
SemaphoreHandle_t mutexuartrecp;
SemaphoreHandle_t semaforouart;

char mensaje[LARGO];

int main( void )
{
    static driver_t Uart_driver1; //Variable de la instanciacion del driver que va a usar la tarea principal

    boardConfig();	 // Inicializar la placa

    //	Reservo memoria para el memory pool
    init_pools();

    //Inicializacion del driver de UART

    Uart_driver1.uartValue = UART_USB;
    Uart_driver1.baudRate = 115200;

    /* Attempt to create a semaphore. */
     if (NULL == (Uart_driver1.permiso = xSemaphoreCreateBinary())){
    	 while(TRUE);
     }

     /* Attempt to create a semaphore. */
      if (NULL == (Uart_driver1.mutex = xSemaphoreCreateMutex())){
     	 while(TRUE);
      }

      xSemaphoreGive( Uart_driver1.mutex);

    //creación de objetos
    objeto_crear(&obj_1, Uart_driver1);


    //creación y validación de tareas
    tarea_crear(led_task,"led_task",2,0,1,0);//creamos tarea led
    tarea_crear(uart_task,"uart_task",4,&Uart_driver1,1,0);//creamos tarea recepción uart


	if ( driverInit( &Uart_driver1 ) == FALSE )//configuramos interr. serial por recep.
    {
        gpioWrite( LED3, ON ); //Error al inicializar el driver
        printf( "Error al inicializar el driver" );
        while( 1 );
    }

    vTaskStartScheduler();
    while(TRUE){

    }

    return 0;
}
