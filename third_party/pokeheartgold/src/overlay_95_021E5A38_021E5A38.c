#include "global.h"

struct BgConfig;

struct BgTemplate;

struct GXBanks;

struct GraphicsModes;

struct Window;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

typedef struct BgTemplate {
    u32 x;
    u32 y;
    u32 bufferSize;
    u32 baseTile;
    u8 size;
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 bgExtPltt;
    u8 priority;
    u8 areaOver;
    u8 dummy;
    u32 mosaic;
} BgTemplate;

typedef struct BgConfig BgConfig;

typedef struct Window Window;

typedef struct GraphicsModes {
    GXDispMode displayMode;
    GXBGMode mainBgMode;
    GXBGMode subBgMode;
    GXBG0As bg0As2DOr3D;
} GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct GXBanks {
    GXVRamBG unk_00;
    GXVRamBGExtPltt unk_04;
    GXVRamSubBG unk_08;
    GXVRamSubBGExtPltt unk_0C;
    GXVRamOBJ unk_10;
    GXVRamOBJExtPltt unk_14;
    GXVRamSubOBJ unk_18;
    GXVRamSubOBJExtPltt unk_1C;
    GXVRamTex unk_20;
    GXVRamTexPltt unk_24;
} GXBanks;

void GfGfx_SetBanks(const GXBanks *banks);

void GfGfx_DisableEngineAPlanes(BgConfig *);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_BothDispOn(void);

void ov95_021E5C44(BgConfig *bgConfig, Window *window, enum BgLayer bgLayer, int tilemapTop, int tilemapLeft, int width, int height, int baseTile, int palette);

void ov95_021E5A38(BgConfig *bgConfig);

extern const GXBanks ov95_021E75B4;

extern const GraphicsModes ov95_021E7538;

typedef struct {
    BgTemplate a[3];
} ov95_021E767C_Array;

extern const ov95_021E767C_Array ov95_021E767C;

void ov95_021E5A38(BgConfig *bgConfig) {
    GfGfx_DisableEngineAPlanes(bgConfig);

    GXBanks gxBanks = ov95_021E75B4;

    GfGfx_SetBanks(&gxBanks);

    MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);

    GraphicsModes grapicsModes = ov95_021E7538;

    SetBothScreensModesAndDisable(&grapicsModes);

    ov95_021E767C_Array bgTemplates = ov95_021E767C;

    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_1, &bgTemplates.a[0], BG_TYPE_STATIC);
    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_2, &bgTemplates.a[1], BG_TYPE_STATIC);
    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_3, &bgTemplates.a[2], BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_3);

    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);

    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}
