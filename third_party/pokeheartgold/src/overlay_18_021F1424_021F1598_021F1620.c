#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"

void ov18_021F1598(PokedexAppData *param_1, s32 param_2, s32 param_3);

void ov18_021F1620(PokedexAppData *app, int base) {
    u16 i;
    u16 slot;

    i = 0;
    do {
        slot = base + i + (app->unk_185E ^ 1) * 0x1e;
        ManagedSprite_SetDrawFlag(app->unk_0670[slot], 0);
        i = i + 1;
    } while (i < 0x1e);

    i = 0;
    do {
        s32 rem;
        s32 quot;

        slot = base + i + app->unk_185E * 0x1e;
        ov18_021F1598(app, i + app->unk_1859 * 0xf, slot);
        rem = (s32)i % 5;
        quot = (s32)i / 5;
        ManagedSprite_SetPositionXY(app->unk_0670[slot], rem * 0x28 + 0x30, quot * 0x28 + 0x18);
        i = i + 1;
    } while (i < 0x1e);
}
