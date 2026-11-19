#include "global.h"
#include "overlay_14_box_funcs.h"

void ov14_021F66E8(void *arg0, s32 arg1, s32 arg2);

s32 ov14_021F6698(void *, s32, s32);

void ov14_021F66E8(void *arg0, s32 arg1, s32 arg2) {
    if (arg2 == 1) {
        ov14_021F6698(arg0, 0xB, arg1);
        return;
    }
    ov14_021F6698(arg0, 0xC, arg1);
}
