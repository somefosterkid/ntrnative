#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"

void ov18_021F2BB0(PokedexAppData *app, int index) {
    s32 value = app->unk_185A;
    ManagedSprite **sprites = app->unk_0670;
    s32 rem = value % 5;
    s32 quot = value / 5;

    ManagedSprite_SetPositionXY(sprites[index], rem * 0x28 + 0x30, quot * 0x28 + 0x18);
    ManagedSprite_SetDrawFlag(sprites[index], 1);
}
