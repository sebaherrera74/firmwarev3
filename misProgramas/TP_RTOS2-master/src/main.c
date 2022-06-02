
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

#include "driver.h"
#include "FreeRTOSConfig.h"
#include "userTasks.h"
#include "driver.h"
#include "semphr.h"

extern void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);

int main( void )
{
    static driver_t Uart_driver1; //Variable de la instanciacion del driver que va a usar la tarea principal

    boardConfig();	 // Inicializar la placa

	//creación y validación de tareas
    tarea_crear(led_task,"led_task",2,0,1,0);//creamos tarea led
    tarea_crear(uart_task,"uart_task",4,&Uart_driver1,1,0);//creamos tarea recepción uart

	if (driverInit(&Uart_driver1) == FALSE)	//configuramos interr. serial por recep.
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
