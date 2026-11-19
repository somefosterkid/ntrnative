#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov40_0222BC44(u8 param0);

void GfGfx_SetBanks(const GraphicsBanks *banks);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

extern const BgTemplate ov40_02244CC8[4];

extern const BgTemplate ov40_02244D38[4];

void ov40_0222BA90(BgConfig *param0);

extern const GraphicsModes _02244C44;

extern const GraphicsBanks ov40_02244CA0;

void ov40_0222BA90(BgConfig *param0) {
    GfGfx_DisableEngineAPlanes();

    {
        GraphicsModes v0 = _02244C44;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        GraphicsBanks v1 = ov40_02244CA0;

        GfGfx_SetBanks(&v1);

        MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    }

    {
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &ov40_02244CC8[0], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &ov40_02244CC8[1], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &ov40_02244CC8[2], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &ov40_02244CC8[3], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
    }

    {
        InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &ov40_02244D38[0], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &ov40_02244D38[1], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_SUB_2, &ov40_02244D38[2], 0);
        InitBgFromTemplate(param0, GF_BG_LYR_SUB_3, &ov40_02244D38[3], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_3);
    }

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);

    ov40_0222BC44(1);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
}
