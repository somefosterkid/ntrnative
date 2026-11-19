#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F6578(PokedexAppData *pokedexApp, int a1, int a2) {
    s16 pos[2];
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], pos[1], (s16)(pos[0] + a2));
    return;
}
