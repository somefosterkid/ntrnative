#include "global.h"

s32 ov45_0222BD4C(void *);

s32 ov45_0222EC68(s32 arg0);

void ov45_0222C800(s32 arg0, s32 arg1, s32 arg2, void *arg3);

void ov45_0222C800(s32 arg0, s32 arg1, s32 arg2, void *arg3) {
    if ((((*((u8 *)(((u8 *)arg3) + 0x1C6))) != 0) && ((*((u16 *)(((u8 *)arg3) + 0x1C4))) == ov45_0222EC68(arg0))) != 0) {
        ov45_0222BD4C(arg3 + 0x1C0);
    }
}
