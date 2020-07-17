#ifndef _TAREAS_H_
#define _TAREAS_H_

//#include "tipos.h"

#define MSG_LED "LEDB ON"
#define MALLOC_ERROR "Memoria insuficiente."

void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);
void TareaLed( void* taskParmPtr );
void TareaUART( void* taskParmPtr );


#endif
