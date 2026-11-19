#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

extern GraphicsModes ov96_0221B9BC;

extern BgTemplate ov96_0221B9CC;

void ov96_021EEF0C(void *param_1, s32 param_2);

void ov96_021EEF0C(void *param_1, s32 param_2) {
    GraphicsModes GStack_24;
    BgTemplate BStack_40;

    GStack_24 = ov96_0221B9BC;
    SetBothScreensModesAndDisable(&GStack_24);
    BStack_40 = ov96_0221B9CC;
    InitBgFromTemplate((BgConfig *)param_1, 0, &BStack_40, 0);
    BgClearTilemapBufferAndCommit((BgConfig *)param_1, 0);
    BG_ClearCharDataRange(0, 0x40, 0, (enum HeapID)param_2);
    GfGfx_EngineATogglePlanes(8, 0);
    GfGfx_EngineATogglePlanes(4, 0);
    GfGfx_EngineATogglePlanes(2, 0);
    GfGfx_EngineATogglePlanes(1, 1);
    BG_SetMaskColor(0, 0);
    BG_SetMaskColor(4, 0);
    return;
}
