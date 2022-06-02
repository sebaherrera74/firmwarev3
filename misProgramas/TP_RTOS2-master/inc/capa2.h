

#ifndef TP_RTOS2_INC_APP_H_
#define TP_RTOS2_INC_APP_H_
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "sapi.h"
#include "timers.h"
#include "semphr.h"

#include "qmpool.h"
#include "crc8.h"
#include "driver.h"


/*=====[Prototypes (declarations) of public functions]=======================*/

void appC2Rx(driver_t* Uart_driver);
void appC2Tx(driver_t* Uart_driver);


#endif /* TP_RTOS2_INC_APP_H_ */
