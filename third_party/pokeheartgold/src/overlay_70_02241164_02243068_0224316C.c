#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, s32, s32);

s32 ov70_0224190C(void *, s32);

s32 ov70_02242FC4(UnkStruct_0201956C *, void *, String *, s32, s32);

s32 ov70_02243EB8(UnkStruct_0201956C *, MsgData *, void *, s32);

s32 ov70_02243F7C(void *, s32);

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

s32 sub_020197F4(UnkStruct_0201956C *, s32);

extern void *ov70_02245E26;

extern void *ov70_02245E27;

s32 ov70_0224316C(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_0224316C(UnkStruct_ov70_02242E58 *arg0) {
    String *temp_r7;
    s32 temp_r3;
    s32 var_r0;
    s32 var_r4;
    s32 var_r6;
    s8 var_r1;
    u8 *var_r2;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u8 *base = (u8 *)arg0;
#endif

    sub_02019688(arg0->unk_1C, 0, 0x64, 0x22, 1);
    sub_02019B08(arg0->unk_1C, 0);
    ov70_0224190C(arg0, 4);
    MI_CpuFill8(arg0->unk_64, 1U, 0x1AU);
    var_r4 = 0;
    var_r6 = 0;
    do {
        temp_r7 = NewString_ReadMsgData(arg0->unk_24, var_r4 + 0x6E);
        if (ov70_02243F7C(arg0, var_r4) == 1) {
            var_r0 = 0xF0E02;
#if defined(SDK_PORT) || defined(NONMATCHING)
            var_r2 = &arg0->unk_64[var_r4];
#else
            var_r2 = base + var_r4 + 0x64;
#endif
            var_r1 = 1;
        } else {
            var_r0 = 0x80902;
#if defined(SDK_PORT) || defined(NONMATCHING)
            var_r2 = &arg0->unk_64[var_r4];
#else
            var_r2 = base + var_r4 + 0x64;
#endif
            var_r1 = 0;
        }
        *var_r2 = var_r1;
        ov70_02242FC4(arg0->unk_1C, &arg0->unk_4[var_r4], temp_r7, 2, var_r0);
        String_Delete(temp_r7);
        var_r4 += 1;
        var_r6 += 0x10;
    } while (var_r4 < 9);
    ov70_02243EB8(arg0->unk_1C, arg0->unk_24, &arg0->unk_4[14], 0x44);
    sub_020196E8(arg0->unk_1C, 0, 0x10, 0);
    sub_020197F4(arg0->unk_1C, 0);
    Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x3D);
    temp_r3 = arg0->unk_3C * 2;
    ov70_02238F9C(arg0->unk_C, (*(u8 *)(((u8 *)&ov70_02245E26) + temp_r3) + 0x10) * 8, *(u8 *)(((u8 *)&ov70_02245E27) + temp_r3) * 8);
    arg0->unk_48 = arg0->unk_3C;
    Sprite_SetDrawFlag(arg0->unk_C, 1);
    arg0->unk_4C = 5;
    return -1;
}
