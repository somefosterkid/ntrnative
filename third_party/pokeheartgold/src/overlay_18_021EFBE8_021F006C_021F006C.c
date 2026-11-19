#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "unk_0201956C.h"

void ov18_021F006C(PokedexAppData *arg0, s32 arg1);

void ov18_021F006C(PokedexAppData *arg0, s32 arg1) {
    u16 *buf;
    Window *windows;
    u16 tile;
    u8 x, y, w, h;
    u32 i, j;

    buf = sub_02019B08(arg0->unk_0008, 0xF);
    windows = arg0->windows;
    tile = GetWindowBaseTile(&windows[arg1]);
    x = GetWindowX(&windows[arg1]);
    y = GetWindowY(&windows[arg1]);
    w = GetWindowWidth(&windows[arg1]);
    h = GetWindowHeight(&windows[arg1]);
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            buf[(y + i) * 32 + x + j] = (buf[(y + i) * 32 + x + j] & 0xF000) + tile + i * w + j;
        }
    }
    CopyWindowPixelsToVram_TextMode(&windows[arg1]);
}

void ov18_021F0118(PokedexAppData *pokedexApp, int a1) {
    u32 i;
    if (a1 == 1) {
        for (i = 0; i <= 16; i++) {
            ScheduleWindowCopyToVram(&pokedexApp->windows[i]);
        }
    } else {
        for (i = 0; i <= 16; i++) {
            ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[i]);
        }
    }
}
