#ifndef FRAME_DRIVER
#define FRAME_DRIVER
#include "tlsf.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"


#define MAX_PAYLOAD_LENGTH 40
#define CRC_LENGTH 2
#define PAYLOAD_BUFFER_SIZE MAX_PAYLOAD_LENGTH + CRC_LENGTH // extra space for temporarilly storing received CRC

#define MAX_PAYLOADS_NUM    5
#define MAX_PAYLOADS_POOL_SIZE  MAX_PAYLOADS_NUM * PAYLOAD_BUFFER_SIZE

#define TIMEOUT_MS  50

/** Returns a char buffer of at least MAX_PAYLOAD_LENGTH size */
char *frameCreatePayloadBuffer();

/** Free the char buffer created with frameCreatePayloadBuffer() */
void frameFreePayloadBuffer(tlsf_t handler, char *buf);

/** Called when a frame is received.
 *
 * `payload` is null-terminated.
 * Client must call frameFreePayloadBuffer(payload) afterwards.
 */
typedef void (*rxCallback_t)(char *payload, void *extra);

/** Generic container for a char buffer + content size */
typedef struct {
    char *buf; /** The char buffer, dynamically allocated, with capacity PAYLOAD_BUFFER_SIZE. */
    uint8_t size; /** Actual content size, in bytes. */
} buffer_t;

typedef struct
{
    uartMap_t uartPort;
    xQueueHandle rxQueue;
    xQueueHandle txQueue;
    rxCallback_t rxCallback;
    void *rxCallbackExtra;

    // tlsf memory control block handler
    tlsf_t tlsfHandler;
    // tlsf raw memory block
    pool_t tlsfMemoryBlock;     // No es mandatorio guardarlo, en caso hubiera que hacer un "deinit" del driver sí.

    // state for _uartRxISR
    buffer_t rx;
    TimerHandle_t rxTimer; // counts 50ms before discarding an incomplete frame

    // state for _uartTxISR
    buffer_t tx;
    uint8_t txBytes;
    SemaphoreHandle_t txSemaphore; // controlled by txTimer
    TimerHandle_t txTimer; // counts 50ms between sending 2 frames
} frameDriver_t;

bool frameDriverInit(frameDriver_t *frameDriver, uartMap_t uart);

void frameSetRxCallback(frameDriver_t *frameDriver, rxCallback_t rxCallback, void *extra);

/** Send a frame with the given payolad.
 *
 * The `payload` char buffer must be created with frameCreatePayloadBuffer()
 */
void frameSend(frameDriver_t *fd, char *payload);

#endif
