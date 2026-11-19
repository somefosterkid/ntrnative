#include "global.h"
struct PokeathlonMedals;

#include "unk_02019BA4.h"
#include "overlay_99_pokeathlon_medals.h"

extern GridCallbacks ov99_021E9FA0;

extern TouchscreenHitbox ov99_021EA03C;

extern DpadMenuBox ov99_021EA250;

void ov99_021E7A78(struct PokeathlonMedals *callbackParam);

void ov99_021E7AB8(struct PokeathlonMedals *arg0);

void ov99_021E7A78(PokeathlonMedals *callbackParam) {
    callbackParam->grid = GridInputHandler_Create(&ov99_021EA03C, &ov99_021EA250, &ov99_021E9FA0, callbackParam, 1, (u8)(u8)((u32)(*((s32 *)&callbackParam->flags) << 5) >> 0x18), callbackParam->heapId);
}

void ov99_021E7AB8(PokeathlonMedals *arg0) {
    GridInputHandler *inputHandler;

    inputHandler = arg0->grid;
    if (inputHandler != NULL) {
        GridInputHandler_Free(inputHandler);
        arg0->grid = NULL;
    }
}
