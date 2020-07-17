
#include "sapi.h"

#include "tipos.h"
#include "FreeRTOS.h"
#include "task.h"

#include "fsm.h"

/* accion de el evento de tecla pulsada */
void buttonPressed( tLedTecla* config )
{
	config->tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
bool_t buttonReleased( tLedTecla* config )
{
	config->tiempo_medido = xTaskGetTickCount() - config->tiempo_down;

	if (config->tiempo_medido > 0)
	{
		return TRUE;
	}
	return FALSE;
}

void fsmButtonError( tLedTecla* config )
{
	config->fsmButtonState = STATE_BUTTON_UP;
}

void fsmButtonInit( tLedTecla* config,uint8_t i )
{
	config->tecla = teclas[i];
	config->contFalling = 0;
	config->contRising = 0;
	config->fsmButtonState = STATE_BUTTON_UP;  // Set initial state
}

// FSM Update Sate Function
bool_t fsmButtonUpdate( tLedTecla* config )
{
	bool_t estado = FALSE;

    switch( config->fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( config->tecla ) )
            {
            	config->fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( config->contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    buttonPressed(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_UP;
                }

                config->contFalling = 0;
            }

            config->contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( config->tecla ) )
			{
				config->fsmButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( config->contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    estado = buttonReleased(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;
                }
                config->contRising = 0;
            }
            config->contRising++;

            /* LEAVE */
            break;

        default:
            fsmButtonError(config);
            break;
    }
    return estado;
}
