#include "global.h"

#include "unk_02019BA4.h"

void ov83_02247A20(void);

void ov83_02247A20(void) {
}

extern GridCallbacks ov83_02248500;

extern TouchscreenHitbox ov83_02248530;

extern TouchscreenHitbox ov83_02248558;

extern DpadMenuBox ov83_022485A8;

extern DpadMenuBox ov83_022485E8;

GridInputHandler *ov83_02247A24(void *callbackParam, u8 initialPos, s32 arg2);

GridInputHandler *ov83_02247A24(void *callbackParam, u8 initialPos, s32 arg2) {
    if (arg2 == 3) {
        return GridInputHandler_Create(&ov83_02248530, &ov83_022485A8, &ov83_02248500, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_107);
    }
    return GridInputHandler_Create(&ov83_02248558, &ov83_022485E8, &ov83_02248500, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_107);
}

extern GridCallbacks ov83_02248510;

void ov83_02247A7C(void *arg0, u8 arg1, s32 arg2);

void ov83_02247A7C(void *callbackParam, u8 initialPos, s32 arg2) {
    if (arg2 == 3) {
        GridInputHandler_Create(&ov83_02248530, &ov83_022485A8, &ov83_02248510, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_107);
        return;
    }
    GridInputHandler_Create(&ov83_02248558, &ov83_022485E8, &ov83_02248510, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_107);
}
