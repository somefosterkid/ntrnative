#include "global.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2ED0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3, s32 param_4);

u32 ov14_021E6070(UnkStruct_ov14_boxapp *param_1, u32 param_2, s32 param_3, void *param_4);
BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);

void ov14_021F2ED0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3, s32 param_4) {
    u32 uVar1;
    BoxPokemon *pBVar2;
    NNSG2dCharacterData *pNVar3;
    ManagedSprite **sprites;

    sprites = param_1->sys->sprites;
    ManagedSprite_SetDrawFlag(sprites[param_4], 0);
    pNVar3 = NULL;
    uVar1 = ov14_021E6070(param_1, param_3, 0xac, (void *)0x0);
    if (uVar1 != 0) {
        pBVar2 = ov14_021E60C0(param_1, param_2, param_3);
        ov14_021F2EA0(param_1, pBVar2, param_4);
        ManagedSprite_SetDrawFlag(sprites[param_4], 1);
    }
    return;
}
