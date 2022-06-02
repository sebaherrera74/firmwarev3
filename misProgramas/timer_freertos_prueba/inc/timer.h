/*=====[Nombre del modulo]=====================================================
 * Copyright  Sebastian Herrera  <sebaherreraa@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Fecha de creacion: 2020/12/27
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _TIMER_H_
#define _TIMER_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/

#include "sapi.h"


/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/

#define PI 3.14
#define BAJA    204000000
#define MEDIA   1000000
#define ALTA    300000

/*=====[Macros estilo funcion publicas]======================================*/

#define printInt(printer,number) printIntFormat((printer),(number),(DEC_FORMAT))

#define printlnString(printer,string);   {printString((printer),(string));\
                                          printEnter((printer));}

/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado
typedef enum {
	velocidad_baja,
	velocidad_media,
	velocidad_alta,
}velocidad_motor;


/*=====[Prototipos de funciones publicas]====================================*/
void signalInit( void );
void signalCheck( void );
void signalStart( void );
void signalStop( void );
uint32_t readPrescaler(void);
void giroMotorCCW(uint32_t cantPasos);
void cambioVelocidad(velocidad_motor veloc);

/*=====[Prototipos de funciones publicas de interrupcion]====================*/

void TIMER2_IRQHandler(void);

/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
