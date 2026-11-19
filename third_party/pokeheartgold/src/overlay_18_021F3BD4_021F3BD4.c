#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F3BD4(PokedexAppData *pokedexApp, int a1) {
    s16 sp0[2];

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[28], &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[28], sp0[1], (s16)(sp0[0] + a1));
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[43], &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[43], sp0[1], (s16)(sp0[0] + a1));
}
