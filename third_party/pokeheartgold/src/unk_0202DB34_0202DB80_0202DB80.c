#include "global.h"

s32 sub_0202DB80(void *arg0);

s32 sub_0202DB80(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0xF0);
}

void sub_0202DB88(void *arg0, s32 arg1);

void sub_0202DB88(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0xF0) = arg1;
}

s32 sub_0202DB90(void *arg0);

s32 sub_0202DB90(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0xF4);
}

void sub_0202DB98(void *arg0, s32 arg1);

void sub_0202DB98(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0xF4) = arg1;
}

u16 sub_0202DBA0(void *arg0);

u16 sub_0202DBA0(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0xEE);
}
