/*=====[Nombre del modulo]=====================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */

/*=====[Inclusion de su propia cabecera]=====================================*/
#include "secuencia_giro.h"


#include "sapi.h"

//#include <dependencia.h>

/*=====[Macros de definicion de constantes privadas]=========================*/



/*=====[Macros estilo funcion privadas]======================================*/



/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico
// Tipo de datos de puntero a funcion

// Tipo de datos enumerado

// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/

/*=====[Definiciones de Variables globales publicas]=========================*/

uint8_t secuenciaPasos[4][4]={
		{1,1,0,0},
		{0,1,1,0},
		{0,0,1,1},
		{1,0,0,1}
};
/*=====[Definiciones de Variables globales privadas]=========================*/

/*=====[Prototipos de funciones privadas]====================================*/
//static void setSentidoGiro(void);
static char* itoa(int value, char* result, int base);
static void valores_cantpasos_decreciente(int cantpasos);

/*=====[Implementaciones de funciones publicas]==============================*/
void inicializar_bobinas(){
	gpioConfig( GPIO1, GPIO_OUTPUT );
	gpioConfig( GPIO2, GPIO_OUTPUT );
	gpioConfig( GPIO3, GPIO_OUTPUT );
	gpioConfig( GPIO4, GPIO_OUTPUT );

}


void rotarBobinasCW(uint8_t velocidad,uint16_t cantPasos){

	uint8_t aux=0;
	uint16_t i;

	for (i=0;i<cantPasos;i++){

		aux=(i+4)%4;
		gpioWrite(GPIO1,secuenciaPasos[aux][0]);
		gpioWrite(GPIO2,secuenciaPasos[aux][1]);
		gpioWrite(GPIO3,secuenciaPasos[aux][2]);
		gpioWrite(GPIO4,secuenciaPasos[aux][3]);
		delay(velocidad);
		//uartTxWrite(UART_USB,i);


	}
}


void rotarBobinasCCW(uint8_t velocidad,uint16_t cantPasos){
	uint8_t aux=0;
	int i;
	static char uartBuff[10];

	for (i=cantPasos;i>0;i--){
		aux=(i+4)%4;

		gpioWrite(GPIO1,secuenciaPasos[aux][0]);
		gpioWrite(GPIO2,secuenciaPasos[aux][1]);
		gpioWrite(GPIO3,secuenciaPasos[aux][2]);
		gpioWrite(GPIO4,secuenciaPasos[aux][3]);
		delay(velocidad);
		valores_cantpasos_decreciente(i);
	}


}

void detenerMotor(void){
	gpioWrite(GPIO1,0);
	gpioWrite(GPIO2,0);
	gpioWrite(GPIO3,0);
	gpioWrite(GPIO4,0);

}



static void valores_cantpasos_decreciente(int cantpasos){
	static char uartBuff[10];

	itoa(cantpasos, uartBuff, 10 ); // base 10 significa decimal
	uartWriteString( UART_USB, uartBuff );
	uartWriteByte( UART_USB, '\r' ); // Envia '\r', retorno de carro
	uartWriteByte( UART_USB, '\n' ); // Envia '\n', nueva linea

}

static char* itoa(int value, char* result, int base) {
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
