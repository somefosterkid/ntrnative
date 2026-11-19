#include "global.h"

#include "screen_fade_fx.h"

s32 sub_02010A00(s32, s32 *, s32, s32);

s32 sub_02010A7C(s32, s32);

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *, s32);

void sub_02011D08(UnkStruct_02011360 *arg0);

void sub_02011D08(UnkStruct_02011360 *arg0) {
    s32 sp0[192];
    s32 *new_var;
    s32 *var_r4;
    s32 temp_r1;
    s32 var_r6;
    void *var_r5;
    var_r5 = sub_02010EE0(&arg0->unk_00, 0);
    temp_r1 = arg0->unk_0C;
    sub_02010A00(((s32)(temp_r1 + (((u32)(temp_r1 >> 6)) >> 0x19))) >> 7, sp0, 0xC0, 0);
    var_r6 = 0;
    var_r4 = sp0;
    do {
        *((s16 *)(0x300 + ((u8 *)var_r5))) = sub_02010A7C(0x80, 0 - (*var_r4));
        new_var = &(*var_r4);
        *((s16 *)(((u8 *)var_r5) + 0x480)) = sub_02010A7C(0x80, *new_var);
        var_r6 += 1;
        var_r4 += 1;
        var_r5 += 2;
    } while (var_r6 < 0xC0);
}
