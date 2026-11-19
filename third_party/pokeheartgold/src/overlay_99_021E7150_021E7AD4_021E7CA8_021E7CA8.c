#include "global.h"

#include "unk_02019BA4.h"

s32 ov99_021E7C58(void *, s32);

void ov99_021E7CA8(void *arg0, s32 arg1);

void ov99_021E7CA8(void *arg0, s32 arg1) {
    GridInputHandler_SetNextInput(*(GridInputHandler **)((u8 *)arg0 + 0x3FC), (u8)arg1);
    ov99_021E7C58(arg0, arg1);
}
