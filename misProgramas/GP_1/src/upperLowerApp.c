#include "upperLowerApp.h"

#include <string.h>
#include <ctype.h>

static bool _validChar(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool _validPayload(char *payload) {
    for (int i = 0; payload[i]; i++) {
        if (!_validChar(payload[i]))
            return false;
    }
    return true;
}

static void _sendError(frameDriver_t *fd, char *payload) {
    strcpy(payload, "ERROR");
    frameSend(fd, payload);
}

/** 
 * Aplicación principal. Callback que se ejecuta ante un nuevo frame.
 * Verifica el comando 'C' del protocolo, despacha el procesamiento
 * al objeto activo según el comando o devuelve error.
 */
static void _onFrameReceived(char *payload, void *extra)
{
    upperLower_t *app = extra;

    if (!_validPayload(payload)) {              // [R_C3-2]
        _sendError(app->frameDriver, payload);  // [R_C3-3]
        return;
    }

    // Analizo primer caracter [R_C3-1]
    switch (payload[0])
    {
    case 'M':
        aoDispatch(&app->upperAo, payload);
        break;

    case 'm':
        aoDispatch(&app->lowerAo, payload);
        break;

    default:
        _sendError(app->frameDriver, payload);
        break;
    }
}

static void _transformAndSend(int (*f)(int), char *payload, upperLower_t *app) {
    int i = 1; // discard m / M
    do {
        // convert to lower / uppercase
        payload[i - 1] = f(payload[i]);
        i++;
    } while (payload[i - 1]);
    frameSend(app->frameDriver, payload);
}

static void _toUpper(void *event, void *param) {
    _transformAndSend(toupper, event, param);       // [R_C3-4] toupper para pasar a mayúscula
}

static void _toLower(void *event, void *param) {
    _transformAndSend(tolower, event, param);       // [R_C3-4] toupper para pasar a minúscula
}

void upperLowerInit(upperLower_t *app, frameDriver_t *frameDriver)
{
    app->frameDriver = frameDriver;
    aoInit(&app->upperAo, _toUpper, app);
    aoInit(&app->lowerAo, _toLower, app);
    frameSetRxCallback(frameDriver, _onFrameReceived, app);
}
