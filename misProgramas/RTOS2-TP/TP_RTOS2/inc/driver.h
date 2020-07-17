/*=============================================================================
 * Copyright (c) 2019, Gonzalo Gontad <gonzalogontad@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2019/09/29
 * Version: v1.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef DRIVER_H_
#define DRIVER_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "string.h"
#include "sapi.h"
#include "timers.h"
#include "semphr.h"
#include "qmpool.h"
/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define PROTOCOL_TIMEOUT pdMS_TO_TICKS(3)
#define LARGO 100
#define PACKET_SIZE 127
#define BLOCK_SIZE (PACKET_SIZE+1)
#define POOL_TOTAL_BLOCKS  4  //Cantidad de bloques en el pool de memoria
#define POOL_SIZE    POOL_TOTAL_BLOCKS*BLOCK_SIZE //tamaño total del pool


/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/
typedef char* tMensaje;

typedef struct
{
    uartMap_t uartValue; //Numero de UART a utilizar
    uint32_t baudRate; //velocidad de transmision
    char *txPoolSto;   //Puntero al segmento de memoria que albergara el pool para trasnmision
    QMPool txPool  ;   //memory pool (contiene la informacion que necesita la biblioteca qmpool.h)
    //TimerHandle_t onTxTimeOut; //Timer para el timeout de la transmision
    //TimerHandle_t onRxTimeOut; //Timer para el timeout de la recepcion
    char *dato_tx; //Bloque de memoria de la transmision en curso
    char *dato_rx; //Bloque de memoria de la recepcion
    uint8_t txCounter; //contador de bytes transmitidos
    uint8_t rxCounter; //contador de bytes recibidos
    QueueHandle_t onTxQueue; //cola de transmision (por esta cola llegan los bloques de memoria a transmitir)
    QueueHandle_t onRxQueue; //cola de recepcion (por esta cola llegan los bloques de memoria recibidos)
    uint8_t txLen; //longitud del paquete en transmision
    uint8_t rxLen; //longitud del paquete recibido
    int rxChecksum; //checksum calculado
    SemaphoreHandle_t mutex;    //no va
    SemaphoreHandle_t permiso;  //no va
} driver_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

bool_t	driverInit	( driver_t* );
char*	getBuffer 	( driver_t* selectedUart );
void 	packetTX 	( driver_t* Uart_driver,char *txBlock );
char* 	waitPacket	( driver_t* );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif
