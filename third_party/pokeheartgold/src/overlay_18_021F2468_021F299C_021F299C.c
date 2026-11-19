#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite.h"
#include "sprite_system.h"

extern u32 ov18_021FA984;

extern u32 ov18_021FAB58;

extern u32 ov18_021FAB8C;

void ov18_021F299C(PokedexAppData *param_1);

void ov18_021F299C(PokedexAppData *param_1) {
    ManagedSprite *pMVar1;
    ManagedSpriteTemplate *unkTemplate;
    u32 uVar3;
    unkTemplate = (ManagedSpriteTemplate *)(&ov18_021FA984);
    uVar3 = 0;
    do {
        pMVar1 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, unkTemplate);
        param_1->unk_0670[uVar3] = pMVar1;
        unkTemplate = (ManagedSpriteTemplate *)((u8 *)unkTemplate + 0x34);
        uVar3 = uVar3 + 1;
    } while (uVar3 <= (8 - 1));
    ManagedSprite_SetDrawFlag(param_1->unk_0670[6], 0);
    ManagedSprite_SetDrawFlag(param_1->unk_0670[7], 0);
    pMVar1 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)(&ov18_021FAB58), FX32_CONST(512));
    param_1->unk_0670[9] = pMVar1;
    pMVar1 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)(&ov18_021FAB8C), FX32_CONST(512));
    param_1->unk_0670[10] = pMVar1;
    return;
}
