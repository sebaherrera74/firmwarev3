/*=====[debounce]===========================================================
 * Copyright 2020 LuisMariano Campos <lmarianocampos@gmail.com>
 * All rights reserved.
 * License: license text or at least name and link
 (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2020/05/09
 */

/*=====[Inclusion of own header]=============================================*/

#include "debounce.h"

/*=====[Inclusions of private function dependencies]=========================*/

#include "linearsensor.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/
void fsmPulseError(dbn_t* debounce) {
	debounce->state = STATE_PULSE_UP;
}

void fsmPulseInit(dbn_t* debounce, gpioMap_t inputPulse) {
	// Example:
	// boardInit();          // Initialize hardwaree
	debounce->inputPulse= inputPulse;
	debounce->state = STATE_PULSE_UP;   // Set initial state
	gpioConfig(debounce->inputPulse, GPIO_INPUT);
}
void fsmPulseUpdate(dbn_t* debounce) {

	static bool_t flagFalling = FALSE;
	static bool_t flagRising = FALSE;
	//static uint32_t counterPulse = 0;;

	switch (debounce->state) {
	case STATE_PULSE_UP:
		if (!gpioRead(debounce->inputPulse)) {
			debounce->state = STATE_PULSE_FALLING;
		}
		break;
	case STATE_PULSE_DOWN:
		if (gpioRead(debounce->inputPulse)) {
			debounce->state = STATE_PULSE_RISING;
		}
		break;
	case STATE_PULSE_FALLING:
		if (flagFalling == FALSE) {
			delayConfig(&(debounce->delay), CUARENTAMS);
			flagFalling = TRUE;
		}
		if (delayRead(&(debounce->delay))) {
			if (gpioRead(debounce->inputPulse)) {
				debounce->state = STATE_PULSE_UP;   // fue un rebote
			} else {
				if (!gpioRead(debounce->inputPulse)) {
					debounce->state = STATE_PULSE_DOWN; // Se puso en bajo la señal
														//fue un flanco descendente
				}
			}
			flagFalling = FALSE;
		}
		break;
	case STATE_PULSE_RISING:
		if (flagRising == FALSE) {
			delayConfig(&(debounce->delay),CUARENTAMS);
			flagRising = TRUE;
		}
		if (delayRead(&(debounce->delay))) {
			if (gpioRead(debounce->inputPulse)) {
				debounce->state = STATE_PULSE_UP; // se caputro un flanco ascendente
				switch (stepper.direction) {
						case STEPPER_RIGHT_OPEN:
							linearSensorIncreaseClockWisePulses(&ls);

							//ls.pcClockwise++;
							printf("Cantidad de Pulsos Horario:%d\n",linearSensorGetPulsesClockWise(& ls));

							break;
						case STEPPER_LEFT_CLOSE:
							//ls.pcCounterClockWise++;
							linearSensorIncreasePulsesCounterClockWise (&ls);
							printf("Cantidad de Pulsos Antihorario:%d\n",linearSensorGetPulsesCounterClockWise(& ls));
							break;
						default: // llamar a una función de error
							break;
						}
				//counterPulse++;
				//printf("Pulso:%d\n",counterPulse);
			} else {
				if (!gpioRead(debounce->inputPulse)) {
					debounce->state = STATE_PULSE_DOWN;   // fue un rebote
				}
			}
			flagRising = FALSE;
		}
		break;
	default:
		fsmPulseError(debounce);
		break;

	}
}
/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

