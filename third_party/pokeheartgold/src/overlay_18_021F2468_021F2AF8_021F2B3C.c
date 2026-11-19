#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F2B3C(PokedexAppData *pokedexApp, int a1, s16 a2) {
    s16 asStack_16;
    s16 sStack_18;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &asStack_16, &sStack_18);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], asStack_16 + a2, sStack_18);
    return;
}

void ov18_021F2B70(PokedexAppData *pokedexApp, int a1, s16 a2) {
    s16 asStack_16;
    s16 sStack_18;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &asStack_16, &sStack_18);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], a2, sStack_18);
    return;
}
