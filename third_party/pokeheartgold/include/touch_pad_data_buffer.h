#ifndef POKEHEARTGOLD_TOUCH_PAD_DATA_BUFFER_H
#define POKEHEARTGOLD_TOUCH_PAD_DATA_BUFFER_H

#include "global.h"

typedef struct TouchPadDataBuffer {
    u16 bufferSize;
    TPData buffer[8];
} TouchPadDataBuffer;

#endif // POKEHEARTGOLD_TOUCH_PAD_DATA_BUFFER_H
