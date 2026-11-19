#include "global.h"
#include "unk_struct_ov70_0223f8a8.h"

extern u32 ov70_02245B5C;

void ov70_0223F8A8(UnkStruct_ov70_0223F8A8 *param_1, s32 param_2);

void ov70_0223F8A8(UnkStruct_ov70_0223F8A8 *param_1, s32 param_2) {
    if (param_2 == 0) {
        param_1->unk_12CC = 0;
        return;
    }
    if (param_2 - 1U < 0x82) {
        param_1->unk_12CC = (u32) * (u16 *)(((u8 *)&ov70_02245B5C) + (param_2 - 1U) * 2);
    }
    return;
}

extern s32 ov70_022459C8;

s32 ov70_0223F8D0(s32 arg0, s32 *arg1);

s32 ov70_0223F8D0(s32 arg0, s32 *arg1) {
    s32 *var_r3;
    u32 var_r4;
    u8 temp_r2;

    var_r3 = &ov70_022459C8;
    var_r4 = 0;
loop_1:
    if (arg0 == *(u8 *)((u8 *)var_r3 + 1)) {
        temp_r2 = *(u8 *)(((u8 *)&ov70_022459C8) + (var_r4 * 2));
        *arg1 = (s32)temp_r2;
        return *(u8 *)(((u8 *)&ov70_022459C8) + ((var_r4 + 1) * 2)) - temp_r2;
    }
    var_r4 += 1;
    var_r3 = (void *)((u8 *)var_r3 + (2));
    if (var_r4 >= 0x16U) {
        return 0;
    }
    goto loop_1;
}
