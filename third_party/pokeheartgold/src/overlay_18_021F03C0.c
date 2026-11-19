#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"


s32 ov18_021F9648(Window *, MsgData *, s32, s32, s32, s32, s32, s32);

void ov18_021F03C0(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = pokedexApp->windows;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 0xe);
    ov18_021EE388(pokedexApp);
    return;
}

void ov18_021F03E0(PokedexAppData *pokedexApp) {
    FillWindowPixelBuffer(&pokedexApp->windows[1], 0U);
    ov18_021F9648(&pokedexApp->windows[1], pokedexApp->msgData, pokedexApp->unk_18C9 + 0x81, 0x1C, 0, 0, 0x20100, 2);
    ScheduleWindowCopyToVram(&pokedexApp->windows[1]);
}
