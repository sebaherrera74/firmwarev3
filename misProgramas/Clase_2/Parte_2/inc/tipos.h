#ifndef RTOS_2_CLASE_2_PARTE_2_INC_TIPOS_H_
#define RTOS_2_CLASE_2_PARTE_2_INC_TIPOS_H_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#define L_MSG  50
#define L_QUEUE 10

static gpioMap_t  teclas[]   = {TEC1,TEC2};

#define N_TECLAS  sizeof(teclas)/sizeof(gpioMap_t)  // 4

DEBUG_PRINT_ENABLE;

typedef enum
{
    STATE_BUTTON_UP,
	STATE_BUTTON_DOWN,
	STATE_BUTTON_FALLING,
	STATE_BUTTON_RISING
} fsmButtonState_t;

typedef struct
{
	gpioMap_t tecla;			//config

	TickType_t tiempo_medido;	//variables
	fsmButtonState_t fsmButtonState;//variables
	TickType_t tiempo_down;		//variables

	uint8_t contFalling  ;		//variables
	uint8_t contRising  ;		//variables
} tLedTecla;

typedef struct
{
	QueueHandle_t cola;
} tObjeto;

#endif
