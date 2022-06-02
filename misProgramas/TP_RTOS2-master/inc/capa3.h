

#ifndef TP_RTOS2_INC_CAPA3_H_
#define TP_RTOS2_INC_CAPA3_H_

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

void appC3(driver_t* Uart_driver);



#endif /* TP_RTOS2_INC_CAPA3_H_ */
