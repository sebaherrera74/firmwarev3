
/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef DRIVER_H_
#define DRIVER_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "sapi.h"
#include "timers.h"
#include "semphr.h"

#include "qmpool.h"

/*=====[Definition macros of public constants]===============================*/

#define PROTOCOL_TIMEOUT pdMS_TO_TICKS(50)//<-------------------------------R_C2-7
#define LARGO 100
#define PACKET_SIZE  LARGO
#define POOL_TOTAL_BLOCKS 5
#define POOL_SIZE POOL_TOTAL_BLOCKS*PACKET_SIZE //Tamanio total del pool

#define SOM		'('//caracter inicio de trama -----------------------------------------------
#define EOM		')'//caracter fin de trama	-------------------------------------------------
#define firstIncomingByte	0
#define MENSAJENOACK  "LA TRAMA TIENE UN ACK INCORRECTO"
#define MENSAJEOVERFLOW "LA TRAMA SUPERA EL LIMITE DE RECEPCION"
#define MENSAJEMEMORYPOOLFULL "LA POOL DE MEMORIA ESTA LLENA"
#define MENSAJETIMEOUT "SE SUPERO EL TIMEPO DE ESPERA DE LA TRAMA"

/*=====[Definitions of public data types]====================================*/
typedef uint8_t tMensaje;

typedef struct
{
    uartMap_t uartValue; //Numero de UART a utilizar
    uint32_t baudRate; //velocidad de transmision
    TimerHandle_t onTxTimeOut; //Timer para el timeout de la transmision
    TimerHandle_t onRxTimeOut; //Timer para el timeout de la recepcion
	tMensaje *dato_tx; //Bloque de memoria de la transmision en curso
	tMensaje *dato_rx; //Bloque de memoria de la recepcion
    uint8_t txCounter; //contador de bytes transmitidos
    uint8_t rxCounter; //contador de bytes recibidos
    QueueHandle_t onTxQueue; //cola de transmision (por esta cola llegan los bloques de memoria a transmitir)
    QueueHandle_t onRxQueue; //cola de recepcion (por esta cola llegan los bloques de memoria recibidos)
    uint8_t txLen; //longitud del paquete en transmision
    uint8_t rxLen; //longitud del paquete recibido
    tMensaje* Pool_puntero; //puntero al segmento de memoria que albergara el pool
    QMPool Pool_memoria; //memory pool (contienen la informacion que necesita la biblioteca qmpool.h
} driver_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

bool_t driverInit( driver_t* );
void packetTX( driver_t* selectedUart, tMensaje* mensaje);
void imprimir(char* mensaje);
bool_t txInterruptEnable( driver_t* );
bool_t rxInterruptEnable( driver_t* );
void onTxTimeOutCallback( TimerHandle_t );
void onRxTimeOutCallback( TimerHandle_t );

#endif
