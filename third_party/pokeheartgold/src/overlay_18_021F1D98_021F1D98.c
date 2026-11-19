#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

extern u32 ov18_021FA450;

void ov18_021F1D98(PokedexAppData *param_1, s32 param_2);

void ov18_021F1D98(PokedexAppData *param_1, s32 param_2) {
    s32 index;
    ManagedSprite **sprites;

    sprites = param_1->unk_0670;
    sprites[param_2] = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)&ov18_021FA450, FX32_CONST(512));
    index = SpriteManager_FindPlttResourceOffset(param_1->unk_066C, 0xc55b, 2);
    ManagedSprite_SetPaletteOverride(sprites[param_2], index);
    ManagedSprite_SetDrawFlag(sprites[param_2], 0);
    return;
}
