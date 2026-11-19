#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

u32 ov70_022429B8(UnkStruct_ov70_02242E58 *, u8 *, s32);

s32 ov70_022437C8(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2);

s32 ov70_022437C8(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2) {
    u32 temp_r0;

    temp_r0 = ov70_022429B8(arg0, arg0->unk_64, arg2);
    switch (temp_r0) {
    case -2U:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x12;
        arg0->unk_3C = -1;
        break;
    case 0xA:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x12;
        arg0->unk_3C = -2;
        break;
    case -1U:
        break;
    default:
        if (temp_r0 > 8U) {
            GF_AssertFail();
        }
        arg0->unk_3C = (s16)temp_r0;
        arg0->unk_4C = 0x12;
        break;
    }
    return -1;
}
