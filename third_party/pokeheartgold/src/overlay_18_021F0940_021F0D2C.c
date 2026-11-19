#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"

s32 ov18_021EE3AC(PokedexAppData *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

s32 ov18_021F09D8(PokedexAppData *, s32);

void ov18_021F0D2C(PokedexAppData *pokedexApp) {
    FillWindowPixelBuffer(&pokedexApp->windows[2], 0U);
    ov18_021EE3AC(pokedexApp, pokedexApp->msgData, 2, ov18_021F09D8(pokedexApp, pokedexApp->unk_18C5), 0x3C, 0, 0, 0x20100, 2);
    ScheduleWindowCopyToVram(&pokedexApp->windows[2]);
}
