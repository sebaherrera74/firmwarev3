#include "frameDriver.h"

#include "sapi.h"
#include <stdlib.h>
#include <string.h>

#include "crc8.h"
#include "task.h"

#define MAX_QUEUED_MESSAGES 5

static inline void led_error(bool on) {
    gpioWrite(LED2, on);
}

static inline void led_rx(bool on) {
    gpioWrite(LED3, on);
}

static inline void led_tx(bool on) {
    gpioWrite(LED1, on);
}

/**
 * Rx Timer timeout callback
 * [R_C2-6] [R_C2-7]
 */
static void _timeoutRxCallback( TimerHandle_t xTimer)
{
    frameDriver_t * fd = (frameDriver_t *) pvTimerGetTimerID(xTimer);

    taskENTER_CRITICAL();
        if (fd->rx.buf)
        {
            frameFreePayloadBuffer(fd->tlsfHandler,fd->rx.buf);
            fd->rx.buf = NULL;
            led_rx(false);
        }
    taskEXIT_CRITICAL();
}

/**
 * ISR on new UART char
 * [R_C2-1]
 */
static void _uartRxISR(void *param)
{
    frameDriver_t *fd = param;
    BaseType_t taskWoken = pdFALSE;

    char c = uartRxRead(fd->uartPort);
    if (!fd->rx.buf && c == '(') {
        // start new frame
        // [R_C2-2]
        fd->rx.size = 0;
        fd->rx.buf = frameCreatePayloadBuffer(fd->tlsfHandler);

        // memory could not be allocated, the frame will be ignored until next '('
        if (fd->rx.buf) {
            // start timer
            xTimerStartFromISR( fd->rxTimer, &taskWoken );
            led_rx(true);
        }
    } else if (!fd->rx.buf) {
        // no current frame -- do nothing until '('
    } else if (c == ')') {
        // done receiving frame - send it to _frameTask
    	xTimerStopFromISR( fd->rxTimer, &taskWoken );
        if (xQueueSendFromISR(fd->rxQueue, &fd->rx, &taskWoken) != pdTRUE) {
            // could not enqueue
            frameFreePayloadBuffer(fd->tlsfHandler, fd->rx.buf);
            led_error(true);
        }
        // Se libera buffer actual dentro de la ISR. Ownership pasó a _frameTask
        // La ISR puede seguir recibiendo datos [R_C2-4]
        fd->rx.buf = NULL;
        led_rx(false);
    } else if (fd->rx.size == PAYLOAD_BUFFER_SIZE || c == '(') {                // [R_C2-3]
        // received payload is too large OR unexpected '(' -- discard frame
        frameFreePayloadBuffer(fd->tlsfHandler, fd->rx.buf);
        fd->rx.buf = NULL;
        led_rx(false);
    } else {
        // payload or CRC character received
        fd->rx.buf[fd->rx.size++] = c;
        xTimerStartFromISR( fd->rxTimer, &taskWoken );      // [R_C2-6]
    }

    portYIELD_FROM_ISR(taskWoken)
}

/**
 *  Tx Timer timeout callback
 *  [R_C2-11]
 */
static void _timeoutTxCallback( TimerHandle_t xTimer)
{
    frameDriver_t * fd = (frameDriver_t *) pvTimerGetTimerID(xTimer);

    taskENTER_CRITICAL();
        xSemaphoreGive(fd->txSemaphore);
    taskEXIT_CRITICAL();
}

/**
 * ISR on UART tx empty
 */
