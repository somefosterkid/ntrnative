#include "global.h"

s32 sub_020182B0(u8 *, void *, void *, void *);

void ov49_0225D030(u8 *arg0, u8 *arg1);

void ov49_0225D030(u8 *arg0, u8 *arg1) {
    sub_020182B0(arg0 + 4, arg1, arg1 + 4, arg1 + 8);
}

s32 sub_020182A0(void *);

void ov49_0225D040(u8 *arg0);

void ov49_0225D040(u8 *arg0) {
    sub_020182A0(arg0 + 4);
}

s32 ov49_0225D04C(void *arg0);

s32 ov49_0225D04C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x80) == 0) {
        *(s32 *)((u8 *)arg0 + 0x80) = 1;
        return 1;
    }
    return 0;
}

s32 ov49_0225D064(void *arg0);

s32 ov49_0225D064(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x88) == 0) {
        *(s32 *)((u8 *)arg0 + 0x88) = 1;
        return 1;
    }
    return 0;
}

s32 sub_020182E0(u8 *, s32, s32);

void ov49_0225D07C(u8 *arg0, s32 arg1);

void ov49_0225D07C(u8 *arg0, s32 arg1) {
    sub_020182E0(arg0 + 4, arg1, 0);
}

s32 ov49_0225D088(void *arg0);

s32 ov49_0225D088(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x80);
}

s32 ov49_0225D090(void *arg0);

s32 ov49_0225D090(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x88);
}
