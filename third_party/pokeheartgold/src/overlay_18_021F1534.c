#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "pokedex.h"
#include "sprite.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F111C(PokedexAppData *, s32, void *, s32, s32);

void *ov18_021F148C(PokedexAppData *, u32, u32, NNSG2dCharacterData **);

void ov18_021F1534(PokedexAppData *arg0, u32 arg1, u32 arg2, s32 arg3);

void ov18_021F1160(PokedexAppData *, s32, u32);
void ov18_021F14FC(PokedexAppData *param_1, u32 param_2, u32 param_3, s32 param_4);
void ov18_021F1598(PokedexAppData *param_1, s32 param_2, s32 param_3);

void ov18_021F1534(PokedexAppData *arg0, u32 species, u32 arg2, s32 arg3) {
    NNSG2dCharacterData *v1;
    void *v0;
    u32 form;
    s32 v2;
    void *ptr;

    form = arg2;
    ptr = ov18_021F148C(arg0, species, arg2, &v1);
    v0 = ptr;
    ov18_021F111C(arg0, arg3, v1->pRawData, 0x200, 2);
    v2 = SpriteManager_FindPlttResourceOffset(arg0->unk_066C, 0xC551, 2);
    ManagedSprite_SetPaletteOverride(arg0->unk_0670[arg3], v2 + GetBattleMonIconPaletteEx(species, form, 0));
    Heap_Free(ptr);
}

void ov18_021F1598(PokedexAppData *param_1, s32 param_2, s32 param_3) {
    u32 uVar1;

    ManagedSprite_SetDrawFlag(param_1->unk_0670[param_3], 0);
    if (param_1->unk_1030[param_2].unk_0 != 0) {
        uVar1 = Pokedex_GetSeenFormByIdx(param_1->args->pokedex, (u32)param_1->unk_1030[param_2].unk_0, 0);
        if (param_1->unk_1030[param_2].unk_0 == 0xac) {
            if (uVar1 == 2) {
                uVar1 = 1;
            } else {
                uVar1 = 0;
            }
        }
        ov18_021F14FC(param_1, (u32)param_1->unk_1030[param_2].unk_0, uVar1, param_3);
        ManagedSprite_SetDrawFlag(param_1->unk_0670[param_3], 1);
        if (param_1->unk_1030[param_2].unk_2 == 1) {
            ov18_021F1160(param_1, param_3, 1);
            return;
        }
        ov18_021F1160(param_1, param_3, 0);
    }
    return;
}
