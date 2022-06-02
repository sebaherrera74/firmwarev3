#include "processcommand.h"
#include "linearsensor.h"
#include "debounce.h"

int main(void) {
	boardConfig();
	configuraUart();
	//configInterruptIRQ();

	printf("Inicia el Firmware...\n");
	printf("PI vale:%f\n",3.14);
	delay_t myDelay;
	delayInit(&myDelay, 10);
	//fsmPulseInit(&debounce, GPIO6);
	stepperMotorInit(&stepper, 100, GPIO2, GPIO1, GPIO0, GPIO3, GPIO4, GPIO5,
			0.9);
	//0.31415 mm por cada pulsos con microstep half
	//linearSensorInit(&ls,0,0,0.0);

	//establezco los Microsteps en Half step
	stepperMotorSetMicroSteps(&stepper, TRUE,FALSE,FALSE);
	while (TRUE) {
		if (delayRead(&myDelay)) {
			processCommandBufferAux();
		}
		//linearSensorSetPosition(&ls, &stepper);
		//fsmPulseUpdate(&debounce);
	}
	return 0;
}
