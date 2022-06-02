#ifndef  _UARTINTERR
#define  _UARTINTERR

#define SIZE_TOTAL_BUFFER 128
#define BUAD_RATE_9600    9600
#define BUAD_RATE_115200  115200

#include "sapi.h"

typedef enum {
	COMMAND_ENABLE_MOTOR,
	COMMAND_DISABLE_MOTOR,
	COMMAND_ORIENTATION_HORARIO,
	COMMAND_ORIENTATION_ANTIHORARIO,
	COMMAND_MICRO_STEPES,
	COMMAND_STEPS,
	COMMAND_TURNS,
	COMMAND_ANGLE,
	COMMAND_ERROR
} uartComand_t;

typedef enum {
	BUFFER_EMPTY, BUFFER_INCOMPLETE, BUFFER_FULL
} stateBuffer_t;

stateBuffer_t buffer;


char bufferRx[SIZE_TOTAL_BUFFER];

void configuraUart(void);
void onRx(void *nousado);

#endif
