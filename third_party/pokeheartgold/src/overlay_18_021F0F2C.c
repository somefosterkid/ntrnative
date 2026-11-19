#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "sprite.h"
#include "sprite_system.h"


void ov18_021F0F2C(PokedexAppData *pokedexApp) {
    ClearWindowTilemapAndScheduleTransfer(pokedexApp->windows);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[3]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[8]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[9]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[7]);
}

s32 ov18_021EE3AC(PokedexAppData *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

s32 ov18_021F09D8(PokedexAppData *, s32);

void ov18_021F0F68(PokedexAppData *pokedexApp, int a1, int a2) {
    s32 v1;
    s32 v0;
    Window *window;
    u32 v2;

    window = pokedexApp->windows;
    v0 = a2;
    FillWindowPixelBuffer(&window[a1], 0U);
    v1 = ov18_021F09D8(pokedexApp, v0);
    v2 = GetWindowWidth(&window[a1]) * 8;
    ov18_021EE3AC(pokedexApp, pokedexApp->msgData, a1, v1, (s32)(v2 + (v2 >> 0x1F)) >> 1, 0, 0, 0x50900, 2);
    ScheduleWindowCopyToVram(&window[a1]);
}

s32 ov18_021F1024(PokedexAppData *);

s32 ov18_021F12FC(PokedexAppData *);

void ov18_021F0FC8(PokedexAppData *pokedexApp) {
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    ov18_021F12FC(pokedexApp);
    ov18_021F1024(pokedexApp);
}

s32 ov18_021F10C8(PokedexAppData *);

s32 ov18_021F1104(void *arg0);

s32 ov18_021F1314(PokedexAppData *);

void ov18_021F0FEC(PokedexAppData *pokedexApp) {
    ov18_021F1104(pokedexApp);
    ov18_021F10C8(pokedexApp);
    ov18_021F1314(pokedexApp);
}

void ov18_021F1004(PokedexAppData *pokedexApp) {
    u32 uVar1;

    uVar1 = 0;
    do {
        if (pokedexApp->unk_0670[0] != NULL) {
            ManagedSprite_TickFrame(pokedexApp->unk_0670[0]);
        }
        uVar1 = uVar1 + 1;
        pokedexApp = (PokedexAppData *)&pokedexApp->bgConfig;
    } while (uVar1 < 0x78);
    return;
}
