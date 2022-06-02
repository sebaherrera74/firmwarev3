/*=====[Nombre del programa]===================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */

/*=====[Inclusiones de dependencias de funciones]============================*/


#include "sapi.h"
#include "spi.h"
#include "tm_stm32f4_ili9341.h"
#include "fsmpantallaili9341.h"
#include "funcionespantalla.h"

DEBUG_PRINT_ENABLE;



/*=====[Macros de definición de constantes privadas]=========================*/



/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/

/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/


int main (void)
{

	//int Error_creacion_Colas=0;
	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();
	i2cInit( I2C0, 100000 );

    static char menuinicial[]="Control\nEspectrofotometro";
    uint16_t* x;
    uint16_t* y;

	uartConfig( UART_USB, 115200 );
	uartWriteString( UART_USB,"ILI9341" );

	// Led para dar señal de vida
	gpioWrite( LED3, ON );
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1); //Rota la pantalla forma Horizontal

	TM_ILI9341_Puts(0,0, &menuinicial , &TM_Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Fill(ILI9341_COLOR_BLACK);

	barraColores();



	TM_ILI9341_Fill(ILI9341_CORAL);

	TM_ILI9341_Fill(ILI9341_LIGHTSALMON);
	TM_ILI9341_Fill(ILI9341_LIGHTSLATEGRAY);

	//TM_ILI9341_DrawCircle(160, 120, 20, ILI9341_COLOR_BLACK);
	//TM_ILI9341_DrawCircle(160, 120, 10, ILI9341_COLOR_BLACK);

	TM_ILI9341_DrawCircle_N(160,120 ,20,10, ILI9341_COLOR_BLACK);



	TM_ILI9341_Fill(ILI9341_COLOR_BLACK);



	delayInaccurateMs(500);
	TM_ILI9341_Fill(ILI9341_COLOR_RED);

	arc(160, 120, 20, ILI9341_COLOR_BLACK);

	TM_ILI9341_DrawFilledCircle(160, 120, 20, ILI9341_COLOR_BLACK);
	TM_ILI9341_DrawFilledCircle(160, 120, 10, ILI9341_COLOR_RED);

	//Fondo color rojo
    char S='S'; //caracter en ASCii

	//TM_ILI9341_DrawFilledCircle(120, 120, 100, ILI9341_COLOR_BLACK);
	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE ) {
		fsmUpdate( );
		TM_ILI9341_DisplayOff();






/*		TM_ILI9341_Puts(2, 0, " Longitud onda especifica", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);
		TM_ILI9341_Puts(2, 40, " Barrido Longitud Onda", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);

		if (!gpioRead( TEC4 )){
			TM_ILI9341_Puts(2, 0, " Longitud onda especifica", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
			//delayInaccurateMs(500);
			delay(1000);

		}

		//delayInaccurateMs(500);
		//TM_ILI9341_Puts(2, 0, " Espectofotometro ", &TM_Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);

		//TM_ILI9341_Puts(2,30, "Ensayo Longitud de onda Esp.", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);

		//delay(1000);
        /*if (!gpioRead( TEC4 )){
         	gpioToggle(LEDB);
    	    TM_ILI9341_DisplayOff();
            }
        if (!gpioRead( TEC3 )){
                 	gpioToggle(LEDB);
                 	TM_ILI9341_DisplayOn();
                    }
        for (j=0;j<240;j++){
        	for (i=0;i<320;i++){                      //310 fin de la pantalla

        		        TM_ILI9341_DrawPixel(i,j, ILI9341_COLOR_WHITE);
        	        	//TM_ILI9341_Putc(i,j,S,&TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
        	        	delayInaccurateUs(10);
        	        }

        }
        TM_ILI9341_Fill(ILI9341_COLOR_RED);

        TM_ILI9341_DrawRectangle(10,30, 20,40, ILI9341_COLOR_BLACK);*/

}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------


