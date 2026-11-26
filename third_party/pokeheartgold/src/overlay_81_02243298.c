#include "global.h"

#include "overlay_81.h"
#include "unk_02019BA4.h"

s32 ov81_022413F4();

s32 ov81_0224141C();

extern GridCallbacks ov81_02243648;
extern TouchscreenHitbox ov81_02243668;
extern DpadMenuBox ov81_02243750;
GridInputHandler *ov81_022432AC(void *arg0, u8 arg1);
extern TouchscreenHitbox ov81_02243688;
extern DpadMenuBox ov81_02243718;
GridInputHandler *ov81_022432DC(void *arg0, u8 arg1);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov81_02242704(u16);
#else
s32 ov81_02242704(s32);
#endif
void ov81_02243314(GridInputHandler *arg0, s32 arg1, s32 arg2);

void ov81_02243298(s32 arg0, s32 arg1) {
    if (arg1 >= 6) {
        ov81_0224141C();
        return;
    }
    ov81_022413F4();
}

GridInputHandler *ov81_022432AC(void *callbackParam, u8 initialPos) {
    return GridInputHandler_Create(&ov81_02243668, &ov81_02243750, &ov81_02243648, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_100);
}

GridInputHandler *ov81_022432DC(void *callbackParam, u8 initialPos) {
    GridInputHandler *inputHandler;

    inputHandler = GridInputHandler_Create(&ov81_02243688, &ov81_02243718, &ov81_02243648, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_100);
    GridInputHandler_ClearEnabledFlag(inputHandler, 2U);
    return inputHandler;
}

void ov81_02243314(GridInputHandler *inputHandler, s32 arg1, s32 arg2) {
    if (arg1 == 1) {
        GridInputHandler_SetAllEnabled(inputHandler);
        GridInputHandler_ClearEnabledFlag(inputHandler, 3U);
    } else {
        GridInputHandler_SetAllEnabled(inputHandler);
        GridInputHandler_ClearEnabledFlag(inputHandler, 4U);
        GridInputHandler_ClearEnabledFlag(inputHandler, 5U);
        GridInputHandler_ClearEnabledFlag(inputHandler, 6U);
    }
    if (ov81_02242704(arg2) == 1) {
        GridInputHandler_ClearEnabledFlag(inputHandler, 2U);
    }
}
