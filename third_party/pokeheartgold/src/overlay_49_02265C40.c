#include "global.h"
#include "overlay_49.h"

void *sub_020181A0(void *);

void ov49_02265C40(void *arg0, u8 *arg1, s32 arg2, s32 arg3);

s32 ov49_02267A84(void *, void *, s32);
s32 ov49_02267C20(void *, void *, s32);

void ov49_02265C40(void *arg0, u8 *arg1, s32 arg2, s32 arg3) {
    void *new_var;
    u8 *new_var2;
    new_var = arg0;
    new_var2 = (u8 *)(arg1 + (arg2 * 4));
    sub_020181A0((new_var + 0x106DC) + (0x14 * (*((u8 *)(((u8 *)((*((u8 **)(new_var2 + 0x87C))) + arg3)) + 2)))));
}

void ov49_02265C68(void *arg0, void *arg1) {
    ov49_02267A84(arg0, arg1, 1);
}

void ov49_02265C74(void *arg0, void *arg1) {
    ov49_02267A84(arg0, arg1, 2);
}

void ov49_02265C80(void *arg0, void *arg1) {
    ov49_02267A84(arg0, arg1, 3);
}

void ov49_02265C8C(void *arg0, void *arg1) {
    ov49_02267C20(arg0, arg1, 1);
}

void ov49_02265C98(void *arg0, void *arg1) {
    ov49_02267C20(arg0, arg1, 2);
}

void ov49_02265CA4(void *arg0, void *arg1) {
    ov49_02267C20(arg0, arg1, 3);
}
