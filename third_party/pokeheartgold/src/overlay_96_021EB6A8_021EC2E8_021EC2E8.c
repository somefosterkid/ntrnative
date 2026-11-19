#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_FillCharDataRange(BgConfig *bgConfig, enum GFBgLayer bgId, u32 fillValue, u32 ntiles, u32 offset);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov96_021EC2E8(BgConfig *param0);

extern const GraphicsModes ov96_0221AF64;

extern const BgTemplate ov96_0221AFA0;

extern const BgTemplate ov96_0221AFF4;

extern const BgTemplate ov96_0221AFBC;

extern const BgTemplate ov96_0221AFD8;

void ov96_021EC2E8(BgConfig *param0) {
    {
        GraphicsModes v0 = ov96_0221AF64;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov96_0221AFA0;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
        BG_FillCharDataRange(param0, GF_BG_LYR_MAIN_1, 0, 1, 0);
    }

    {
        BgTemplate v2 = ov96_0221AFF4;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
        BG_FillCharDataRange(param0, GF_BG_LYR_MAIN_1, 0, 1, 0);
    }

    {
        BgTemplate v4 = ov96_0221AFBC;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v4, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v5 = ov96_0221AFD8;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v5, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);
    }
}
