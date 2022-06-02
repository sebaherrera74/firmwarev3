/*=====[Nombre del programa]===================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */

/*Interrupcion del Pin GPIO07 , para sensar el swicht de fin de carrera que me dice la posicion
 * cero del monocromador.
 */

/*=====[Inclusiones de dependencias de funciones]============================*/

#include "sapi.h"
#include "chip.h"       // <= Biblioteca LPCOpen, capa chip (del fabricante NXP)
#include "swichtIrq.h"

static bool_t estadoswicht=OFF;


// Handler interrupt from GPIO pin or GPIO pin mapped to PININT



/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void)
{


	// ----- Configuraciones -------------------------

	boardConfig();

	swichtIrq_init(&swichtIrq,GPIO6);
	//GPIO6





   // ----- Repetir por siempre ---------------------
   while(TRUE) {
	   if(swichtIrqEstado(&swichtIrq)){
		   gpioToggle(LED1);

	   }


	   sleepUntilNextInterrupt();



    //gpioToggle(LED1);





   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}






















