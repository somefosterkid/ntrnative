#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_33.h"
#include "pm_string.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

struct BgConfig;

struct BgTemplate;

extern const BgTemplate ov33_0225DA68;

extern const BgTemplate ov33_0225DA84;

extern const BgTemplate ov33_0225DAA0;

void ov33_0225D720(BgConfig *bgConfig);

void ov33_0225D720(BgConfig *bgConfig) {
    BgTemplate template1;
    BgTemplate template2;
    BgTemplate template3;
    GXS_SetGraphicsMode(GX_BGMODE_0);
    template1 = ov33_0225DA68;
    InitBgFromTemplate(bgConfig, 4, &template1, 0);
    BG_ClearCharDataRange(4, 0x20, 0, HEAP_ID_8);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    template2 = ov33_0225DA84;
    InitBgFromTemplate(bgConfig, 5, &template2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 5);
    template3 = ov33_0225DAA0;
    InitBgFromTemplate(bgConfig, 6, &template3, 0);
}
