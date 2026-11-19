#include "global.h"

s32 ov07_0223476C(s32 *, s32, s32, s32);

void ov07_0223474C(s32 *arg0, s32 arg1);

void ov07_0223474C(s32 *arg0, s32 arg1) {
    ov07_0223476C(arg0, *arg0, 0, arg1);
}

void ov07_0223475C(void *arg0, s32 arg1);

void ov07_0223475C(void *arg0, s32 arg1) {
    ov07_0223476C(arg0, *(s32 *)((u8 *)arg0 + 4), 1, arg1);
}
