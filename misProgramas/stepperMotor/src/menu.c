#include "menu.h"
void menu(void) {
	uartWriteByte(UART_USB, 27);
	uartWriteString(UART_USB, "[2J");
	uartWriteByte(UART_USB, 27);
	uartWriteString(UART_USB, "[H");
	printf("*****Menú Principal *****\r\n");
	printf("*****Para Controlar el Motor Paso a Paso****\r\n");
	printf(" 1. Habilitar el Motor Paso a Paso.\r\n");
	printf(" 2. Deshabilitar el Motor Paso a Paso.\r\n");
	printf(" 3. Establecer Sentido de Giro Horario.\r\n");
	printf(" 4. Establecer Sentido de Giro Antihorario.\r\n");
	printf(" 5. Establecer los Micropasos.\r\n");
	printf(" 6. Establecer Cantidad de Pasos.\r\n");
	printf(" 7. Establecer Ángulo de Giro.\r\n");
	printf(" 8. Establecer Cantidad de Vueltas.\r\n");
}
