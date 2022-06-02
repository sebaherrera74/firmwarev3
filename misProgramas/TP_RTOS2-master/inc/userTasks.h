
#ifndef __USER_TASKS_H__
#define __USER_TASKS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "capa2.h"
#include "capa3.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"

#include "driver.h"

/*=====[Prototypes (declarations) of public functions]=======================*/

void led_task( void* taskParmPtr );  // Task declaration
void uart_task( void* pvParameters );	//Tarea de mayusculizacion
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);


#endif /* __USER_TASKS_H__ */
