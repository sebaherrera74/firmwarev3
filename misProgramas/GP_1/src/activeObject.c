#include "activeObject.h"

#define MAX_QUEUED_EVENTS   5

bool aoInit(ao_t *ao, aoCallback_t processEvent, void *processEventParam)
{
    if (!(ao->eventsQueue = xQueueCreate(MAX_QUEUED_EVENTS, sizeof(void *))))
        return false;
    ao->task = NULL;
    ao->processEvent = processEvent;
    ao->processEventParam = processEventParam;
}

static void aoEventLoop(void *param) {
    ao_t *ao = param;
    void *event;

    while (xQueueReceive(ao->eventsQueue, &event, 0) == pdTRUE) {
        ao->processEvent(event, ao->processEventParam);
    }
    
    // No hay nada más para procesar, elimnamos OA. [R_C3-7]
    ao->task = NULL;
    vTaskDelete(NULL);
}

void aoDispatch(ao_t *ao, void *event) {
    // Envía evento al objeto activo
    xQueueSend(ao->eventsQueue, &event, portMAX_DELAY);

    // Creamos tarea del objeto activo, si no estuviera creada antes [R_C3-6]
    if (ao->task == NULL) {
        xTaskCreate(
            aoEventLoop,
            "_mayusAoTask",
            configMINIMAL_STACK_SIZE * 2,
            ao,
            tskIDLE_PRIORITY + 1,
            &ao->task
        );
    }
}
