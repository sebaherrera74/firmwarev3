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

char* itoa(int value, char* result, int base) ;

/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)


{
	uint16_t adc0, adc1, adc2, adc3=0;
	float voltage_0=0;
	static char result[10];
	uint16_t muestra = 0;
	static char uartBuff[10];
	uint8_t i=0;

	// ----- Configuraciones -------------------------
	boardInit();
	i2cInit( I2C0, 100000 );

	adcConfig( ADC_ENABLE ); /* ADC */
	uartConfig( UART_USB, 115200 );
	//uartWriteString( UART_USB,"Driver de Espectrofotometro \r\n" );
	//uartWriteString( UART_USB,"lectura de conversor analogico digital " );

	// ----- Repetir por siempre ---------------------
	while(TRUE) {
        i++;
		muestra = adcRead( CH1 );
		//uartWriteString( UART_USB, "ADC CH1 value: " );
		/* Conversión de muestra entera a ascii con base decimal */
		itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */
		itoa( i, result, 10 ); /* 10 significa decimal */
		/* Enviar muestra y Enter */
		uartWriteString( UART_USB, uartBuff );
		uartWriteString( UART_USB, "\n" );



		delay(1000);






	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontrolador y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}





char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

















