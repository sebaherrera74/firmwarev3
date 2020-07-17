#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "frameDriver.h"

#include "frameDriver.h"
#include "FreeRTOSConfig.h"
#include "tlsf.h"
#include "upperLowerApp.h"

void error() {
    while (1)
        ;
}

int main(void)
{
    boardInit();

    static frameDriver_t frameDriver;
    if (!frameDriverInit(&frameDriver, UART_USB))
        error();

    static upperLower_t upperLowerApp;
    upperLowerInit(&upperLowerApp, &frameDriver);


    // Descomentar para probar dos instancias del driver y aplicación
    // static frameDriver_t frameDriver2;
    // if (!frameDriverInit(&frameDriver2, UART_232))
    //     error();

    // static upperLower_t upperLowerApp2;
    // upperLowerInit(&upperLowerApp2, &frameDriver2);

    vTaskStartScheduler();

    error();
}
