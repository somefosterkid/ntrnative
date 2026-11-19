#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

int ov18_021F6540(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    s16 pos[2];
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &pos[1], &pos[0]);
    if ((u32)a2 >= (u32)pos[0]) {
        return (u32)(a2 - pos[0]) / (u32)a3;
    }
    return (u32)(pos[0] - a2) / (u32)a3;
}
