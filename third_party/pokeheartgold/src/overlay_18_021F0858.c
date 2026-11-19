#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021EE35C(PokedexAppData *, u8 *, u32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

extern u32 ov18_021F9DB0;

void ov18_021F0858(PokedexAppData *pokedexApp) {
    Window *pWVar1;
    u32 uVar2;

    ov18_021EE35C((struct PokedexAppData *)((unsigned char *)(pokedexApp)), (unsigned char *)(&ov18_021F9DB0), 2);
    uVar2 = 0;
    pWVar1 = pokedexApp->windows;
    do {
        FillWindowPixelBuffer(pWVar1, 0);
        uVar2 = uVar2 + 1;
        pWVar1 = pWVar1 + 1;
    } while (uVar2 < 2);
    ov18_021F9648(pokedexApp->windows, pokedexApp->msgData, 0xad, 0, 0, 4, 0x20100, 0);
    ov18_021F9648(pokedexApp->windows + 1, pokedexApp->msgData, 0xae, 0x3c, 0, 4, 0xf0c00, 2);
    uVar2 = 0;
    pWVar1 = pokedexApp->windows;
    do {
        ScheduleWindowCopyToVram(pWVar1);
        uVar2 = uVar2 + 1;
        pWVar1 = pWVar1 + 1;
    } while (uVar2 < 2);
    return;
}

void ov18_021F08E0(PokedexAppData *pokedexApp) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = pokedexApp->windows;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 2);
    ov18_021EE388(pokedexApp);
    return;
}
