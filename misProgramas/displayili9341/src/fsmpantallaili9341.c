#include "fsmpantallaili9341.h"
#include "funcionespantalla.h"

// FSM DECLARATIONS

// FSM state names
typedef enum{
   STATE_switch_posicion_cero,       //Estado del switch que este ubicado en la posicion cero
   STATE_tipos_ensayos ,             //pantalla de los tipos de ensayos a realizar
   STATE_ensayo_barrido,             //pantalla de ensayo de barrido
   STATE_ensayo_longitudonda         //pantalla de ensaya de una longitud de onda determinada
} fsmState_t;


// FSM IMPLEMENTATIONS

// Variable that hold the current state
fsmState_t fsmState;

// FSM Error Handler Function
void fsmError( void )
{
   // Error handler, example, restart FSM:
   // fsmState = STATE_INIT;
}

// FSM Initialize Function
void fsmInit( void )
{
   // Example:
   // boardInit();          // Initialize hardwaree
   fsmState = STATE_switch_posicion_cero;   // Set initial state
}

// FSM Update Sate Function
void fsmUpdate( void ) {
	switch( fsmState ) {

	case STATE_switch_posicion_cero:
		posicioncero();
		if(gpioRead(TEC1)){  //Aqui vamos a simular el switch
			fsmState=STATE_tipos_ensayos;
			cambiofondo(ILI9341_COLOR_RED);
		}
    break;


    case STATE_tipos_ensayos:
    	tipoensayos();
    	if (!gpioRead(TEC4)){
    		fsmState=STATE_ensayo_barrido;
    		cambiofondo(ILI9341_COLOR_GREEN);

    	}
    	if (!gpioRead(TEC1)){
    		fsmState=STATE_ensayo_longitudonda;
    		cambiofondo(ILI9341_COLOR_GREEN2);


    	}
    break;

    case STATE_ensayo_barrido:
    	ensayobarrido();
     	      /* UPDATE OUTPUTS */
               // Code for update outputs...

               /* CHECK TRANSITION CONDITIONS */
               // Code for check transitions (some ifs)...
    break;

    case STATE_ensayo_longitudonda:
        	ensayolongitudonda();
         	      /* UPDATE OUTPUTS */
                   // Code for update outputs...

                   /* CHECK TRANSITION CONDITIONS */
                   // Code for check transitions (some ifs)...
        break;

    default:
    	fsmError();
   break;
   }
}
