#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct Window;

struct WindowTemplate;

void ov82_0223FD2C(BgConfig *bgConfig, Window *windows);

void ov82_0223FD2C(BgConfig *bgConfig, Window *windows) {
    static const WindowTemplate sWinTemplates[] = {
        { GF_BG_LYR_MAIN_0, 2,  19, 27, 4,  12, 1   },
        { GF_BG_LYR_MAIN_0, 0xA, 0x14, 9,  2,  13, 0x89 },
        { GF_BG_LYR_MAIN_1, 1, 0, 0x1F,  0x16,  13, 1 },
        { GF_BG_LYR_SUB_0, 2,  0x13,  0x1B, 4, 2, 0xA   }
    };

    for (u8 i = 0; i < 4; i++) {
        AddWindow(bgConfig, &windows[i], &sWinTemplates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}
