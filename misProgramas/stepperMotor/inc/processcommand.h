#ifndef _PROCESSCOMMAND
#define _PROCESSCOMMAND

#include "uartinterr.h"
#include "stepmotor.h"

typedef enum {
	COMMAND_VALID, COMMAND_INVALID
} stateCommand_t;



//esta función procesa los comandos que recibieron en el buffer, se hará polling en la función principal  preguntando si el buffer esta lleno.
void processCommandBufferAux(void);

//esta función convierte de caracter a decimal
uint32_t processCommandConverterCaracterToDecimal(char * pointer, uint8_t length);

//Esta función identifica el comando recibido y establece los micropasos llamando a la función de stepperMotor
void processCommandSetMicroSteps(stepperMotorMicroSteps_t microSteps);

// esta función transmite por el puerto serial la recepción de un comando Invalido
void processCommandInvalid(stateCommand_t sCommand);

 #endif
