/*=============================================================================
 * Copyright (c) 2019, Agustin Curcio Berardi <acurber91@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2019/09/22
 * Version: v1.0
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"

#include "mayusc.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

void tickTask( void* pvParameters )
{
    while( TRUE )
    {
        // Una tarea muy bloqueante para demostrar que la interrupcion funcina
        gpioToggle( LEDG );
        vTaskDelay( 100/portTICK_RATE_MS );
    }
}

//capTask recibe como parametro los datos de una UART con el driver inicializado
//se queda esperando a que llegue un paquete, lo procesa y lo envia por la UART
void checkTask( void* pvParameters )
{
    char *TXbuffer;
    driver_t *selectedUart;
    selectedUart = ( driver_t * )pvParameters;
    while ( TRUE )
    {
    	gpioToggle( LED3 );

        TXbuffer = waitPacket( selectedUart ); //espero un paquete
        if ( TXbuffer!=NULL )	//si recibo null es porque ocurrio un error en la comunicacion
        {
            Check_packet( TXbuffer ); //Analizo si el paquete es valido y luego lo paso a mayuscula si corresponde o cargo un mensaje de error
            packetTX( selectedUart,TXbuffer ); //envio el paquete procesado o el mensaje de error.
        }

        vTaskDelay( 100/portTICK_RATE_MS );
    }
}



void Check_packet( char* string )
{
    int i=2;//indice arranca en uno porque el primer byte es el tamaño del string

    int largo = string[0];

    if (string[1] != '(')
    {
    	strcpy( string, " ERROR_B" );
		string[0]=7; //Indico cuantos bytes tiene el string
		return;
    }

    if (string[largo] != ')')
        {
        	strcpy( string, " ERROR_E" );
    		string[0]=7; //Indico cuantos bytes tiene el string
    		return;
        }

    while ( largo > i+1 )
    {

        if ( ( string[i]>='0' )&&( string[i]<='9' ) ) //si el caracter esta entre a y z los paso a mayuscula
        {
            //string[i]=string[i]+( 'A'-'a' );
        }
        else
        {
            strcpy( string, " ERROR_N" );
            string[0]=7; //Indico cuantos bytes tiene el string
            return;
        }
        i++;
    }
}


/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

