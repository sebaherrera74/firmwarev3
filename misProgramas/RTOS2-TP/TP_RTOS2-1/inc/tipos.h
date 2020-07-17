/*
 * tipos.h
 *
 *  Created on: Jul 7, 2020
 *      Author: felipe
 */

#ifndef _TIPOS_H_
#define _TIPOS_H_

#include "driver.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#define PACKET_SIZE  LARGO
#define POOL_TOTAL_BLOCKS 5
#define POOL_SIZE POOL_TOTAL_BLOCKS*PACKET_SIZE //Tamanio total del pool




typedef struct
{
	QueueHandle_t cola;
} tObjeto;



#endif
