/*
 * Layer2.h
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */

#ifndef PROGRAMS_RTOS2_INC_LAYER2_H_
#define PROGRAMS_RTOS2_INC_LAYER2_H_

#include "FreeRTOS.h"
#include "sapi.h"

#define STRING_STX	"("
#define STRING_ETX	")"

/// Variable used to store message
typedef uint8_t Message_t;

/**
 * \struct Layer2_t
 *
 * @brief Structure that contain all data to serialize and deserialize
 * message
 */
typedef struct
{
	uint8_t crc[2];			/*< CRC of message */
	uint8_t code;			/*< Command of message */
	Message_t *message;		/*< Message received */
}Layer2_t;


/**
 * @brief Function to initialize asign memory pool for Layer2_t
 *
 * @param[in] mode	mode of operation, in handler mode or in normal mode
 * @param[in,out] layer Pointer to structure Layer2_t.
 */
uint8_t Layer2_constructor(Layer2_t * me, uint8_t mode);

/**
 * @brief Function to deinitialize asignation of memory pool for Layer2_t
 *
 * @param[in] mode	mode of operation, in handler mode or in normal mode
 * @param[in,out] layer Pointer to structure Layer2_t.
 */
void Layer2_destructor(Layer2_t * me, uint8_t mode);

/**
 * @brief Function to build message, this is calculate CRC and put
 *        character of of STX and ETX.
 *
 * @param[in,out]  information Pointer of structure Serialize_t where there are
 * all part of message and final message.
 *
 * \return Return 1 if operation was success or 0 in other case.
 */
uint8_t Serialize_BuildMessage(Layer2_t *information);


/**
 * @brief Function to parse message received, verify CRC and separate all
 *        part of message.
 * @param[in] information Pointer of structure Serialize_t where there are
 * all part of message
 * @param[out] me pointer of structure where data will be stored
 *
 * \return Return 1 if operation was success in other case 0.
 */
uint8_t Deserialize_ParseMessage(Layer2_t *me, Message_t *information);

#endif /* PROGRAMS_RTOS2_INC_LAYER2_H_ */
