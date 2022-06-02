#ifndef _STEP_MOTOR
#define _STEP_MOTOR

#include "sapi.h"
#include "sapi_timer.h"

#define CYCLE_PULSE 24000  //30 mseg
#define DUTY_CYCLE  5000

//cantidad de estructuras de motor PaP que puede almacenar el arreglo
//en caso de usar un arreglo de estructura para la conexión de más de un motor PaP
#define STEPPER_MOTOR_TOTAL 2

typedef enum {
	STEPPER_MOTOR_0, STEPPER_MOTOR_1
} stepperMotorMap_t;

typedef enum {
	STEPPER_RIGHT_OPEN, STEPPER_LEFT_CLOSE
} stepperMotorDirection_t;

typedef enum {
	STEPPER_DISABLE, STEPPER_ENABLE
} stepperMotorEnable_t;

typedef enum {
	STEPPER_AXIS_NO_MOVE, STEPPER_AXIS_LAST_MOVE, STEPPER_AXIS_IN_MOVE
} stepperMotorMove_t;

typedef enum {
	RESOLUTION_FULL_STEP,
	RESOLUTION_HALF_STEP,
	RESOLUTION_4_STEP,
	RESOLUTION_8_STEP,
	RESOLUTION_16_STEP,
	RESOLUTION_32_STEP
} stepperMotorMicroSteps_t;

typedef struct {
	uint32_t stepsPerRevolution;
	uint32_t lastNumberOfSteps;
	float LastAnglePosition;

	gpioMap_t pulsePin;
	gpioMap_t directionPin;
	gpioMap_t enablePin;

	gpioMap_t microStepsM0Pin;
	gpioMap_t microStepsM1Pin;
	gpioMap_t microStepsM2Pin;

	stepperMotorDirection_t direction;

	stepperMotorEnable_t isEnable;
	stepperMotorMove_t isMoveAxis;

	float rpm;
	float stepAngle;

} stepperMotor_t;

stepperMotor_t stepper;
static uint32_t stepCount;

//en caso de usar un arreglo de estructura para la conexión de más de un motor PaP
stepperMotor_t stepperM[STEPPER_MOTOR_TOTAL];

// esta función inicializa la estructura del motor paso a paso y el timer
void stepperMotorInit(stepperMotor_t *stepper, uint32_t stepsPerRevolution,
		gpioMap_t pulsePin, gpioMap_t directionPin, gpioMap_t enablePin,
		gpioMap_t microStepsM0Pin, gpioMap_t microStepsM1Pin,
		gpioMap_t microStepsM2Pin, float stepAngle);

//esta función es para establecer la velocidad del eje del motro PaP
void stepperMotorSetSpeed(stepperMotor_t * stepper, float rpm);

//esta función permite leer la velocidad del eje del motor PaP
float stepperMotorGetSpeed(stepperMotor_t * stepper);

//esta función permite establecer la resolución de los microspasos
void stepperMotorSetMicroSteps(stepperMotor_t *stepper, bool_t m0MicroStep,
		bool_t m1MicroStep, bool_t m2MicroStep);

//esta función permite leer el microsteps que tiene configurado el motor.
stepperMotorMicroSteps_t stepperMotorGetMicroStepes(stepperMotor_t * stepper);

// esta función permite establecer el sentido de giro del motor PaP
void stepperMotorSetDirection(stepperMotor_t * stepper,
		stepperMotorDirection_t dir);

//esta función permite leer el sentido de giro del eje del motor PaP
stepperMotorDirection_t stepperMotorGetDirection(stepperMotor_t * stepper);

//esta función permite Habilitar el motor PaP
void stepperMotorEnable(stepperMotor_t * stepper, stepperMotorEnable_t enable);

//esta función permite leer si el motor PaP está habilitado
stepperMotorEnable_t stepperMotorGetEnable(stepperMotor_t * stepper);

//esta función permite mover el eje del motor una cierta cantidad de pasos
void stepperMotorMoveSteps(stepperMotor_t * stepper, uint32_t numberOfSteps);

//esta función permite mover el eje del motor PaP una cierta cantidad vueltas completas
void stepperMotorMoveTurns(stepperMotor_t * stepper, uint32_t numberOfTurns);

//Esta función permite girar el eje del motor un determinado ángulo
void stepperMotorMoveAngle(stepperMotor_t * stepper, float angle);

//Esta función detiene el movimiento del eje del motor
void stepperMotorStopMoveSteps(stepperMotor_t * stepper);

// Esta función permite establecer el angulo o la ultima cantidad de pulsos que se proporciono al Motor PaP
void stepperMotorSetAngle(stepperMotor_t* stepper,stepperMotorDirection_t dir );

// Esta función retorna el angulo de apertura de la valvula
float stepperMotorGetAngle(stepperMotor_t* stepper);

#endif
