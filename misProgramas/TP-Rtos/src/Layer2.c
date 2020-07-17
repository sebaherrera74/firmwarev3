/*
 * Layer2.c
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */
#include <string.h>

#include "Layer2.h"
#include "crc8.h"
#include "types.h"
#include "qmpool.h"
/* ---------- Prototype of private function ---------- */

/**
 * @brief Convert value of hexadecimal to string.
 *
 * @param[in]  crc Value on hexadecimal.
 * @param[out] string String of exit.
 */
static void HexaToString(uint8_t crc, uint8_t *string);
/* --------------------------------------------------- */

/**
 * @brief Convert from character to hexadecimal.
 *
 * @param[in] character Character of input.
 *
 * \return Return number on hexadecimal
 */
static uint8_t CharToHexa(uint8_t character);

/**
 * @brief Call CRC function.
 *
 * @param[in] character pointer of input.
 *
 * \return Return i it it is ok otherwise 0
 */
static uint8_t CheckCrc(Layer2_t * const msg);

/**
 * @brief Call CRC function.
 *
 * @param[in] character pointer of input.
 *
 * \return Return i it it is ok otherwise 0
 */
static uint8_t crc_calculation(Layer2_t * msg, uint8_t crc);

/* ------- Implementation of private function -------- */
static void HexaToString(uint8_t crc, uint8_t *string)
{
	uint8_t crc_LSB = 0, crc_MSB = 0, index = 0;

	crc_LSB = crc & 0x0F;
	crc_MSB = (crc & 0xF0) >> 4;

	string[0] = "0123456789ABCDEF"[crc_MSB];
	string[1] = "0123456789ABCDEF"[crc_LSB];
}

static uint8_t CharToHexa(uint8_t character)
{
	uint8_t value;

	/* Convert to capital letter */
	if (character >= 'a' && character <= 'f')
	{
		character -= 0x20;
	}

	switch(character)
	{
		case '1':
		{
			value = 0x01;
		}
		break;

		case '2':
		{
			value = 0x02;
		}
		break;

		case '3':
		{
			value = 0x03;
		}
		break;

		case '4':
		{
			value = 0x04;
		}
		break;

		case '5':
		{
			value = 0x05;
		}
		break;

		case '6':
		{
			value = 0x06;
		}
		break;

		case '7':
		{
			value = 0x07;
		}
		break;

		case '8':
		{
			value = 0x08;
		}
		break;

		case '9':
		{
			value = 0x09;
		}
		break;

		case 'A':
		{
			value = 0x0A;
		}
		break;

		case 'B':
		{
			value = 0x0B;
		}
		break;

		case 'C':
		{
			value = 0x0C;
		}
		break;

		case 'D':
		{
			value = 0x0D;
		}
		break;

		case 'E':
		{
			value = 0x0E;
		}
		break;

		case 'F':
		{
			value = 0x0F;
		}
		break;

		default:
			value = 0x00;
	}

	return value;
}



/* ------- Implementation of public function --------- */
uint8_t Layer2_constructor(Layer2_t * me, uint8_t mode){
	//if(inHandlerMode())
	if (mode == HANDLER_MODE_ON){
		if(me->message == NULL)
			me->message = (Message_t *)QMPool_get_fromISR(memoryBlock_aux, 0);//pedimos memoria
	}
	else{
		if(me->message == NULL)
			me->message = (Message_t *)QMPool_get(memoryBlock_aux, 0);//pedimos memoria
	}
	if(me->message == NULL)
		return 0;
	return 1;
}

void Layer2_destructor(Layer2_t * me, uint8_t mode){

	if (mode == HANDLER_MODE_ON){
		QMPool_put_fromISR( memoryBlock_aux, (Message_t *)me->message );
	}
	else{
		QMPool_put( memoryBlock_aux, (Message_t *)me->message );
	}
	me->code = '\0';
	me->message = NULL;
	strncpy((char *)me->crc, '\0', 2);
}


uint8_t Serialize_BuildMessage(Layer2_t *information)
{
	return 1;
}

uint8_t Deserialize_ParseMessage(Layer2_t *me, Message_t *information)
{
	volatile uint32_t index, length;
	uint8_t crc = 0, seed = 0, crcMsg = 0;

	if (information == NULL)
	{
		return 0;
	}

	/**
	 * Format of message:
	 *
	 *      (   Code    Data      CRC       )
	 *      ^	  ^		 ^ 	       ^        ^
	 *      |     |      |         |        |
	 *      |     |      |         |        |
	 *     STX Command  Data  2Characters  ETX
	 */

	//Get length of message from index 0
	length = strlen((char *)information)-1;

	me->code = information[1];
	me->crc[0] = information[length - 1];
	me->crc[1] = information[length - 2];

	index = length-2;
	// Get most significant nibble of CRC
	crcMsg = CharToHexa(*(information + index++)) << 4;
	// Get less significant nibble of CRC and total CRC
	crcMsg |= CharToHexa(*(information + index));

	// Calculate CRC of current message
	seed = crc8_init();
	crc = crc8_calc(seed, (void *)(information + 1), length-3);

	// Validate of message
	if (crc != crcMsg)
	{
		return 0;
	}

	// Shift character to left on 1 position
	for(index = 0; index < length-1; index++)
	{
		*(me->message + index) = *(information + index + 2);
	}

	// Delete CRC and character )
	strncpy((char *)(me->message+length-4), '\0', 3);


	return 1;
}

static uint8_t CheckCrc(Layer2_t * const msg){
	uint8_t crc = 0, seed = 0, crcMsg = 0;
	seed = crc8_init();

	crc = crc_calculation(msg, seed);

	crcMsg = CharToHexa(msg->crc[1]) << 4;
	// Get less significant nibble of CRC and total CRC
	crcMsg |= CharToHexa(msg->crc[0]);
	/*if (crc != crcMsg)
	{
		return 0;
	}*/

	return 1;

}

static uint8_t crc_calculation(Layer2_t * msg, uint8_t crc){
	volatile uint8_t seed;
	uint8_t i;
	volatile uint8_t lenght = strlen((char *)msg->message);

	seed = crc8_calc(crc, &msg->code, 1);

	for(i = 0; i < lenght; i++ ){
		seed = crc8_calc(seed, (char *)&msg->message[i], 1);
	}
	return seed;
}
