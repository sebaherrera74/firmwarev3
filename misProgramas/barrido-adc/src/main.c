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


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "sapi.h"
#include "ADS1015.h"


DEBUG_PRINT_ENABLE;

SemaphoreHandle_t sem_inicio;
SemaphoreHandle_t sem_final;  //No me hace falta el semaforo del final




/*=====[Macros de definición de constantes privadas]=========================*/

uint8_t secuenciaPasos[4][4]={
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
};


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/
void rotarBobinasCCW(uint8_t velocidad,uint16_t cantPasos);
void inicializar_bobinas(void);
void rotarBobinasCW(uint8_t velocidad,uint16_t cantPasos);
void detenerMotor(void);
/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/


int main (void)
{

	//int Error_creacion_Colas=0;
	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();
	i2cInit( I2C0, 100000 );
	inicializar_bobinas();

	uartConfig( UART_USB, 115200 );
	uartWriteString( UART_USB,"Driver de Espectrofotometro \r\n" );
	uartWriteString( UART_USB,"Programa que prueba barrido completo + adquisicion de datos \r\n" );
	// Led para dar señal de vida
	gpioWrite( LED3, ON );

	// configura teclas para debounce
	/*teclas_config();
		inicializacion_gpio();
		delayInaccurateMs(100);
		posicion_cero();          //funcion bloqueante, posiciona el motor al arranque en la longitud de onda inicial

		/* funcion que crea semaforos y colas a utilizar */
	//Error_creacion_Colas=sem_queues_init();




	void tarea_barrido( void* taskParmPtr);
	void tarea_analogico_digital( void* taskParmPtr );

	//Creacion de semaforos que me va  ha disparar el inicio del ensayo y el final del ensayo




	//CREACION DE TAREAS EN  freeRTOS
	//--Tarea de barrido d elongitud de onda ----//

	BaseType_t res1 =xTaskCreate(tarea_barrido,                     // Funcion de la tarea a ejecutar
			(const char *)"Barrido longitud de onda",   // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,      // Cantidad de stack de la tarea
			0,                    // Parametros de tarea
			tskIDLE_PRIORITY+1,              // Prioridad de la tarea
			0);                              // Puntero a la tarea creada en el sistema


	//----------Tarea de lectura de valor analogico del conversor
	BaseType_t res2 =xTaskCreate(tarea_analogico_digital,   //Funcion de la tarea a ejecutar
			(const char *)"lectura de valor analogico-digital ",         // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,        // Cantidad de stack de la tarea
			0,                                // Parametros de tarea
			tskIDLE_PRIORITY+1,               // Prioridad de la tarea
			0);                              // Puntero a la tarea creada en el sistema

	/*Chequeo de errores e la creacion de las tareas */

	if(res1 == pdFAIL || res2 == pdFAIL){
		gpioWrite( LED1, ON );
		printf( "Error al crear las tareas." );
		while(TRUE)                              //Entro en un lazo cerrado
		{
		}
	}


	sem_inicio=xSemaphoreCreateBinary();
		sem_final=xSemaphoreCreateBinary();
	//Inicia Scheduler y chequeo para activar el scheduler que las colas y semaforos se hayan
	//creado correctamente

	vTaskStartScheduler();


	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE ) {

		// Si cae en este while 1 significa que no pudo iniciar el scheduler
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------


void tarea_barrido( void* taskParmPtr ){
	taskENTER_CRITICAL();
	printf("\r\n %s \r\n",pcTaskGetTaskName(NULL));
	fflush( stdout );
	taskEXIT_CRITICAL();

	//tTecla* config = (tTecla*) taskParmPtr;



	// Tarea periodica cada 10 ms
	portTickType xPeriodicity =  40/ portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {

		if(!gpioRead(TEC3)){
			rotarBobinasCCW(60,1000); //Con 60 mseg demora 1 minuto exacto en hacer un barrido completo de acuerdo a requerimientos
			vTaskDelay(1000);
			rotarBobinasCW(10,1000);
			detenerMotor();
			//xSemaphoreGive(sem_final);

		}
		vTaskDelay(10);
	}

	vTaskDelete(NULL);
}

void tarea_analogico_digital( void* taskParmPtr ){
	taskENTER_CRITICAL();
	printf("\r\n %s \r\n",pcTaskGetTaskName(NULL));
	fflush( stdout );
	taskEXIT_CRITICAL();
	uint16_t adc0, adc1, adc2, adc3=0;
	float voltage_0=0;
	static char result[10],resultcontmuestras[10];
	uint64_t contador_muestras=0;



	// Tarea periodica cada 10 ms
	portTickType xPeriodicity =  40/ portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {

		if(xSemaphoreTake(sem_inicio,portMAX_DELAY)){
			gpioToggle(LED1);
			contador_muestras++;
			setGain(GAIN_TWOTHIRDS);    // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)

			adc0 = readADC_SingleEnded(0);
			//tomo lectura de un solo canal por ahora
			//adc1 = readADC_SingleEnded(1);
			//adc2 = readADC_SingleEnded(2);
			//adc3 = readADC_SingleEnded(3);
			//printf("AIN0: %i \r\n ",adc0);

			voltage_0=(adc0*0.003);   //multiplico valor leido por la resolucion seleccionada arriba


			floatToString( voltage_0, result, 8 );  //convierto valor flotante en string para visualizar

			uartWriteString( UART_USB, result );
			uartWriteByte( UART_USB, '\r' ); // Envia '\r', retorno de carro
			uartWriteByte( UART_USB, '\n' ); // Envia '\n', nueva linea
			//Imprimo la cantidad de muestras que voy midiendo
			uint64ToString( contador_muestras, resultcontmuestras,10 );
			uartWriteString( UART_USB, resultcontmuestras );
			uartWriteByte( UART_USB, '\r' ); // Envia '\r', retorno de carro
			uartWriteByte( UART_USB, '\n' ); // Envia '\n', nueva linea

			//cuando llegue a las muestras que tengo que tomar, lo reseteo a cero
			if (contador_muestras==1000){
		    	contador_muestras =0;
		    }

		}



	}
}



void inicializar_bobinas(void){
	gpioConfig( GPIO1, GPIO_OUTPUT );
	gpioConfig( GPIO2, GPIO_OUTPUT );
	gpioConfig( GPIO3, GPIO_OUTPUT );
	gpioConfig( GPIO4, GPIO_OUTPUT );

}

void rotarBobinasCCW(uint8_t velocidad,uint16_t cantPasos){
	uint8_t aux=0;
	int i;

	for (i=cantPasos;i>0;i--){
		aux=(i+4)%4;

		gpioWrite(GPIO1,secuenciaPasos[aux][0]);
		gpioWrite(GPIO2,secuenciaPasos[aux][1]);
		gpioWrite(GPIO3,secuenciaPasos[aux][2]);
		gpioWrite(GPIO4,secuenciaPasos[aux][3]);
		vTaskDelay(velocidad);
		xSemaphoreGive(sem_inicio);
	}
}

void rotarBobinasCW(uint8_t velocidad,uint16_t cantPasos){

	uint8_t aux=0;
	uint16_t i;
	for (i=0;i<cantPasos;i++){

		aux=(i+4)%4;
		gpioWrite(GPIO1,secuenciaPasos[aux][0]);
		gpioWrite(GPIO2,secuenciaPasos[aux][1]);
		gpioWrite(GPIO3,secuenciaPasos[aux][2]);
		gpioWrite(GPIO4,secuenciaPasos[aux][3]);
		vTaskDelay(velocidad);

	}
}

void detenerMotor(void){
	gpioWrite(GPIO1,0);
	gpioWrite(GPIO2,0);
	gpioWrite(GPIO3,0);
	gpioWrite(GPIO4,0);

}





