#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02241DB4(UnkStruct_ov70_02242E58 *, s32);

s32 ov70_02243AC4(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243AC4(UnkStruct_ov70_02242E58 *arg0) {
    Sprite *sprite;

    ov70_02241DB4(arg0, 5);
    Sprite_SetDrawFlag(arg0->unk_C, 0);
    if ((s32) arg0->unk_3E < 0) {
        arg0->unk_4C = 0x13;
    } else {
        sub_020198FC(arg0->unk_1C, 0, -4, 0, 3);
        sprite = arg0->unk_18;
        if (sprite != NULL) {
            Sprite_SetDrawFlag(sprite, 0);
        }
        Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x32);
        arg0->unk_4C = 2;
        arg0->unk_50 = 0x18;
    }
    return -1;
}

s32 ov70_02243B1C(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243B1C(UnkStruct_ov70_02242E58 *arg0) {
    arg0->unk_4C = 0x14;
    arg0->unk_48 = arg0->unk_3E;
    return -1;
}
