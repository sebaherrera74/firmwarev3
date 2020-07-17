/*
 * extras.h
 *
 *  Created on: Jul 7, 2020
 *      Author: felipe
 */

#ifndef MITADINTENTOS_TP_RTOS2_INC_EXTRAS_H_
#define MITADINTENTOS_TP_RTOS2_INC_EXTRAS_H_

typedef char* tMensaje;

void post(tObjeto* objeto,tMensaje* mensaje);
void get(tObjeto* objeto,tMensaje** mensaje);
void imprimir(char* mensaje);
void objeto_crear(tObjeto* objeto,driver_t Uart_driver);



#endif /* MITADINTENTOS_TP_RTOS2_INC_EXTRAS_H_ */
