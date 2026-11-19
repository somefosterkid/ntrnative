#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s16 ov70_0224261C(UnkStruct_ov70_02242E58 *work);

s32 ov70_02243688(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243688(UnkStruct_ov70_02242E58 *arg0) {
    s16 v0;

    v0 = ov70_0224261C(arg0);
    switch (v0) {
    case -1:
        break;
    case -2:
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        PlaySE(0x5DCU);
        arg0->unk_4C = 0xE;
        arg0->unk_40 = -1;
        break;
    default:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0xF;
        arg0->unk_40 = v0;
        break;
    }
    return -1;
}
