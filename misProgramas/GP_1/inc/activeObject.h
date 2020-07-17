#ifndef MAYUS_AO
#define MAYUS_AO

#include "FreeRTOS.h"
#include "sapi.h"
#include "queue.h"
#include "task.h"

typedef void (*aoCallback_t)(void *event, void *param);

typedef struct {
    xQueueHandle eventsQueue;
    TaskHandle_t task;
    aoCallback_t processEvent;
    void *processEventParam;
} ao_t;

bool aoInit(ao_t *ao, aoCallback_t processEvent, void *processEventParam);

void aoDispatch(ao_t *ao, void *event);

#endif
