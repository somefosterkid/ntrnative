#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, int, int);

s32 ov70_0223F904(ListMenuItem **, MsgData *, s32);

s32 ov70_0224190C(void *, s32);

s32 ov70_02242508(s32, s32);

s32 ov70_0224352C(void *, s32, s32);

s32 ov70_02243EB8(UnkStruct_0201956C *, void *, void *, s32);

void ov70_02243F00(UnkStruct_0201956C *, MessagePrinter *, Window *, s32, u32);

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

extern const u8 ov70_02245D8A;

extern const u8 ov70_02245D8B;

extern const void *ov70_02245E84;

s32 ov70_02243B2C(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243B2C(UnkStruct_ov70_02242E58 *arg0) {
    s32 i;
    sub_02019688(arg0->unk_1C, 0, 0x64, 0x1E, 1);
    sub_02019B08(arg0->unk_1C, 0);
    ov70_0224190C(arg0, 3);
    arg0->unk_5C = ov70_0223F904(&arg0->unk_34, arg0->unk_2C, arg0->unk_3E + *(s32 *)(&ov70_02245E84 + arg0->unk_3C));
    ov70_0224352C(arg0, 0, arg0->unk_5C);
    arg0->unk_5A = 0;
    ov70_02243F00(arg0->unk_1C, arg0->unk_38, &arg0->unk_4[5], arg0->unk_5A, ov70_02242508(arg0->unk_5C, 5));
    ov70_02243EB8(arg0->unk_1C, arg0->unk_24, &arg0->unk_4[14], 0x44);
    sub_020196E8(arg0->unk_1C, 0, 1, 0);
    arg0->unk_48 = 0;
    i = arg0->unk_48;
    ov70_02238F9C(arg0->unk_C, (&ov70_02245D8A)[i * 2], (&ov70_02245D8B)[i * 2]);
    ov70_02238F9C(arg0->unk_10, 0xB0U, 0x88U);
    ov70_02238F9C(arg0->unk_14, 0x58U, 0x88U);
    Sprite_SetDrawFlag(arg0->unk_C, 1);
    Sprite_SetDrawFlag(arg0->unk_10, 1);
    Sprite_SetDrawFlag(arg0->unk_14, 1);
    arg0->unk_4C = 0x19;
    return -1;
}
