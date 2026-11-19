#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void ov87_021E6C60(BgConfig *bgConfig);

extern const GraphicsModes ov87_021E81B0;

extern const BgTemplate ov87_021E8228;

extern const BgTemplate ov87_021E8260;

extern const BgTemplate ov87_021E81D4;

extern const BgTemplate ov87_021E81F0;

extern const BgTemplate ov87_021E820C;

extern const BgTemplate ov87_021E8244;

void ov87_021E6C60(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov87_021E81B0;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplateSub0 = ov87_021E8228;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplateSub0, BG_TYPE_STATIC);
    BG_ClearCharDataRange(4, 32, 0, (enum HeapID)122);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplateSub3 = ov87_021E8260;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bgTemplateSub3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);

    BgTemplate bgTemplateSub2 = ov87_021E81D4;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bgTemplateSub2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate bgTemplateMain1 = ov87_021E81F0;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgTemplateMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgTemplateMain0 = ov87_021E820C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgTemplateMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplateMain2 = ov87_021E8244;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgTemplateMain2, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, (enum HeapID)122);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, FALSE);
}
