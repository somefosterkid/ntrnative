#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void ov70_02239134(BgConfig *bgConfig);

void ov70_022391F0(BgConfig *bgConfig, u32 y, u32 flag);

extern const GraphicsModes ov70_0224525C;

extern const BgTemplate ov70_022452F4;

extern const BgTemplate ov70_022452D8;

extern const BgTemplate ov70_02245310;

void ov70_02239134(BgConfig *bgConfig) {
    int new_var;
    GraphicsModes graphicsModes = ov70_0224525C;
    SetBothScreensModesAndDisable(&graphicsModes);
    BgTemplate bgMain0 = ov70_022452F4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgTemplate bgMain1 = ov70_022452D8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgMain1, BG_TYPE_STATIC);
    BgTemplate bgMain2 = ov70_02245310;
    new_var = 16;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgMain2, BG_TYPE_STATIC);
    ov70_022391F0(bgConfig, GF_BG_LYR_MAIN_0, 0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, (enum HeapID)61);
    GfGfx_EngineATogglePlanes(new_var, GF_BG_LYR_MAIN_1);
    GfGfx_EngineBTogglePlanes(new_var, GF_BG_LYR_MAIN_1);
}

typedef enum HeapID HeapID;

extern BgTemplate ov70_022452A0;

extern BgTemplate ov70_02245284;

extern BgTemplate ov70_022452BC;

extern BgTemplate ov70_0224532C;

void ov70_022391F0(BgConfig *bgConfig, u32 y, u32 flag) {
    BgTemplate template0;
    BgTemplate template1;
    BgTemplate template2;
    BgTemplate template3;

    template0 = ov70_022452A0;
    InitBgFromTemplate(bgConfig, 4, &template0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);

    template1 = ov70_02245284;
    InitBgFromTemplate(bgConfig, 6, &template1, 0);
    if (flag == 0) {
        BgClearTilemapBufferAndCommit(bgConfig, 6);
    }

    template2 = ov70_022452BC;
    template2.y = y;
    InitBgFromTemplate(bgConfig, 5, &template2, 0);
    BG_ClearCharDataRange((HeapID)(4), 0x20, 0, (HeapID)(61));

    template3 = ov70_0224532C;
    InitBgFromTemplate(bgConfig, 7, &template3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 7);
}
