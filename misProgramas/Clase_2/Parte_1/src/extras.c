#include "tipos.h"

void post(char* mensaje,QueueHandle_t cola)
{
	xQueueSend( cola, mensaje, portMAX_DELAY );
}

void get(char* mensaje,QueueHandle_t cola)
{
	xQueueReceive( cola, mensaje ,portMAX_DELAY );
}

