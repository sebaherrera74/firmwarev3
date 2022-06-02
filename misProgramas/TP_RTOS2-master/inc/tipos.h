
#ifndef TP_RTOS2_INC_TIPOS_H_
#define TP_RTOS2_INC_TIPOS_H_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"
#include "driver.h"

#define PACKET_SIZE  LARGO
#define POOL_TOTAL_BLOCKS 5
#define POOL_SIZE POOL_TOTAL_BLOCKS*PACKET_SIZE //Tamanio total del pool

typedef struct{

	QueueHandle_t cola;
} tObjeto;



#endif /* TP_RTOS2_INC_TIPOS_H_ */
