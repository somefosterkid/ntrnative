#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, s32, s32);

s32 ov70_02242D44(void *, s32, s32, s32);

extern void *ov70_02245DF8;

extern void *ov70_02245DF9;

s32 ov70_02243768(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov70_02243768(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r3;

    ov70_02242D44(arg0, 6, 0x20, arg3);
    sub_020198FC(arg0->unk_1C, 0, -4, 0, 4);
    arg0->unk_4C = 1;
    arg0->unk_50 = 0x11;
    arg0->unk_48 = 0xA;
    arg0->unk_3C = -1;
    temp_r3 = arg0->unk_48 * 2;
    ov70_02238F9C(arg0->unk_C, (*(u8 *)(((u8 *)&ov70_02245DF8) + temp_r3) + 0x10) * 8, *(u8 *)(((u8 *)&ov70_02245DF9) + temp_r3) * 8);
    Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x31);
    return -1;
}
