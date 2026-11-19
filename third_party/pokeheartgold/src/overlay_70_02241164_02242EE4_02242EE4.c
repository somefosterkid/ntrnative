#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, int, int);

s32 ov70_0223F7E4(void *, void *, s32);

s32 ov70_0224190C(void *, s32);

s32 ov70_02242508(s32, s32);

s32 ov70_022434C0(void *, s32, s32);

s32 ov70_02243EB8(UnkStruct_0201956C *, void *, void *, s32);

void ov70_02243F00(UnkStruct_0201956C *, MessagePrinter *, Window *, s32, u32);

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

s32 sub_020197F4(UnkStruct_0201956C *, s32);

extern void *ov70_02245D76;

extern void *ov70_02245D77;

void ov70_02242EE4(UnkStruct_ov70_02242E58 *arg0);

void ov70_02242EE4(UnkStruct_ov70_02242E58 *arg0) {
    s32 temp_r0;
    s32 temp_r3;
    sub_02019688(arg0->unk_1C, 0, 0x64, 0x23, 1);
    sub_02019B08(arg0->unk_1C, 0);
    ov70_0224190C(arg0, 2);
    temp_r0 = arg0->unk_60;
    if (temp_r0 == 1) {
        arg0->unk_5C = ov70_0223F7E4(&arg0->unk_34, arg0->unk_24, 1);
    } else if (temp_r0 == 0) {
        arg0->unk_5C = ov70_0223F7E4(&arg0->unk_34, arg0->unk_24, 0);
    }
    ov70_022434C0(arg0, 0, arg0->unk_5C);
    arg0->unk_5A = 0;
    ov70_02243F00(arg0->unk_1C, arg0->unk_38, &arg0->unk_4[4], arg0->unk_5A, ov70_02242508(arg0->unk_5C, 4));
    ov70_02243EB8(arg0->unk_1C, arg0->unk_24, &arg0->unk_4[14], 0x44);
    sub_020196E8(arg0->unk_1C, 0, 0x20, 0);
    Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x31);
    arg0->unk_48 = 0;
    arg0->unk_7E = 0;
    temp_r3 = (arg0->unk_48) * 2;
    ov70_02238F9C(arg0->unk_C, *((u8 *)(((u8 *)&ov70_02245D76) + temp_r3)), *((u8 *)(((u8 *)&ov70_02245D77) + temp_r3)));
    sub_020197F4(arg0->unk_1C, 0);
    sub_020198FC(arg0->unk_1C, 0, -4, 0, 4);
    arg0->unk_4C = 0x20;
}
