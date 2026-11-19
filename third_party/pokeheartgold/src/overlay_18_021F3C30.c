#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

void ov18_021F3C30(PokedexAppData *pokedexApp) {
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[25], 0x30, -0x60);
    ov18_021F11C0(pokedexApp, 0x19, 1);
}
