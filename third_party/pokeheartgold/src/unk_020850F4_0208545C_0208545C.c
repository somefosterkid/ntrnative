#include "global.h"
#include "screen_fade_internal.h"
#include "sys_task.h"

struct BgConfig;

struct BgTemplate;

struct GXBanks;

struct SysTask;

struct SysTaskQueue;

struct System;

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

void GfGfx_DisableEngineAPlanes(BgConfig *, void *);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void GXLayers_SwapDisplay(void);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;

extern System gSystem;

void sub_0208545C(BgConfig *param0, void *arg1);

void GfGfx_SwapDisplay(void);

extern const GXBanks _02102630;

typedef struct {
    BgTemplate a[3];
} _02102658_Array;

extern const _02102658_Array _02102658;

typedef struct {
    BgTemplate a[4];
} _021026AC_Array;

extern const _021026AC_Array _021026AC;

void sub_0208545C(BgConfig *param0, void *arg1) {
    GfGfx_DisableEngineAPlanes(param0, arg1);

    {
        GXBanks v0 = _02102630;

        GfGfx_SetBanks(&v0);

        MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
        MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
        MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    }

    {
        _02102658_Array v1 = _02102658;

        InitBgFromTemplate(param0, BG_LAYER_MAIN_1, &v1.a[0], 0);
        InitBgFromTemplate(param0, BG_LAYER_MAIN_2, &v1.a[1], 0);
        InitBgFromTemplate(param0, BG_LAYER_MAIN_3, &v1.a[2], 0);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_MAIN_1);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_MAIN_2);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_MAIN_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
    }

    {
        _021026AC_Array v2 = _021026AC;

        InitBgFromTemplate(param0, BG_LAYER_SUB_0, &v2.a[0], 0);
        InitBgFromTemplate(param0, BG_LAYER_SUB_1, &v2.a[1], 0);
        InitBgFromTemplate(param0, BG_LAYER_SUB_2, &v2.a[2], 0);
        InitBgFromTemplate(param0, BG_LAYER_SUB_3, &v2.a[3], 0);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_SUB_0);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_SUB_1);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_SUB_2);
        BgClearTilemapBufferAndCommit(param0, BG_LAYER_SUB_3);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 0);
    }

    gSystem.screensFlipped = DS_SCREEN_SUB;

    GfGfx_SwapDisplay();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
}
