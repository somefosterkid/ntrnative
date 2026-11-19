#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void ov68_021E5BC0(BgConfig *bgConfig);

extern const GraphicsModes ov68_021E7BF8;

extern const BgTemplate ov68_021E7C60;

extern const BgTemplate ov68_021E7C44;

extern const BgTemplate ov68_021E7C7C;

extern const BgTemplate ov68_021E7C98;

extern const BgTemplate ov68_021E7CB4;

void ov68_021E5BC0(BgConfig *bgConfig) {
    GraphicsModes mode = ov68_021E7BF8;

    SetBothScreensModesAndDisable(&mode);

    BgTemplate mainWindowTemplate = ov68_021E7C60;

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)2, &mainWindowTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate mainBgTemplate = ov68_021E7C44;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainBgTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate mainBgOverlayTemplate = ov68_021E7C7C;

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)4, &mainBgOverlayTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)4);

    BgTemplate subWindowTemplate = ov68_021E7C98;
    InitBgFromTemplate(bgConfig, 6, &subWindowTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 6);

    BgTemplate subBgTemplate = ov68_021E7CB4;

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)7, &subBgTemplate, 0);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)7);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_66);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_66);
}
