#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s16 ov70_02242C64(UnkStruct_ov70_02242E58 *work);

s32 ov70_02243C08(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243C08(UnkStruct_ov70_02242E58 *arg0) {
    s16 v0;

    v0 = ov70_02242C64(arg0);
    switch (v0) {
    case -1:
        break;
    case -2:
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        arg0->unk_4C = 2;
        arg0->unk_50 = 0x1A;
        break;
    default:
        sub_020198FC(arg0->unk_1C, 0, 6, 0, 6);
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        Sprite_SetDrawFlag(arg0->unk_10, 0);
        Sprite_SetDrawFlag(arg0->unk_14, 0);
        arg0->unk_4C = 3;
        arg0->unk_50 = 0x1B;
        arg0->unk_42 = v0;
        break;
    }
    return -1;
}
