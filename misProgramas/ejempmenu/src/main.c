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


/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/
void  led1On ( void );
void  led1Off ( void );
void led2On ( void );
void led2Off ( void );
void led3On ( void );
void led3Off ( void );



typedef void  (* action )( void );


typedef struct {
	char * txt ;
	action doAction ;
} menuItem ;

const menuItem menu [] = {
{ " 1 - Encender led 1\r\n " , led1On } ,
{ " 2 - Apagar   led 1\r\n " , led1Off } ,
{ NULL , NULL }
};


char str[]={"ups,opcion invalida\r\n " };

/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{


	// ----- Configuraciones -------------------------
   boardInit();
   uartConfig( UART_USB, 115200 );
   uint8_t op ;
   int i =0;


   // ----- Repetir por siempre ---------------------
   while(TRUE) {
	   for (i =0;menu[i].txt!= NULL ; i ++){
		   printf (menu[i].txt);
	   }
	   if(uartReadByte( UART_USB,&op)){
		   op -= '1';
		   if (( op >=0) && ( op < i )){
			   menu[op].doAction ();
	           printf( " Ok.\r\n " );

	            }
	     else{
	    	 uartWriteString(UART_USB, str);
	     }
	   }
	   op=0;

   delay(1000);
   }
	   return 0;
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.


void led1On ( void ){
	gpioWrite(LED1,ON);
}
void led1Off ( void ){
gpioWrite(LED1,OFF);
}











