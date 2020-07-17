#ifndef RTOS_2_CLASE_2_PARTE_2_INC_TAREAS_H_
#define RTOS_2_CLASE_2_PARTE_2_INC_TAREAS_H_

#include "tipos.h"

#define MSG_LED "LEDB ON"
#define MALLOC_ERROR "Memoria insuficiente."

void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);
void Tarea_A( void* taskParmPtr );
void Tarea_B( void* taskParmPtr );
void Tarea_C( void* taskParmPtr );
void Tarea_D( void* taskParmPtr );

#endif /* RTOS_2_CLASE_2_PARTE_2_INC_TAREAS_H_ */
