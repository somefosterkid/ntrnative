#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EFBE8(PokedexAppData *pokedexApp, int a1) {
    int new_var;
    u8 bVar1;
    FillWindowPixelBuffer(pokedexApp->windows + a1, 0);
    bVar1 = GetWindowWidth(pokedexApp->windows + a1);
    new_var = ((u32)bVar1) * 8;
    ov18_021F9648(pokedexApp->windows + a1, pokedexApp->msgData, pokedexApp->dexSearchCriteria[0] + 0x29, new_var / 2, 0, 4, 0x20100, 2);
    return;
}

void ov18_021EFC3C(PokedexAppData *pokedexApp, int windowId) {
    u8 bVar1;
    s32 iVar2;
    u8 new_var;
    FillWindowPixelBuffer(pokedexApp->windows + windowId, 0);
    if (pokedexApp->dexSearchCriteria[1] == 0x1a) {
        iVar2 = 0x71;
    } else {
        iVar2 = pokedexApp->dexSearchCriteria[1] + 0x45;
    }
    bVar1 = GetWindowWidth(pokedexApp->windows + windowId);
    new_var = bVar1;
    ov18_021F9648(pokedexApp->windows + windowId, pokedexApp->msgData, iVar2, (new_var * 8) / 2, 0, 4, 0x20100, 2);
    return;
}
