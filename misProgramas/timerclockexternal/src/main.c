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

#include "timerClockExternal.h"
#include "sapi.h"

/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/
uint32_t cantidadPasos=48;

/*=====[Definiciones de variables globales privadas]=========================*/



/*=====[Funcion principal, punto de entrada al programa luego de encender]===*/
int main( void )
{

   // ----- Setup -----------------------------------
   boardInit();

   signalInit();  // Initialize timer to generate signal





   //signalStart(); // Start to generate signal

   signalCheck(); // For debug (see output at printf UART)

   // ----- Repeat for ever -------------------------
   while( true ) {

	   if(!gpioRead(TEC1)){
		   giroMotorCCW(cantidadPasos);

		/*while (count<192){
			signalStart();
			}
		signalStop();
		count=0;*/
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


	   //delay(250);
	   gpioToggle(LED1);


   }

	   /*if(!gpioRead(TEC1)){
		   if (count<10){ // TEC1 start signal
		 	 signalStart();
		  }
	   }
	   }*/

      /*if( !gpioRead(TEC1) ) { // TEC1 start signal
         signalStart();
         printf( "Signal start.\r\n" );
         delay(250);
      }
      if( !gpioRead(TEC2) ) { // TEC2 stop signal
         signalStop();
         printf( "Signal stop.\r\n" );
         delay(250);
      }*/


   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}























