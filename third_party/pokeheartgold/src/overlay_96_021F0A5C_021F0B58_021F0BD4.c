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

void Bg_ScheduleAffineRotationCenter(BgConfig *bgConfig, u8 bgLayer, u8 bgAffineUpdateOp, int val);

void ov96_021F0BD4(BgConfig *bgConfig);

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);

extern const GraphicsModes ov96_0221BC7C;

extern const BgTemplate ov96_0221BCCC;

extern const BgTemplate ov96_0221BCE8;

extern const BgTemplate ov96_0221BD04;

extern const BgTemplate ov96_0221BD20;

extern const BgTemplate ov96_0221BD3C;

extern const BgTemplate ov96_0221BD58;

extern const BgTemplate ov96_0221BD74;

extern const BgTemplate ov96_0221BD90;

void ov96_021F0BD4(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov96_0221BC7C;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate layerMain0 = ov96_0221BCE8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &layerMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate layerMain1 = ov96_0221BD04;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &layerMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate layerMain2 = ov96_0221BD3C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &layerMain2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate layerMain3 = ov96_0221BD58;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &layerMain3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate layerSub0 = ov96_0221BD74;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &layerSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate layerSub1 = ov96_0221BD90;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &layerSub1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate layerSub3 = ov96_0221BCCC;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &layerSub3, BG_TYPE_AFFINE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);

    BgTemplate layerSub2 = ov96_0221BD20;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &layerSub2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    GfGfx_EngineATogglePlanes((enum GFBgLayer)8, 0);
}
