#include "processcommand.h"

void processCommandBufferAux(void) {

	if (buffer == BUFFER_FULL) {
		char bufferAux[SIZE_TOTAL_BUFFER];
		for (uint8_t i = 0; bufferRx[i] != '\0'; i++) {
			bufferAux[i] = bufferRx[i];
			buffer = BUFFER_EMPTY;

		}

		switch (*bufferAux) {
		case 'M':
			switch (*(bufferAux + 1)) {
			case 'E': // habilitar el motor.
				stepperMotorEnable(&stepper, STEPPER_ENABLE);
				processCommandInvalid(COMMAND_VALID);
				break;
			case 'D': //deshabilitar el motor.
				stepperMotorEnable(&stepper, STEPPER_DISABLE);
				processCommandInvalid(COMMAND_VALID);
				break;
			case 'M':
					//establece los micropasos.
					// hacer la función de microstep
					switch(*(bufferAux + 2)){
					case 'S':
							switch(*(bufferAux + 3)){
							case 'F':
								processCommandSetMicroSteps(RESOLUTION_FULL_STEP);
								processCommandInvalid(COMMAND_VALID);
								break;
							case 'H':
								processCommandSetMicroSteps(RESOLUTION_HALF_STEP);
								processCommandInvalid(COMMAND_VALID);
								break;
							case '0':
									switch(*(bufferAux + 4)){
									case '4':
										processCommandSetMicroSteps(RESOLUTION_4_STEP);
										processCommandInvalid(COMMAND_VALID);
										break;
									case '8':
										processCommandSetMicroSteps(RESOLUTION_8_STEP);
										processCommandInvalid(COMMAND_VALID);
										break;
									default:processCommandInvalid(COMMAND_INVALID);
										//error4(); error en trama posición 4(microstep 4 u 8)

										break;
									}
								break;
							case '1':
									if(*(bufferAux+4)=='6'){
										processCommandSetMicroSteps(RESOLUTION_16_STEP);
										processCommandInvalid(COMMAND_VALID);
									}
									else processCommandInvalid(COMMAND_INVALID);
								break;
							case '3':
								if(*(bufferAux+4)=='2'){
									processCommandSetMicroSteps(RESOLUTION_32_STEP);
									processCommandInvalid(COMMAND_VALID);
								}
								else processCommandInvalid(COMMAND_INVALID);
								break;
							default: processCommandInvalid(COMMAND_INVALID);
								break;

							}

						break;
					}

				break;
			case 'T':
					//establece el sentido de dirección horario y antihorario.
					switch (*(bufferAux + 2)) {
					case 'A':
						stepperMotorSetDirection(&stepper, STEPPER_RIGHT_OPEN);
						processCommandInvalid(COMMAND_VALID);
						break;
					case 'H':
						stepperMotorSetDirection(&stepper, STEPPER_LEFT_CLOSE);
						processCommandInvalid(COMMAND_VALID);
						break;
					default:processCommandInvalid(COMMAND_INVALID);
					break;
					}
				break;
			case 'S': //establece la cantidad de pasos.
				//TODO verificar eque cuando mando MSSH quiere ejecutar este comanto CORREGIR
				stepperMotorMoveSteps(&stepper, processCommandConverterCaracterToDecimal(bufferAux+2, 4));
				processCommandInvalid(COMMAND_VALID);

				break;
			case 'A': // establece el ángulo de giro.
				stepperMotorMoveAngle(&stepper, processCommandConverterCaracterToDecimal(bufferAux+2, 4));
				processCommandInvalid(COMMAND_VALID);
				break;
			case 'F': //establece la cantidad de vueltas que debe realizar el eje del motor.
				stepperMotorMoveTurns(&stepper,processCommandConverterCaracterToDecimal(bufferAux+3, 3));
				processCommandInvalid(COMMAND_VALID);
				break;
			default: processCommandInvalid(COMMAND_INVALID);// llamar a la función Error1();
				break;
			}
			break;
		default: processCommandInvalid(COMMAND_INVALID);
			break;
		}
	}
}

uint32_t processCommandConverterCaracterToDecimal(char * pointer, uint8_t length){
	    uint32_t num=0;
	    uint32_t digito = 0;
	    uint32_t tam = length;
	    uint32_t expo = 0;
	    uint32_t pot=1;
	    uint32_t i;

	    for(i=tam ;i>0;--i)
	    {
	        digito=(uint32_t) (*(pointer+(i-1))-48);

	        expo = tam-i;
	        pot=1;
	        while( expo > 0)//para sacar la potencia
	        {
	                pot  = pot*10  ;
	                expo = expo-1 ;
	        }
	    num = num+digito*pot;
	    }
	    return num;
}

void processCommandSetMicroSteps(stepperMotorMicroSteps_t microSteps){
	switch(microSteps){
	case RESOLUTION_FULL_STEP:
		stepperMotorSetMicroSteps(&stepper, FALSE,FALSE,FALSE);
		break;
	case RESOLUTION_HALF_STEP:
		stepperMotorSetMicroSteps(&stepper, TRUE,FALSE,FALSE);
			break;
	case RESOLUTION_4_STEP:
		stepperMotorSetMicroSteps(&stepper, FALSE,TRUE,FALSE);
			break;
	case RESOLUTION_8_STEP:
		stepperMotorSetMicroSteps(&stepper, TRUE,TRUE,FALSE);
			break;
	case RESOLUTION_16_STEP:
		stepperMotorSetMicroSteps(&stepper, FALSE,FALSE,TRUE);
			break;
	case RESOLUTION_32_STEP:
		stepperMotorSetMicroSteps(&stepper, TRUE,TRUE,TRUE);
			break;

	}
}

void processCommandInvalid(stateCommand_t sCommand){
	if(sCommand==COMMAND_INVALID) printf("Se recibio un comando Invalido\n");
}

