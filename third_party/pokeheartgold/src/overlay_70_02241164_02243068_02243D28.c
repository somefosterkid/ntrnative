#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s8 ov70_022426F4(UnkStruct_ov70_02242E58 *work);

s32 ov70_02243D28(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243D28(UnkStruct_ov70_02242E58 *arg0) {
    s8 v0;

    v0 = ov70_022426F4(arg0);
    switch (v0) {
    case -1:
        break;
    case -2:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x1E;
        arg0->unk_44 = -2;
        break;
    default:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x1E;
        arg0->unk_44 = v0;
        break;
    }
    return -1;
}

s32 ov70_02241DB4(void *, s32);

s8 ov70_02243D90(UnkStruct_ov70_02242E58 *arg0);

s8 ov70_02243D90(UnkStruct_ov70_02242E58 *arg0) {
    ov70_02241DB4(arg0, 1);
    return arg0->unk_44;
}
