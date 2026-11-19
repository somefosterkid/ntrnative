#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

void ov18_021F9648(Window *, MsgData *, s32, s32, u32, u32, u32, u32);

extern const u16 ov18_021F9DC0[];

void ov18_021EFC9C(PokedexAppData *pokedexApp, u32 type, int windowId, int a3) {
    FillWindowPixelBuffer(&pokedexApp->windows[windowId], 0);
    if (a3 == -1) {
        a3 = (GetWindowWidth(&pokedexApp->windows[windowId]) * 8) / 2;
    }
    ov18_021F9648(&pokedexApp->windows[windowId], pokedexApp->msgData, ov18_021F9DC0[type], a3, 0, 4, 0x20100, 2);
}
