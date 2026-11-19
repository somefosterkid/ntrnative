#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "unk_0201956C.h"


void ov18_021F014C(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;
    uVar1 = 0x51;
    window = pokedexApp->windows + 0x51;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 <= (0x5e - 1));
    return;
}

void ov18_021F0168(PokedexAppData *pokedexApp) {
    u32 v0;

    v0 = 0;
    do {
        sub_020195F4(pokedexApp->unk_0008, v0 + 0x11, 2, 0x12, 2);
        v0 += 1;
    } while (v0 < 6U);
}
