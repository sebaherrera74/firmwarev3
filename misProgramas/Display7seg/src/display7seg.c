/*=====[Swicht posicion cero]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/18
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "display7seg.h"

#include "sapi.h"
/*=====[Macros de definicion de constantes privadas]=========================*/
/*=====[Macros estilo funcion privadas]======================================*/
/*=====[Definiciones de tipos de datos privados]=============================*/
// Tipo de datos que renombra un tipo basico
// Tipo de datos de puntero a funcion
// Tipo de datos enumerado
// Tipo de datos estructua, union o campo de bits
/*=====[Definiciones de Variables globales publicas externas]================*/
/*=====[Definiciones de Variables globales publicas]=========================*/
/*=====[Definiciones de Variables globales privadas]=========================*/
/*=====[Prototipos de funciones privadas]====================================*/
//static void funPrivada(void);
/*=====[Implementaciones de funciones publicas]==============================*/

void display7seg_init(){

	//Inicializo pines
	//configuro como salidas
	gpioInit( GPIO0, GPIO_OUTPUT);
	gpioInit( GPIO1, GPIO_OUTPUT);
	gpioInit( GPIO2, GPIO_OUTPUT);
	gpioInit( GPIO3, GPIO_OUTPUT);
	gpioInit( GPIO4, GPIO_OUTPUT);
}

void display7segEnciendeDig(uint8_t numero){

	switch (numero) {


	case 0 :
	gpioWrite( GPIO0, 0 );
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 0 );
	gpioWrite( GPIO3, 0 );
    	break;

	case 1:
		gpioWrite( GPIO0, 1);
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 0);
	gpioWrite( GPIO3, 0);
   break;

	case 2:
	gpioWrite( GPIO0, 0);
	gpioWrite( GPIO1, 1);
	gpioWrite( GPIO2, 0);
	gpioWrite( GPIO3, 0);
	break;

	case 3:
	gpioWrite( GPIO0, 1);
	gpioWrite( GPIO1, 1);
	gpioWrite( GPIO2, 0);
	gpioWrite( GPIO3, 0);
break;
	case 4:
	gpioWrite( GPIO0, 0);
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 1);
	gpioWrite( GPIO3, 0);
break;

	case 5:
	gpioWrite( GPIO0, 1);
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 1);
	gpioWrite( GPIO3, 0);
break;

	case 6:
	gpioWrite( GPIO0, 0);
	gpioWrite( GPIO1, 1);
	gpioWrite( GPIO2, 1);
	gpioWrite( GPIO3, 0);
break;

	case 7:
	gpioWrite( GPIO0, 1);
	gpioWrite( GPIO1, 1);
	gpioWrite( GPIO2, 1);
	gpioWrite( GPIO3, 0);
break;

	case 8:
	gpioWrite( GPIO0, 0);
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 0);
	gpioWrite( GPIO3, 1);
break;

	case 9:
	gpioWrite( GPIO0, 1);
	gpioWrite( GPIO1, 0);
	gpioWrite( GPIO2, 0);
	gpioWrite( GPIO3, 1);
break;

	default:
	      // instructions par défaut
	      break;


	}


}










/*=====[Implementaciones de funciones de interrupcion publicas]==============*/
/*=====[Implementaciones de funciones privadas]==============================*/

