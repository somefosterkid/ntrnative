#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F2E4C(PokedexAppData *pokedexApp, int a1, int a2) {
    unsigned int new_var;
    s16 asStack_16;
    s16 sStack_18;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &asStack_16, &sStack_18);
    new_var = sStack_18;
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], asStack_16, (s16)(((u32)((new_var + a2) * 0x10000)) >> 0x10));
    return;
}

void ov18_021F2E80(PokedexAppData *pokedexApp, int a1, int a2) {
    s32 iVar1;
    s16 asStack_1a;
    s16 sStack_1c;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &asStack_1a, &sStack_1c);
    iVar1 = ov18_021F2DD4(pokedexApp, (u32)pokedexApp->unk_1859, a1, a2);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], asStack_1a, (s16)iVar1);
    return;
}
