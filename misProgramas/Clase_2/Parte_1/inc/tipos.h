
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"

#define N_TECLAS	2

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
	gpioMap_t led;				//config

	TickType_t tiempo_medido;	//variables
	fsmButtonState_t fsmButtonState;//variables
	TickType_t tiempo_down;		//variables

	uint8_t contFalling  ;		//variables
	uint8_t contRising  ;		//variables

	SemaphoreHandle_t sem_tec_pulsada;
} tLedTecla;

QueueHandle_t cola_1;

void fsmButtonUpdate( tLedTecla* config );
void post(char* mensaje,QueueHandle_t cola);
void get(char* mensaje,QueueHandle_t cola);
