/*=====[Nombre del programa]===================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */

/*Objetivo

 */


/*=====[Inclusiones de dependencias de funciones]============================*/

#include "sapi.h"
#include "timer.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"




/*=====[Macros de definición de constantes privadas]=========================*/
void tarea_motor( void* taskParmPtr );
void gpiotoogle( void* taskParmPtr );
/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/
uint32_t cantidadPasos=960;

/*=====[Definiciones de variables globales privadas]=========================*/









/*=====[Funcion principal, punto de entrada al programa luego de encender]===*/
int main( void )
{

   // ----- Setup -----------------------------------
   boardInit();
   signalInit();  // Initialize timer to generate signal

   BaseType_t res1 =xTaskCreate(tarea_motor,                     // Funcion de la tarea a ejecutar
   			(const char *)"tarea motor",   // Nombre de la tarea como String amigable para el usuario
   			configMINIMAL_STACK_SIZE*2,      // Cantidad de stack de la tarea
   			0,                    // Parametros de tarea
   			tskIDLE_PRIORITY+1,              // Prioridad de la tarea
   			0);                              // Puntero a la tarea creada en el sistema


   	//----------Tarea que incrementara o decrementara el valor de longitud de onda para el ensayo
   	BaseType_t res2 =xTaskCreate(gpiotoogle,                  // Funcion de la tarea a ejecutar
   			(const char *)"gpio toogle ",    // Nombre de la tarea como String amigable para el usuario
   			configMINIMAL_STACK_SIZE*2,        // Cantidad de stack de la tarea
   			0,                                // Parametros de tarea
   			tskIDLE_PRIORITY+1,               // Prioridad de la tarea
   			0);                              // Puntero a la tarea creada en el sistema


   	vTaskStartScheduler();




   // ----- Repeat for ever -------------------------
   while( true ) {
	   }


   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}

void tarea_motor( void* taskParmPtr ){

	while(TRUE){
		if(!gpioRead(TEC1)){
			giroMotorCCW(cantidadPasos);
		}
		if(!gpioRead(TEC2)){
			cambioVelocidad(velocidad_baja);
		}

		if(!gpioRead(TEC3)){
			cambioVelocidad(velocidad_media);
		}

		if(!gpioRead(TEC4)){
			cambioVelocidad(velocidad_alta);
		}
		vTaskDelay(100/portTICK_RATE_MS);
		gpioToggle(LED1);
		}



}

void gpiotoogle( void* taskParmPtr ){


	while(TRUE){
		gpioToggle(LED3);
		vTaskDelay(100/portTICK_RATE_MS);
	}

}




















