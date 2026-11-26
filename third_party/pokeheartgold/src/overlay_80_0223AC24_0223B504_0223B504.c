#include "global.h"

#include "overlay_80_0223AC24.h"

s32 sub_02014A60(void *);

void sub_02014AA0(void);

s32 sub_02014AB0(s32, void *, s32, s32);

void ov80_0223B504(void *arg0);

void ov80_0223B504(void *arg0) {
    s32 v0;

    v0 = sub_02014A60(*(void **)((u8 *)arg0 + 0x1C));
    sub_02014AA0();
    sub_02014AB0(v0, *(void **)((u8 *)arg0 + 0x3020), 0x20, 1);
}

s32 sub_02014A8C(void *);

void ov80_0223B528(void *arg0) {
    sub_02014A8C(*(void **)((u8 *)arg0 + 0x1C));
    ov80_0223B504(arg0);
}
