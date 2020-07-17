#ifndef UPPER_LOWER_APP
#define UPPER_LOWER_APP

#include "activeObject.h"
#include "frameDriver.h"

typedef struct
{
    frameDriver_t *frameDriver;
    // Un objeto activo para cada operación
    // [R_C3-5]
    ao_t upperAo;   
    ao_t lowerAo;
} upperLower_t;

void upperLowerInit(upperLower_t *app, frameDriver_t *frameDriver);

#endif