static void _uartTxISR(void *param)
{
    frameDriver_t *fd = param;
    BaseType_t taskWoken = pdFALSE;

    if (!fd->tx.buf) {
        if (xQueueReceiveFromISR(fd->txQueue, &fd->tx, &taskWoken) != pdTRUE) {
            // nothing to do
            uartCallbackClr(fd->uartPort, UART_TRANSMITER_FREE);
            portYIELD_FROM_ISR(taskWoken)
            return;
        }
        fd->txBytes = 0;
    }
    if (!uartTxReady(fd->uartPort)) {
        portYIELD_FROM_ISR(taskWoken)
        return;
    }

    led_tx(true);

    // fd->tx.buf contains [payload + CRC]
    //   eg: if message is 'hello' and CRC is '3B'
    //   fd->tx.buf = 'hello3B'
    //   fd->tx.size = 7
    char c;
    if (fd->txBytes == 0) {
        c = '(';
    } else if (fd->txBytes < fd->tx.size + 1) {
        c = fd->tx.buf[fd->txBytes - 1];
    } else {
        c = ')';
    }
    uartTxWrite(fd->uartPort, c);
    fd->txBytes++;
    if (fd->txBytes == fd->tx.size + 2 /* '(' and ')' */) {
        // done sending frame
        frameFreePayloadBuffer(fd->tlsfHandler,fd->tx.buf);
        fd->tx.buf = NULL;

        // Wait 50ms before sending next frame
        xTimerStartFromISR(fd->txTimer, &taskWoken);

        led_tx(false);
    }

    portYIELD_FROM_ISR(taskWoken)
}

static inline bool isInterrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

static inline BaseType_t enterCriticalSection() {
    if (isInterrupt()) {
        return taskENTER_CRITICAL_FROM_ISR();
    } else {
        taskENTER_CRITICAL();
        return 0;
    }
}

static inline void exitCriticalSection(BaseType_t savedInterruptStatus) {
    if (isInterrupt()) {
        taskEXIT_CRITICAL_FROM_ISR(savedInterruptStatus);
    } else {
        taskEXIT_CRITICAL();
    }
}

char *frameCreatePayloadBuffer(tlsf_t handler) {
    BaseType_t savedInterruptStatus = enterCriticalSection();

    void *r = tlsf_malloc(handler, PAYLOAD_BUFFER_SIZE);

    // Memory check
    if (r && tlsf_block_size(r) < PAYLOAD_BUFFER_SIZE) {
        tlsf_free(handler, r);
        r = NULL;
    }

    if (!r) {
        led_error(true);
    }

    exitCriticalSection(savedInterruptStatus);
    return r;
}

void frameFreePayloadBuffer(tlsf_t handler, char *buf) {
    BaseType_t savedInterruptStatus = enterCriticalSection();

    tlsf_free(handler, buf);

    exitCriticalSection(savedInterruptStatus);
}

void frameSetRxCallback(frameDriver_t *fd, rxCallback_t rxCallback, void *extra)
{
    fd->rxCallback = rxCallback;
    fd->rxCallbackExtra = extra;
}

static bool _extractCRC(const buffer_t *payloadWithCRC, uint8_t *crc) {
    // extract the last CRC_LENGTH bytes into s
    char s[CRC_LENGTH + 1];
    strncpy(s, payloadWithCRC->buf + payloadWithCRC->size - CRC_LENGTH, CRC_LENGTH);
    s[CRC_LENGTH] = '\0';

    // parse hex number
    char *endptr;
    long int n = strtol(s, &endptr, 16);
    if (endptr - s != CRC_LENGTH) {
        return false;
    }
    *crc = n;
    return true;
}


/**
 * Agrega CRC a un payload
 * [R_C2-10]
 */
void _appendCRC(buffer_t *payloadWithoutCRC, uint8_t crc)
{
    char crcS[CRC_LENGTH + 1];
    sprintf(crcS,"%02X",crc);

    strncpy(payloadWithoutCRC->buf + payloadWithoutCRC->size, crcS, CRC_LENGTH);

    payloadWithoutCRC->size = payloadWithoutCRC->size + CRC_LENGTH;

}

