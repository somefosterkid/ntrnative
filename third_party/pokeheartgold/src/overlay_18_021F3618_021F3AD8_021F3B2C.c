#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F3B2C(PokedexAppData *pokedexApp, int a1) {
    s16 sp0[2];

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[17], &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[17], (s16)(sp0[1] + a1), sp0[0]);
}
