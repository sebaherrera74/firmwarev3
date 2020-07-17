
#ifndef RTOS_2_CLASE_2_PARTE_2_INC_FSM_H_
#define RTOS_2_CLASE_2_PARTE_2_INC_FSM_H_

#define DEBOUNCE_TIME 40
#define FSM_MSG "TEC%d T%dms"

void fsmButtonError( tLedTecla* config );
void fsmButtonInit( tLedTecla* config,uint8_t i );
void buttonPressed( tLedTecla* config );
void buttonReleased( tLedTecla* config );
void fsmButtonUpdate( tLedTecla* config );

#endif /* RTOS_2_CLASE_2_PARTE_2_INC_FSM_H_ */
