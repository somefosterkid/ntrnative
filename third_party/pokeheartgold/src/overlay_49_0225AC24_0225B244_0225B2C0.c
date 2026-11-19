#include "global.h"

s32 ov45_0222E7FC(s32);

s32 ov49_0225B24C(void *, void *);

void *ov49_0225B388(void *, s32, s32);

s32 ov49_0225B3A8(void *, s32, s32, s32, s32);

void ov49_0225B2C0(void *arg0, void *arg1, s32 arg2);

void ov49_0225B2C0(void *arg0, void *arg1, s32 arg2) {
    ov49_0225B3A8(arg1, ov45_0222E7FC(arg2), 5, 0, 2);
    ov49_0225B24C(arg0, ov49_0225B388(arg1, 2, 0x20));
}

void ov49_0225B2F0(void *arg0, void *arg1);

void ov49_0225B2F0(void *arg0, void *arg1) {
    ov49_0225B24C(arg0, ov49_0225B388(arg1, 2, 0xE));
}
