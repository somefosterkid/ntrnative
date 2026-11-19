#include "global.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

extern const GraphicsModes ov112_021F3684;

extern const BgTemplate ov112_021F3704;

extern const BgTemplate ov112_021F3720;

extern const BgTemplate ov112_021F373C;

extern const BgTemplate ov112_021F3758;

extern const BgTemplate ov112_021F3774;

extern const BgTemplate ov112_021F3790;

void ov112_021E79F0(BgConfig *bgConfig);

void ov112_021E79F0(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov112_021F3684;

    SetBothScreensModesAndDisable(&graphicsModes);

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &ov112_021F3704, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &ov112_021F3720, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &ov112_021F373C, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &ov112_021F3758, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &ov112_021F3774, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &ov112_021F3790, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_154);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, HEAP_ID_154);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, HEAP_ID_154);
}
