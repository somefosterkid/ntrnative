#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov73_021E5D20(BgConfig *bgConfig);

extern const BgTemplate ov73_021EA5B0;

extern const BgTemplate ov73_021EA5CC;

extern const BgTemplate ov73_021EA578;

extern const BgTemplate ov73_021EA594;

extern const GraphicsModes ov73_021EA540;

void ov73_021E5D20(BgConfig *bgConfig) {
    GraphicsModes modes = ov73_021EA540;
    SetBothScreensModesAndDisable(&modes);

    BgTemplate mainWindowTemplate = ov73_021EA5B0;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)4, &mainWindowTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)4);

    BgTemplate mainBgTemplate = ov73_021EA5CC;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)5, &mainBgTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)5);

    BgTemplate subWindowTemplate = ov73_021EA578;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)0, &subWindowTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)0);

    BgTemplate subBgTemplate = ov73_021EA594;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &subBgTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 1);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_50);
    BG_ClearCharDataRange((enum GFBgLayer)4, 32, 0, HEAP_ID_50);
}
