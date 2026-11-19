#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F3AD8(PokedexAppData *pokedexApp) {
    s16 asStack_e;
    s16 sStack_10;

    if (pokedexApp->unk_1860 == 1) {
        ov18_021F11C0(pokedexApp, 0x11, 1);
        ov18_021F2AC0(pokedexApp, 0x11);
        ManagedSprite_GetPositionXY(pokedexApp->unk_0670[0x11], &asStack_e, &sStack_10);
        ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0x11], asStack_e + -0x120, sStack_10);
    }
    return;
}
