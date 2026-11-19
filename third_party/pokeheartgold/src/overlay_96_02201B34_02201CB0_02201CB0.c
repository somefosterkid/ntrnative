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

void Bg_ScheduleAffineRotationCenter(BgConfig *bgConfig, u8 bgLayer);

void ov96_02201CB0(BgConfig *bgConfig);

extern const GraphicsModes ov96_0221C7DC;

extern const BgTemplate ov96_0221C83C;

extern const BgTemplate ov96_0221C858;

extern const BgTemplate ov96_0221C890;

extern const BgTemplate ov96_0221C8AC;

extern const BgTemplate ov96_0221C804;

extern const BgTemplate ov96_0221C820;

extern const BgTemplate ov96_0221C874;

void ov96_02201CB0(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov96_0221C7DC;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate layerMain0 = ov96_0221C83C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &layerMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate layerMain1 = ov96_0221C858;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &layerMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate layerMain2 = ov96_0221C890;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &layerMain2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate layerMain3 = ov96_0221C8AC;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &layerMain3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate layerSub0 = ov96_0221C804;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)5, &layerSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)5);

    BgTemplate layerSub1 = ov96_0221C820;
    InitBgFromTemplate(bgConfig, (enum GFBgLayer)6, &layerSub1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)6);

    BgTemplate layerSub3 = ov96_0221C874;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &layerSub3, BG_TYPE_STATIC);

    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineATogglePlanes(8, 0);
}
