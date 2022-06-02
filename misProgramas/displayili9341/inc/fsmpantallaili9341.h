/*
 * fsmpantalaili9341.h
 *  *
 *  Created on: 17/11/2020
 *      Author: Sebastian Herrera
 */

#ifndef _FSMPANTALLAILI9341_H_
#define _FSMPANTALLAILI9341_H_
#include "sapi.h"
#include "tm_stm32f4_ili9341.h"

#ifdef __cplusplus
extern "C" {
#endif


// FSM functions
void fsmDisplayili9341Error( void );
void fsmDisplayili9341Init( void );
void fsmDisplayili9341( void );


/*==================[c++]====================================================*/
  #ifdef __cplusplus
  }
  #endif

  /*==================[end of file]============================================*/
  #endif



