#include "global.h"

#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov70_0223CE44(BgConfig *param0);

void GfGfx_EngineBTogglePlanes(u8 planeMask, u8 enable);

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);

void ov70_022391F0(BgConfig *bgConfig, u32 param1, u32 param2);

extern const GraphicsModes ov70_022456F0;

extern const BgTemplate ov70_02245768;

extern const BgTemplate ov70_02245730;

extern const BgTemplate ov70_0224574C;

extern const BgTemplate ov70_02245714;

void ov70_0223CE44(BgConfig *param0) {
    {
        GraphicsModes v0 = ov70_022456F0;

        SetBothScreensModesAndDisable(&v0);
        GfGfx_EngineBTogglePlanes(22, 1);
    }

    {
        BgTemplate v1 = ov70_02245768;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
    }

    {
        BgTemplate v2 = ov70_02245730;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2, 0);
    }

    {
        BgTemplate v3 = ov70_0224574C;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v3, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v4 = ov70_02245714;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v4, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
    }

    ov70_022391F0(param0, 0, 0);
    BG_ClearCharDataRange(0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(3, 0x20, 0, HEAP_ID_61);
    GfGfx_EngineATogglePlanes(16, 1);
    GfGfx_EngineBTogglePlanes(16, GF_BG_LYR_MAIN_1);
}
