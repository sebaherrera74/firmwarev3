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

#include "ADS1015.h"
#include "sapi.h"


DEBUG_PRINT_ENABLE;

/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/



/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{
	uint16_t adc0, adc1, adc2, adc3=0;
	   	float voltage_0=0;
	   	static char result[10];

	// ----- Configuraciones -------------------------
   boardInit();
   i2cInit( I2C0, 100000 );


	uartConfig( UART_USB, 115200 );
	uartWriteString( UART_USB,"Driver de Espectrofotometro \r\n" );
	uartWriteString( UART_USB,"lectura de conversor analogico digital " );

   // ----- Repetir por siempre ---------------------
   while(TRUE) {

	   	//  configuro resolucion

	   	setGain(GAIN_TWOTHIRDS);    // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)

	   	adc0 = readADC_SingleEnded(0);
	   	//tomo lectura de un solo canal por ahora
	   	//adc1 = readADC_SingleEnded(1);
	   	//adc2 = readADC_SingleEnded(2);
	   	//adc3 = readADC_SingleEnded(3);
	   	//printf("AIN0: %i \r\n ",adc0);

	   	voltage_0=(adc0*0.003);   //multiplico valor leido por la resolucion seleccionada arriba


	   floatToString( voltage_0, result, 8 );  //convierto valor flotante en string para visualizar

	   uartWriteString( UART_USB, result );
	   	uartWriteByte( UART_USB, '\r' ); // Envia '\r', retorno de carro
	   	uartWriteByte( UART_USB, '\n' ); // Envia '\n', nueva linea

       delay(10);
	   	//solo muestro el AIN0 por ahora solo a los fines practicos

	   	//printf("Valor de voltaje leido es : %s \r\n ",result);
	   	//printf("AIN1: %i \r\n ",adc1);
	   	//printf("AIN2: %i \r\n ",adc2);
	   	//printf("AIN3: %i \r\n ",adc3);



    //delay(1000);




   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}






















