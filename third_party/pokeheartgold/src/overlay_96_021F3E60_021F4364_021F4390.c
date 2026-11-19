#include "global.h"

#include "error_handling.h"
#include "sprite.h"

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

struct Sprite *ov96_021EB5B8(void *param_1);

void ov96_021F4390(u8 *param_1, s32 param_2, u32 param_3);

void ov96_021F4390(u8 *param_1, s32 param_2, u32 param_3) {
    Sprite *sprite;
    s32 seq;
    s32 iVar1;
    switch (param_3) {
    case 1:
        seq = 0x15;
        seq = 0x12;
        break;

    case 2:
        seq = 0x13;
        break;

    case 3:
        seq = 0x14;
        break;

    case 4:
        seq = 0x15;
        break;

    default:
        GF_AssertFail();
        seq = 0x12;
        break;
    }

    iVar1 = 4;
    iVar1 = ((u32)(*((u8 *)((param_1 + param_2) + 0x170)))) * iVar1;
    ov96_021EB52C(*((s16 **)((param_1 + 0x140) + iVar1)), 1, 1);
    sprite = ov96_021EB5B8(*((void **)((param_1 + 0x140) + iVar1)));
    Sprite_SetAnimCtrlSeq(sprite, seq);
    return;
}
