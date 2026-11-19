#include "global.h"

#include "system.h"

void ov69_021E645C(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5);

void ov69_021E645C(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;

    v0 = 0;
    v1 = 0;
    v2 = 0;
    v3 = 0;
    if (gSystem.touchX != 0xFFFF) {
        v2 = gSystem.touchX - arg0;
        if (v2 < 0) {
            v2 ^= -1;
            v0 = 0x10;
        } else if (v2 > 0) {
            v0 = 0x20;
        }
    }
    *arg2 = v0;
    *arg3 = v2 & 0x3F;
    if (gSystem.touchY != 0xFFFF) {
        v3 = gSystem.touchY - arg1;
        if (v3 < 0) {
            v3 ^= -1;
            v1 = 0x80;
        } else if (v3 > 0) {
            v1 = 0x40;
        }
    }
    *arg4 = v1;
    *arg5 = v3 & 0x3F;
}
