/*=====[Nombre del programa]===================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */


/*=====[Inclusiones de dependencias de funciones]============================*/


#include "sapi.h"


DEBUG_PRINT_ENABLE;

/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/

struct header_struct {
   char		pre[8];
   uint32_t id;
   uint16_t N;
   uint16_t fs ;
   char		pos[4];
} header={"*header*",0,128,10000,"end*"};

void trigger(int16_t threshold)
{
   while((adcRead(CH1)-512)>threshold)
	  ;
   while((adcRead(CH1)-512)<threshold)
	  ;
   return;
}

/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{


	// ----- Configuraciones -------------------------
   boardInit();





	   uint16_t sample = 0;
	      int16_t adc[header.N];
	      boardConfig();
	      uartConfig(UART_USB,460800);
	      adcConfig( ADC_ENABLE);
	      cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );
	      while(1) {
	   	  cyclesCounterReset();
	   	  adc[sample] = (int16_t )adcRead(CH1)-512;					 // va de -512 a 511
	   	  uartWriteByteArray ( UART_USB ,(uint8_t* )&adc[sample] ,sizeof(adc[0]) );
	   	  if ( ++sample==header.N ) {
	   		 gpioToggle ( LEDR ); // este led blinkea a fs/N
	   		 sample = 0;
	   		 //trigger(2);
	   		 header.id++;
	   		 uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(header ));
	   		 adcRead(CH1); //why?? hay algun efecto minimo en el 1er sample.. puede ser por el blinkeo de los leds o algo que me corre 10 puntos el primer sample. Con esto se resuelve.. habria que investigar el problema en detalle
	   	  }
	   	  gpioToggle ( LED1 );											 // este led blinkea a fs/2
	   	  while(cyclesCounterRead()< EDU_CIAA_NXP_CLOCK_SPEED/header.fs) // el clk de la CIAA es 204000000
	   		 ;



   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}






















