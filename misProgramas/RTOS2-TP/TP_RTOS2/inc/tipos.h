/*
 * tipos.h
 *
 *  Created on: Jul 7, 2020
 *      Author: felipe
 */

#ifndef MITADINTENTOS_TP_RTOS2_INC_TIPOS_H_
#define MITADINTENTOS_TP_RTOS2_INC_TIPOS_H_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"
#include "driver.h"






typedef struct
{
	QueueHandle_t cola;
} tObjeto;



#endif /* MITADINTENTOS_TP_RTOS2_INC_TIPOS_H_ */
