/*=====[Module Name]===========================================================
 * Copyright 2020 Luis Mariano Campos <lmarianocampos@gmail.com>
 * All rights reserved.
 * License: license text or at least name and link
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2020/05/09
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"
#include  "linearsensor.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define CUARENTAMS  40
/*=====[Public function-like macros]=========================================*/

//#define printInt(printer,number) printIntFormat((printer),(number),(DEC_FORMAT))

//#define printlnString(printer,string);   {printString((printer),(string));\
                                          printEnter((printer));}

/*=====[Definitions of public data types]====================================*/
//typedef uint8_t bool_t;

typedef enum {
		 STATE_PULSE_UP,
		 STATE_PULSE_DOWN,
		 STATE_PULSE_FALLING,
		 STATE_PULSE_RISING
} fsmPulseState_t;

typedef struct {
	gpioMap_t inputPulse;
	fsmPulseState_t state;
	delay_t delay;
}dbn_t;
dbn_t debounce;

/*=====[Prototypes (declarations) of public functions]=======================*/

void fsmPulseError( dbn_t* debounce);
void fsmPulseInit( dbn_t* debounce,gpioMap_t inputPulse);
void fsmPulseUpdate( dbn_t* debounce);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/



/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _DEBOUNCE_H_ */
