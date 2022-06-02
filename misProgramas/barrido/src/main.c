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
Medir el tiempo de pulsación de un botón y mostrar el tiempo encendiendo un led.
Resumen
El programa espera a que un botón sea pulsado. Mientras el botón está pulsado, se mide el tiempo utilizando
un contador hasta que el botón es soltado. En ese momento, se enciende el led asociado al botón durante el
 tiempo calculado.
Comentarios
Para medir el tiempo del botón es posible implementar un bucle iterativo que se extienda mientras el botón
 está apretado. Dentro de ese bucle, existirá un delay pequeño (por ejemplo, de 1 milisegundo) y por cada
 iteración se incrementará en 1 el contador. Cuando el bucle finaliza, significa que el botón fue soltado y
 en el contador tendremos la cantidad de milisegundos que debemos encender el led.
 */


/*=====[Inclusiones de dependencias de funciones]============================*/

#include "sapi.h"
#include "secuencia_giro.h"


DEBUG_PRINT_ENABLE;

/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/



/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{


	// ----- Configuraciones -------------------------
   boardInit();

   inicializar_bobinas();

	uartConfig( UART_USB, 115200 );
	uartWriteString( UART_USB,"Driver de Espectrofotometro \r\n" );
	uartWriteString( UART_USB,"Presione Tecla 1 p/ comenzar " );

   // ----- Repetir por siempre ---------------------
   while(TRUE) {

        if (!gpioRead(TEC1)){
		    gpioWrite(LEDB,ON);
		    gpioWrite(LED1,OFF);
            rotarBobinasCCW(20,1000);
        }
	   else{
           gpioWrite(LED1,ON);
		   gpioWrite(LEDB,OFF);
		   //detenerMotor();

     	   }



    //delay(1000);




   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}






















