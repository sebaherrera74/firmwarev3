#ifndef LINEARSENSOR_H
#define LINEARSENSOR_H

#include "sapi.h"
#include "stepmotor.h"

#define CANAL3 3
#define PORT2  2 //corresponde al GPIO LCD2
#define PIN5   5 //corresponde al GPIO LCD2

#define PORT3  3 //corresponde al GPIO6
#define PIN6   6 //corresponde al GPIO6

#define PORT0  0 //corresponde a la TEC 1
#define PIN4   4 //corresponde a la TEC 1

typedef struct{

	uint32_t pcClockwise;
	uint32_t pcCounterClockWise;
	float anglePosition; //va estar en grados

}linearSensor_t;

linearSensor_t ls;

//esta función configura el input capture
void configInterruptIRQ(void);

// inicia el el objeto sensor lineal
void linearSensorInit(linearSensor_t* ls,uint32_t pcAntihorario,uint32_t pcHorario,float position);

// esta función establece la posición correspondiente al filo de la compuerta
void linearSensorSetPosition(linearSensor_t* ls,stepperMotor_t* stepper);

// esta función devuelve la posición del filo de la compuerta
uint32_t linearSensorGetPosition(linearSensor_t* ls);

// esta función establece la disctancia que que recorre el filo de la compuerta al girar un paso el eje del Motor PaP
//void linearSensorSetDistancePerPulse(linearSensor_t* ls, float dPerStep);

//esta función retorna la distancia por pulso
float linearSensorGetDistancePerPulse(linearSensor_t* ls);

// esta función retorna el valor de los pulsos que se contaron en sentido horario
uint32_t linearSensorGetPulsesClockWise(linearSensor_t* ls);

// esta función retorna el valor de los pulsos que se contaron en sentido Antihorario
uint32_t linearSensorGetPulsesCounterClockWise(linearSensor_t* ls);

// esta función permite incrementar en uno los pulsos correspondiente al sentido horario
void linearSensorIncreaseClockWisePulses(linearSensor_t* ls);

// esta función permite incrementar en uno los pulsos correspondiente al sentido antihorario
void linearSensorIncreasePulsesCounterClockWise (linearSensor_t*ls);

#endif
