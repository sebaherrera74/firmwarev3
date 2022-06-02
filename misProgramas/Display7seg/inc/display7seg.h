/*=====[Swicht posicion cero]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/18
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _DISPLAY7SEG_H_
#define _DISPLAY7SEG_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/

#include "sapi.h"


/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/


/*=====[Macros estilo funcion publicas]======================================*/


/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado


// Tipo de datos estructua, union o campo de bits




/*=====[Prototipos de funciones publicas]====================================*/

void display7seg_init();
void display7segEnciendeDig(uint8_t numero);




/*=====[Prototipos de funciones publicas de interrupcion]====================*/


/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
