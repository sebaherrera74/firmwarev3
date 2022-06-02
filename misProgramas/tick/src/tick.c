

#include "sapi.h"


int count=0;

void onTimer(void *unused)
{
	gpioToggle(LED1);
	gpioToggle(LED2);
	gpioToggle(GPIO1);
	count++;

}


int main (void)
{
	boardInit();
	gpioInit(LED1,GPIO_OUTPUT);
	gpioInit(GPIO1,GPIO_OUTPUT);
	uartConfig( UART_USB, 115200 );
	tickInit(50);
	tickCallbackSet(onTimer,0);
	while (TRUE)
	{
		//__WFI();
		delay(1000);
		printf("\r\n",count);
	}
	return 0;





}
