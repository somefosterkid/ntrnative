#include "global.h"
struct PokedexAppData;

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"


ManagedSprite *ov18_021F11EC(struct PokedexAppData *arg0, ManagedSpriteTemplate *arg1);

void ov18_021F118C(PokedexAppData *pokedexApp, int a1, int a2) {
    ManagedSprite_SetAnimationFrame(pokedexApp->unk_0670[a1], 0);
    ManagedSprite_SetAnim(pokedexApp->unk_0670[a1], a2);
    return;
}

int ov18_021F11AC(PokedexAppData *pokedexApp, int a1) {
    BOOL BVar1;

    BVar1 = ManagedSprite_IsAnimated(pokedexApp->unk_0670[a1]);
    return BVar1;
}

void ov18_021F11C0(PokedexAppData *pokedexApp, int a1, int a2) {
    if (a2 == 1) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 0);
    return;
}

ManagedSprite *ov18_021F11EC(PokedexAppData *arg0, ManagedSpriteTemplate *unkTemplate) {
    if (unkTemplate->vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        return SpriteSystem_NewSprite(arg0->unk_0668, arg0->unk_066C, unkTemplate);
    }
    return SpriteSystem_NewSpriteWithYOffset(arg0->unk_0668, arg0->unk_066C, unkTemplate, 0x200000);
}
