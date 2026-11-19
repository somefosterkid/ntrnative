#include "global.h"

#include "unk_02019BA4.h"

extern s32 ov83_02248544;

s32 ov83_02247AD4(GridInputHandler *arg0);

void ov83_02247B04(GridInputHandler *arg0);

s32 ov83_02242AB4(void *, s32, s32);
void ov83_02247B1C(void *arg0, s32 arg1, s32 arg2);
void ov83_02247B34(void *arg0, s32 arg1, s32 arg2);

s32 ov83_02247AD4(GridInputHandler *inputHandler) {
    int new_var;
    u32 v0;
    new_var = GridInputHandler_HandleInput_NoHold(inputHandler);
    v0 = new_var + 4;
    switch (v0) {
    case 0:

    case 1:

    case 2:
    case 3:
        return new_var;
        new_var = new_var * (new_var = 4);
    default:
        return *((s32 *)((&ov83_02248544) + new_var));
    }
}

void ov83_02247B04(GridInputHandler *inputHandler) {
    s32 v0;

    v0 = GridInputHandler_GetNextInput(inputHandler);
    GridInputHandler_SetNextLastUnk0FInputs(inputHandler, 0, v0, v0);
}

void ov83_02247B1C(void *arg0, s32 arg1, s32 arg2) {
    ov83_02242AB4(arg0, *(s32 *)(&ov83_02248544 + arg1), *(s32 *)(&ov83_02248544 + arg2));
}

void ov83_02247B34(void *arg0, s32 arg1, s32 arg2) {
    ov83_02242AB4(arg0, *(s32 *)(&ov83_02248544 + arg1), *(s32 *)(&ov83_02248544 + arg2));
}
