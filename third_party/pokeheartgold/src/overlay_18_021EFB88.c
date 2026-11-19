#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

s32 ov18_021EF388(PokedexAppData *, s32, s32);

s32 ov18_021F9648(Window *, MsgData *, s32, s32, s32, s32, s32, s32);

void ov18_021EFB88(PokedexAppData *pokedexApp) {
    ov18_021E613C(pokedexApp, 0U);
    FillWindowPixelBuffer(&pokedexApp->windows[0x50], 0U);
    ov18_021F9648(&pokedexApp->windows[0x50], pokedexApp->msgData, 0x20, 0x18, 0, 0, 0x20100, 2);
    ov18_021EF388(pokedexApp, 0x11, 0x27);
    ov18_021EF388(pokedexApp, 0x13, 0x28);
    ScheduleWindowCopyToVram(&pokedexApp->windows[0x50]);
}
