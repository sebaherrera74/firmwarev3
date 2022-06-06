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

#include "display7seg.h"
#include "sapi.h"


// Handler interrupt from GPIO pin or GPIO pin mapped to PININT



/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{
	// ----- Configuraciones -------------------------

	boardConfig();
	display7seg_init();
	gpioWrite( GPIO4, 1);
	gpioWrite( GPIO5, 1);
 uint8_t i=0;

 uint16_t tiempo=1000;


	// ----- Repetir por siempre ---------------------
	while(TRUE) {
if (tiempo==0){
	tiempo=1000;
	}
		/*delay(1000);
		display7segEnciendeDig(0);
		delay(1000);
		display7segEnciendeDig(1);
		delay(1000);
		display7segEnciendeDig(2);
		delay(1000);*/
	for (i=0;i<10;i++){
	  display7segEnciendeDig(i);

	  delay(tiempo);
  }
tiempo=tiempo-50;


		//gpioToggle(LED1);
		//    delay(250);




	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontrolador y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}






