void frameSend(frameDriver_t *fd, char *payload)
{
    buffer_t tx;

    tx.buf = payload;
    tx.size = strlen(payload);

    // Calculo crc y lo agrego
    _appendCRC(&tx, crc8(tx.buf));

    if (xQueueSend(fd->txQueue, &tx, portMAX_DELAY) != pdTRUE) {    // TODO: Revisar portMAX_DELAY
        // could not enqueue
        frameFreePayloadBuffer(fd->tlsfHandler,fd->tx.buf);

        led_error(true);
    }
    uartCallbackSet(fd->uartPort, UART_TRANSMITER_FREE, _uartTxISR, fd);

    // Bloqueo esperando el semáforo antes de transmitir (espera de 50ms)
    // [R_C2-11]
    if (xSemaphoreTake(fd->txSemaphore, pdMS_TO_TICKS(TIMEOUT_MS) * 2) == pdTRUE) // TODO: Revisar timeout del semáforo
    {
        uartSetPendingInterrupt(fd->uartPort);
    }
    else
    {   // could not send within 2 timeouts
        frameFreePayloadBuffer(fd->tlsfHandler,fd->tx.buf);

        led_error(true);
    }
}

void _frameTask(void *param) {
    frameDriver_t *fd = param;
    while (true) {
        buffer_t rx;

        // block until a full frame is received in _uartRxISR
        xQueueReceive(fd->rxQueue, &rx, portMAX_DELAY);

        // rx.buf contains [payload + CRC]
        //   eg: if message is 'hello' and CRC is '3B'
        //   rx.buf = 'hello3B'
        //   rx.size = 7

        // extract CRC from frame
        uint8_t receivedCRC;
        if (!_extractCRC(&rx, &receivedCRC)) {      
            // invalid CRC [R_C2-8]
            frameFreePayloadBuffer(fd->tlsfHandler,rx.buf);
            continue;
        }

        // null-terminate the payload (this overwrites the CRC section we just extracted)
        rx.buf[rx.size - CRC_LENGTH] = '\0';

        // validate CRC
        uint8_t actualCRC = crc8(rx.buf);
        if (receivedCRC != actualCRC) {
            // CRC mismatch [R_C2-8]
            frameFreePayloadBuffer(fd->tlsfHandler,rx.buf);
            continue;
        }

        // all good! Call apllication [R_C2-9]
        fd->rxCallback(rx.buf, fd->rxCallbackExtra);
    }
}

bool frameDriverInit(frameDriver_t *fd, uartMap_t uart)
{
    *fd = (frameDriver_t){0};

    // Memoria para tlsf (Estructura de control + Máximo de payloads admitido)
    fd->tlsfMemoryBlock = (pool_t) pvPortMalloc(MAX_PAYLOADS_POOL_SIZE + tlsf_size());
    fd->tlsfHandler = tlsf_create_with_pool(fd->tlsfMemoryBlock, MAX_PAYLOADS_POOL_SIZE + tlsf_size());

    // Queues
    if (!(fd->rxQueue = xQueueCreate(MAX_QUEUED_MESSAGES, sizeof(buffer_t)))) {
        led_error(true);
        return false;
    }

    if (!(fd->txQueue = xQueueCreate(MAX_QUEUED_MESSAGES, sizeof(buffer_t)))) {
        led_error(true);
        return false;
    }

    // Timers
    if (!(fd->rxTimer = xTimerCreate( "RX Time Out", pdMS_TO_TICKS( TIMEOUT_MS ), pdFALSE, ( void* ) fd, _timeoutRxCallback ))) {
        led_error(true);
        return false;
    }

    if (!(fd->txTimer = xTimerCreate( "TX Time Out", pdMS_TO_TICKS( TIMEOUT_MS ), pdFALSE, ( void* ) fd, _timeoutTxCallback ))) {
        led_error(true);
        return false;
    }

    // Semaphores
    if (!(fd->txSemaphore = xSemaphoreCreateBinary())) {
        led_error(true);
        return false;
    }
    xSemaphoreGive(fd->txSemaphore);

    // Uart
    fd->uartPort = uart;
    uartConfig(uart, 115200);
    uartCallbackSet(uart, UART_RECEIVE, _uartRxISR, fd);
    uartCallbackSet(uart, UART_TRANSMITER_FREE, _uartTxISR, fd);
    uartInterrupt(uart, true);

    // Task
    if (xTaskCreate(
        _frameTask,
        "_frameTask",
        configMINIMAL_STACK_SIZE * 2,
        (void*) fd,
        tskIDLE_PRIORITY + 1,
        0
    ) != pdPASS) {
        led_error(true);
        return false;
    }

    return true;
}
