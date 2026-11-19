#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void ToggleBgLayer(u8 bgLayer, u8 enable);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void GfGfx_SetBanks(const GraphicsBanks *banks);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void ov93_0225D1D8(BgConfig *param0);

extern const GraphicsBanks ov93_02262AC8;

extern const GraphicsModes ov93_02262A44;

typedef struct {
    BgTemplate a[3];
} ov93_02262B40_Array;

extern const ov93_02262B40_Array ov93_02262B40;

void ov93_0225D1D8(BgConfig *param0) {
    GfGfx_DisableEngineAPlanes();

    {
        GraphicsBanks v0 = ov93_02262AC8;

        GfGfx_SetBanks(&v0);

        MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    }

    {
        GraphicsModes v1 = ov93_02262A44;

        SetBothScreensModesAndDisable(&v1);
    }

    {
        ov93_02262B40_Array v2 = ov93_02262B40;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2.a[0], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v2.a[1], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v2.a[2], 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);

        G2_SetBG0Priority(2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    }

    {
        int v3;
        static const BgTemplate v4[] = {
            {
             .x = 0,
             .y = 0,
             .bufferSize = 0x800,
             .baseTile = 0,
             .size = BG_SCREEN_SIZE_256x256,
             .colorMode = GX_BG_COLORMODE_16,
             .screenBase = GX_BG_SCRBASE_0x0000,
             .charBase = GX_BG_CHARBASE_0x08000,
             .bgExtPltt = GX_BG_EXTPLTT_01,
             .priority = 1,
             .areaOver = 0,
             .mosaic = FALSE,
             },
            {
             .x = 0,
             .y = 0,
             .bufferSize = 0x800,
             .baseTile = 0,
             .size = BG_SCREEN_SIZE_256x256,
             .colorMode = GX_BG_COLORMODE_16,
             .screenBase = GX_BG_SCRBASE_0x2000,
             .charBase = GX_BG_CHARBASE_0x10000,
             .bgExtPltt = GX_BG_EXTPLTT_01,
             .priority = 3,
             .areaOver = 0,
             .mosaic = FALSE,
             },
            {
             .x = 0,
             .y = 0,
             .bufferSize = 0x1000,
             .baseTile = 0,
             .size = BG_SCREEN_SIZE_256x256,
             .colorMode = GX_BG_COLORMODE_16,
             .screenBase = GX_BG_SCRBASE_0x4000,
             .charBase = GX_BG_CHARBASE_0x14000,
             .bgExtPltt = GX_BG_EXTPLTT_01,
             .priority = 3,
             .areaOver = 0,
             .mosaic = FALSE,
             },
            {
             .x = 0,
             .y = 0,
             .bufferSize = 0x400,
             .baseTile = 0,
             .size = BG_SCREEN_SIZE_256x256,
             .colorMode = GX_BG_COLORMODE_256,
             .screenBase = GX_BG_SCRBASE_0x6000,
             .charBase = GX_BG_CHARBASE_0x18000,
             .bgExtPltt = GX_BG_EXTPLTT_01,
             .priority = 0,
             .areaOver = 0,
             .mosaic = FALSE,
             },
        };

        for (v3 = 0; v3 < NELEMS(v4); v3++) {
            if (v3 < 3) {
                InitBgFromTemplate(param0, GF_BG_LYR_SUB_0 + v3, &v4[v3], 0);
            } else {
                InitBgFromTemplate(param0, GF_BG_LYR_SUB_0 + v3, &v4[v3], 1);
            }

            BG_ClearCharDataRange(4 + v3, 0x20, 0, HEAP_ID_117);
            BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0 + v3);
            BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_0 + v3, BG_POS_OP_SET_X, 0);
            BgSetPosTextAndCommit(param0, GF_BG_LYR_SUB_0 + v3, BG_POS_OP_SET_Y, 0);
        }

        ToggleBgLayer(GF_BG_LYR_SUB_3, 0);
    }
}
