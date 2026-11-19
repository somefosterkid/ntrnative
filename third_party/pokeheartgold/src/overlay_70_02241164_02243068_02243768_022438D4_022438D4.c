#include "global.h"

#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, s32, s32);

s32 ov70_02242D44(void *, s32, s32);

extern const u8 ov70_02245DF8[];

extern const u8 ov70_02245DF9[];

s32 ov70_022438D4(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_022438D4(UnkStruct_ov70_02242E58 *arg0) {
    Sprite *sprite;
    s16 idx;

    ov70_02242D44(arg0, 6, 0x10);
    Sprite_SetAnimCtrlSeq(arg0->unk_C, 0x3D);
    Sprite_SetDrawFlag(arg0->unk_C, 1);
    idx = arg0->unk_3C;
    arg0->unk_48 = idx;
    ov70_02238F9C(arg0->unk_C, (ov70_02245DF8[idx * 2] + 0x10) * 8, ov70_02245DF9[idx * 2] * 8);
    sprite = arg0->unk_18;
    if (sprite != NULL) {
        Sprite_SetDrawFlag(sprite, 1);
    }
    arg0->unk_4C = 0x11;
    return -1;
}
