
#include "capa2.h"
#include <stdlib.h>
#include <string.h>

/*=====[Prototypes (declarations) of private functions]======================*/
static void msj_process(tMensaje* p);

/*=====[Implementations of public functions]=================================*/

void appC3(driver_t* Uart_driver){

	tMensaje* p = NULL;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xQueueReceive( Uart_driver->onRxQueue, &p ,portMAX_DELAY );
	msj_process(p);
	packetTX( Uart_driver, p);

}

void msj_process(tMensaje* p){

	int i;
	i=1;

    if(p[0] == 'm'){

    	while(p[i]!= '\0'){

    		if(((p[i] >= 'A' )&&( p[i] <= 'Z' )) || ((p[i] >= 'a' )&&( p[i]<= 'z' ))){

    			p[i] = p[i]+( 'a'-'A' );
    			i++;

			}else{
				strcpy( p, " ERROR_1" );
				return;
			}
        }
	} else if (p[0] == 'M') {

    	while(p[i]!= '\0'){

    		if(((p[i] >= 'a' )&&( p[i]<= 'z' )) || ((p[i] >= 'A' )&&( p[i] <= 'Z' )) ){

    			p[i] = p[i] + ( 'A'-'a' );
    			i++;

			}else{
				strcpy( p, " ERROR_1\r\n" );
				return;
			}
        }
    }else{
    	strcpy( p, " ERROR_2\r\n" );
		return;
    }
}



