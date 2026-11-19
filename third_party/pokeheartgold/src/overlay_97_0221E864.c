#include "global.h"
#include "unk_0200B150.h"

void ov97_0221E864(void);

void ov97_0221E864(void) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 1, 0x7E, 0, 0x20, HEAP_ID_POKEATHLON);
}
