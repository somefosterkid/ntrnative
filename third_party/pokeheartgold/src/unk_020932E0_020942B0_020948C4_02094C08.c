#include "global.h"

#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094C08(UnkStruct_020932E0_Info *arg0, UnkStruct_020932E0_Slot20C *arg1, s32 arg2, Sprite *sprite, void *arg4, NARC *arg5, s32 arg6) {
    NNSG2dCharacterData *v0;
    UnkStruct_020932E0_Slot20C *v1;

    v1 = &arg1[arg2];
    if (sprite != NULL) {
        sub_02094C6C(GetMonIconNaixEx(arg0->unk_0, arg0->unk_8, arg0->unk_A), &v0, arg4, arg5);
        MIi_CpuCopyFast(v0->pRawData, v1->unk_C, 0x200U);
        v1->unk_0 = (arg6 + (arg2 * 0x10)) << 5;
        v1->unk_8 = sprite;
        v1->unk_4 = GetMonIconPaletteEx(arg0->unk_0, arg0->unk_A, arg0->unk_8) + 8;
        Sprite_SetPaletteOverride(sprite, v1->unk_4);
        return;
    }
    v1->unk_8 = NULL;
}
