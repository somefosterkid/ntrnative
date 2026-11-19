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

void GfGfx_EngineATogglePlanes(u8 param_1, u8 param_2);

void ov96_021FCF00(BgConfig *bgConfig);

extern const GraphicsModes ov96_0221C61C;

extern const BgTemplate ov96_0221C648;

extern const BgTemplate ov96_0221C664;

extern const BgTemplate ov96_0221C680;

extern const BgTemplate ov96_0221C69C;

extern const BgTemplate ov96_0221C6B8;

extern const BgTemplate ov96_0221C6D4;

extern const BgTemplate ov96_0221C62C;

void ov96_021FCF00(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov96_0221C61C;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate layerMain0 = ov96_0221C648;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &layerMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate layerMain1 = ov96_0221C664;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &layerMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate layerMain2 = ov96_0221C680;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &layerMain2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate layerMain3 = ov96_0221C69C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &layerMain3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate layerSub0 = ov96_0221C6B8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &layerSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate layerSub1 = ov96_0221C6D4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &layerSub1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate layerSub3 = ov96_0221C62C;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)6, &layerSub3, BG_TYPE_STATIC);

    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)6);
    GfGfx_EngineATogglePlanes(8, 0);
}
