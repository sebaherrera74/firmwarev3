
/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "timer.h"

/*=====[Definition macros of private constants]==============================*/



// Posibles salidas de toggle pin on match

// ENET_CRS_DV ---> P1_16, FUNC4 = T0_MAT0
// ENET_RXD0   ---> P1_15, FUNC4 = T0_MAT1
// ENET_TXD0   ---> P1_18, FUNC4 = T0_MAT3

// GPIO3       ---> P6_7,  FUNC5 = T2_MAT0
// GPIO4       ---> P6_8,  FUNC5 = T2_MAT1
// GPIO5       ---> P6_9,  FUNC5 = T2_MAT2
// GPIO7       ---> P6_11, FUNC5 = T2_MAT3

// RS232_TXD   ---> P2_3,  FUNC6 = T3_MAT0
// RS232_RXD   ---> P2_4,  FUNC6 = T3_MAT1

#define SIGNAL_PIN_SCU_PORT   6              // GPIO3 (EDU-CIAA-NXP) --> P6
#define SIGNAL_PIN_SCU_PIN    7              // GPIO3 (EDU-CIAA-NXP) --> _7
#define SIGNAL_PIN_FUNC       5              // GPIO3 (EDU-CIAA-NXP) --> FUNC5

#define SIGNAL_TIMER          LPC_TIMER2     // LPC4337 TIMER2
#define SIGNAL_TIMER_CLK      CLK_MX_TIMER2  // LPC4337 TIMER2 CLOCK
#define SIGNAL_TIMER_RESET    RGU_TIMER2_RST // LPC4337 TIMER2 RGU
#define SIGNAL_MATCH_NUMBER   0              // MATCH0

#define SIGNAL_COMPARE_VAUE    300000 //51000      // TimerClk = 204 MHz ==> 204 MHz / 51000 = 4 KHz ==> 250 us
#define SIGNAL_INITIAL_STATE  0              // Signal initial state low

//225000 es el valor para velocidad maxima TimerClk = 204 MHz ==> 204 MHz / 225000 = 906 HZ frecuencia del clk 453 HZ


/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of pubic functions]======================================*/

/*=====[Main function, program entry point after power on or reset]==========*/




//contador de interrupciones,este contador contara tanto las interrupciones producidas por flancos positivos,
//como por flancos negativos del clock externo por GPIO3,

static volatile uint32_t countIrq=0;

void TIMER2_IRQHandler(void)
{
	static bool On = false;

	if (Chip_TIMER_MatchPending(SIGNAL_TIMER, 2)) {

		gpioToggle(LEDB);
		countIrq++;
		Chip_TIMER_ClearMatch(SIGNAL_TIMER, 2);

		//On = (bool) !On;
		//Board_LED_Set(0, On);
	}
}




void signalInit( void )
{

   // Set pin to Timer match output mode (generate signal), se utiliza el pin GPIO03
   Chip_SCU_PinMux( SIGNAL_PIN_SCU_PORT,
                    SIGNAL_PIN_SCU_PIN,
                    SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
                    SIGNAL_PIN_FUNC );

   // Enable timer clock and reset it
   Chip_TIMER_Init( SIGNAL_TIMER );
   Chip_RGU_TriggerReset( SIGNAL_TIMER_RESET );
   while( Chip_RGU_InReset( SIGNAL_TIMER_RESET ) );


   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 0); //=> 204 MHz / 51000 = 4 KHz --4000/2=2000 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 1); //=> 204 MHz / 51000 = 4 KHz --4000/4=1000 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 2); //=> 204 MHz / 51000 = 4 KHz --4000/6=666 Hz

   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 3); //=> 204 MHz / 51000 = 4 KHz --4000/8=500 Hz

   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 4); //=> 204 MHz / 51000 = 4 KHz --4000/10=400 Hz


   // Timer setup for match output toggle
   Chip_TIMER_Reset( SIGNAL_TIMER );

   Chip_TIMER_MatchEnableInt(SIGNAL_TIMER, 2);

   Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,SIGNAL_COMPARE_VAUE );

   Chip_TIMER_ResetOnMatchEnable( SIGNAL_TIMER,
                                  SIGNAL_MATCH_NUMBER );

   Chip_TIMER_ExtMatchControlSet( SIGNAL_TIMER,
                                  SIGNAL_INITIAL_STATE,
                                  TIMER_EXTMATCH_TOGGLE,
                                  SIGNAL_MATCH_NUMBER );
   /* Enable timer interrupt */
   	NVIC_EnableIRQ(TIMER2_IRQn);
   	NVIC_ClearPendingIRQ(TIMER2_IRQn);


}

void signalCheck( void )
{
   // Get timer peripheral clock rate
   uint32_t timerClkFreq = Chip_Clock_GetRate( SIGNAL_TIMER_CLK );

   printf( "timerClkFreq = %d [Hz]\r\n", timerClkFreq );

   float toggleFreq = (float) timerClkFreq / SIGNAL_COMPARE_VAUE;
   float toggleTime = (1.0 / toggleFreq) * 1000.0 * 1000.0;

   printf( "Toggle frequency = %.2f [Hz]\r\n", toggleFreq );
   printf( "Toggle time = %.2f [us]\r\n", toggleTime );
}

void signalStart( void )
{
   // Enable timer to start to generate signal
   Chip_TIMER_Enable( SIGNAL_TIMER );

}

void signalStop( void )
{
   // Disable timer to stop to generate signal
   Chip_TIMER_Disable( SIGNAL_TIMER );
}

uint32_t readPrescaler(void){
	return Chip_TIMER_ReadPrescale(SIGNAL_TIMER);
}

void giroMotorCCW(uint32_t cantPasos){
	while (2*cantPasos>countIrq){       //va ,multiplicado por 2 por que cuenta los dos flancos
		signalStart();
	}
	signalStop();
	countIrq=0;
}
void cambioVelocidad(velocidad_motor veloc){

	switch (veloc){

	case velocidad_baja:
		 //Chip_TIMER_Reset( SIGNAL_TIMER );
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,BAJA);
		break;

	case velocidad_media:
		 //Chip_TIMER_Reset( SIGNAL_TIMER );
			Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,MEDIA);
			break;

	case velocidad_alta:
		 //Chip_TIMER_Reset( SIGNAL_TIMER );
			Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,ALTA);
			break;

	default:
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,BAJA);
		break;
}




}




