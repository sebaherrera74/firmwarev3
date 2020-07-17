/*
 * extras.h
 *
 *  Created on: Jul 7, 2020
 *      Author: felipe
 */

#ifndef _EXTRAS_H_
#define _EXTRAS_H_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#include "tipos.h"
typedef char* tMensaje;

void post(tObjeto* objeto,tMensaje* mensaje);
void get(tObjeto* objeto,tMensaje** mensaje);
void imprimir(char* mensaje);
void objeto_crear(tObjeto* objeto,driver_t Uart_driver);



#endif
