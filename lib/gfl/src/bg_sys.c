// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/bg_sys.c. The BG system of both engines,
// named by the "bg_sys.c" the ROM passes to the heap. pokebw2's guesses GFL_BGSysIsPixelOfColor, GFL_BGSysSetScrTile,
// GFL_BGSysFlipTile and sBGSys keep their dsd names here.
#include "types.h"
#include "gfl/areaman.h"
#include "gfl/bg_sys.h"
#include "gfl/calctool.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// A BG of either engine
struct BGSysBG {
    // The screen buffer, which is copied to VRAM at screenOffset map entries, or NULL
    void *screen;
    u32 screenSize;
    u32 screenOffset;
    int x;
    int y;
    u8 mode;
    u8 resolution;
    u8 colorMode;
    u8 tileSize;
    // The affine transform of an affine or extended BG
    u16 rotation;
    fx32 scaleX;
    fx32 scaleY;
    int centerX;
    int centerY;
};

struct BGSys {
    HeapID heapId;
    u16 unused;
    // The BGs, as bits, whose transform or screen buffer is sent at the next update
    u16 transformRequests;
    u16 screenRequests;
    BGSysBG bgs[BGSYS_BG_COUNT];
    // The characters allocated in each engine's BG VRAM, in blocks of 0x20 bytes, which hold the screens too
    AreaMan *charAreas[2];
    // Where each BG's characters and screen are in VRAM, in bytes, and the size of its characters
    u32 charBase[BGSYS_BG_COUNT];
    u32 screenBase[BGSYS_BG_COUNT];
    u32 charSize[BGSYS_BG_COUNT];
};

static u8 GFL_BGSysCalcScreenSizeMode(u8 resolution, u8 mode);
static void GFL_BGSysGetTileDimensions(u8 resolution, u8 *width, u8 *height);
static void GFL_BGSysAdjustBGMoveCoord(BGSysBG *bg, u32 op, int value);
static void GFL_BGSysSetBGTransformIdentity(u8 bg);
static void GFL_BGSysTransferUncomp(const void *src, void *dest, u32 size);
static void GFL_BGSysUploadScr(u8 bg, const void *src, u32 offset, u32 size);
static void GFL_BGSysLoadCharCore(u8 bg, const void *src, u32 size, u32 offset);
static void GFL_BGSysUploadChar(u8 bg, const void *src, u32 offset, u32 size);
static u16 GFL_BGSysConvCoordsToTileIndex(u8 x, u8 y, u8 resolution);
static u16 GFL_BGSysConvCoordsToTileIndexEx(u8 x, u8 y, u8 width, u8 height);
static void GFL_BGSysLoadScrArea_TEXT(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u16 *src, u8 srcX, u8 srcY,
                                      u8 srcWidth, u8 srcHeight, u8 large);
static void GFL_BGSysLoadScrArea_AFFINE(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u8 *src, u8 srcX, u8 srcY,
                                        u8 srcWidth, u8 srcHeight, u8 large);
static void GFL_BGSysLoadScrArea_EXTENDED(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u16 *src, u8 srcX,
                                          u8 srcY, u8 srcWidth, u8 srcHeight, u8 large);
static void GFL_BGSysFillScrArea_TEXT(BGSysBG *bg, u16 tile, u8 x, u8 y, u8 width, u8 height, u8 palette);
static void GFL_BGSysFillScrArea_AFFINE(BGSysBG *bg, u8 tile, u8 x, u8 y, u8 width, u8 height);
static void GFL_BGSysFillScrArea_EXTENDED(BGSysBG *bg, u16 tile, u8 x, u8 y, u8 width, u8 height, u8 palette);
static void GFL_BGSysAllocScreen(u8 bg, u32 offset, u32 size);
static void GFL_BGSysFreeScreenMemory(u8 bg, u32 offset, u32 size);
static void func_02043130(u8 flip, u8 *tile, u32 heapId);
static void GFL_BGSysAdjustBGRotateCoord(BGSysBG *bg, u32 op, u16 value);
static void GFL_BGSysAdjustBGScaleCoord(BGSysBG *bg, u32 op, fx32 value);
static void GFL_BGSysAdjustBGOriginCoord(BGSysBG *bg, u32 op, int value);
static void GFL_BGSysFlushTransform(void);
static void GFL_BGSysLoadQueuedScreens(void);

static BGSys *data_02146a44;

void GFL_BGSysCreate(HeapID heapId) {
    int i;

    data_02146a44 = GFL_HeapAllocate(heapId, sizeof(BGSys), FALSE, "bg_sys.c", 134);
    MI_CpuFill8(data_02146a44, 0, sizeof(BGSys));
    data_02146a44->heapId = heapId;
    data_02146a44->transformRequests = 0;
    data_02146a44->screenRequests = 0;
    data_02146a44->charAreas[BGSYS_ENGINE_MAIN] = GFL_AreaManCreate(0x1000, heapId);
    data_02146a44->charAreas[BGSYS_ENGINE_SUB] = GFL_AreaManCreate(0x1000, heapId);
    for (i = 0; i < BGSYS_BG_COUNT; i++) {
        data_02146a44->charBase[i] = 0;
        data_02146a44->screenBase[i] = 0;
        data_02146a44->charSize[i] = 0;
    }
}

void GFL_BGSysFree(void) {
    u8 i;

    for (i = 0; i < BGSYS_BG_COUNT; i++) {
        GFL_BGSysReleaseBG(i);
    }
    GFL_AreaManFree(data_02146a44->charAreas[BGSYS_ENGINE_MAIN]);
    GFL_AreaManFree(data_02146a44->charAreas[BGSYS_ENGINE_SUB]);
    GFL_HeapFree(data_02146a44);
    data_02146a44 = NULL;
}

u32 GFL_BGSysAllocChar(u32 bg, u32 size, u32 fromEnd) {
    int tileSize;
    u32 start;
    u32 count;
    u32 partial;
    u32 tiles;
    AreaMan *area;
    u32 pos;

    tileSize = GFL_BGSysGetBGBytesPerTile(bg);
    start = data_02146a44->charBase[bg] / tileSize;
    count = data_02146a44->charSize[bg] / tileSize;
    partial = (size % tileSize) != 0 ? 1 : 0;
    tiles = size / tileSize;
    area = bg < BGSYS_BG_SUB ? data_02146a44->charAreas[BGSYS_ENGINE_MAIN] : data_02146a44->charAreas[BGSYS_ENGINE_SUB];
    if (fromEnd == FALSE) {
        pos = GFL_AreaManAllocHead(area, start, count, tiles + partial);
    } else {
        pos = GFL_AreaManAllocTail(area, start + count - 1, count, tiles + partial);
    }
    if (pos != AREAMAN_FAIL) {
        return (pos * tileSize - data_02146a44->charBase[bg]) / tileSize;
    }
    return AREAMAN_FAIL;
}

BOOL GFL_BGSysAllocCharAt(u32 bg, u32 pos, u32 size) {
    u32 tileSize = GFL_BGSysGetBGBytesPerTile(bg);
    u32 start = data_02146a44->charBase[bg] / tileSize;
    AreaMan *area = bg < BGSYS_BG_SUB ? data_02146a44->charAreas[BGSYS_ENGINE_MAIN] : data_02146a44->charAreas[BGSYS_ENGINE_SUB];

    return GFL_AreaManSetBits(area, start + pos, size);
}

void GFL_BGSysFreeCharMemory(u32 bg, u32 pos, u32 size) {
    u32 tileSize = GFL_BGSysGetBGBytesPerTile(bg);

    pos = (data_02146a44->charBase[bg] + pos * tileSize) / tileSize;
    switch (bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_MAIN], pos, size / tileSize + (size % tileSize != 0));
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_SUB], pos, size / tileSize + (size % tileSize != 0));
        break;
    }
}

void GFL_BGSysSetLCDConfig(const BGSysLCDConfig *config) {
    GX_SetGraphicsMode(config->displayMode, config->bgModeMain, config->bg0Is3D);
    GXS_SetGraphicsMode(config->bgModeSub);
    GX_SetBGScrOffset(GX_BGSCROFFSET_0x00000);
    GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);
    GFL_BGSysDisableBGsA();
    GFL_BGSysDisableBGsB();
}

void GFL_BGSysSetLCDConfigForEngine(const BGSysLCDConfig *config, u32 engine) {
    if (engine == BGSYS_ENGINE_MAIN) {
        GX_SetGraphicsMode(config->displayMode, config->bgModeMain, config->bg0Is3D);
        GFL_BGSysDisableAllA();
    } else {
        GXS_SetGraphicsMode(config->bgModeSub);
        GFL_BGSysDisableAllB();
    }
}

void GFL_BGSysCreateBG(u8 bg, const BGSetup *setup, u8 mode) {
    u8 screenSize = GFL_BGSysCalcScreenSizeMode(setup->resolution, mode);

    switch (bg) {
    case 0:
        G2_SetBG0Control(screenSize, setup->colorMode, setup->screenBase, setup->charBase, setup->extPaletteSlot);
        G2_SetBG0Priority(setup->priority);
        G2_BG0Mosaic(setup->mosaic);
        break;
    case 1:
        G2_SetBG1Control(screenSize, setup->colorMode, setup->screenBase, setup->charBase, setup->extPaletteSlot);
        G2_SetBG1Priority(setup->priority);
        G2_BG1Mosaic(setup->mosaic);
        break;
    case 2:
        switch (mode) {
        default:
        case BGMODE_TEXT:
            G2_SetBG2ControlText(screenSize, setup->colorMode, setup->screenBase, setup->charBase);
            break;
        case BGMODE_AFFINE:
            G2_SetBG2ControlAffine(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        case BGMODE_EXTENDED:
            G2_SetBG2Control256x16Pltt(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        }
        G2_SetBG2Priority(setup->priority);
        G2_BG2Mosaic(setup->mosaic);
        break;
    case 3:
        switch (mode) {
        default:
        case BGMODE_TEXT:
            G2_SetBG3ControlText(screenSize, setup->colorMode, setup->screenBase, setup->charBase);
            break;
        case BGMODE_AFFINE:
            G2_SetBG3ControlAffine(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        case BGMODE_EXTENDED:
            G2_SetBG3Control256x16Pltt(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        }
        G2_SetBG3Priority(setup->priority);
        G2_BG3Mosaic(setup->mosaic);
        break;
    case 4:
        G2S_SetBG0Control(screenSize, setup->colorMode, setup->screenBase, setup->charBase, setup->extPaletteSlot);
        G2S_SetBG0Priority(setup->priority);
        G2S_BG0Mosaic(setup->mosaic);
        break;
    case 5:
        G2S_SetBG1Control(screenSize, setup->colorMode, setup->screenBase, setup->charBase, setup->extPaletteSlot);
        G2S_SetBG1Priority(setup->priority);
        G2S_BG1Mosaic(setup->mosaic);
        break;
    case 6:
        switch (mode) {
        default:
        case BGMODE_TEXT:
            G2S_SetBG2ControlText(screenSize, setup->colorMode, setup->screenBase, setup->charBase);
            break;
        case BGMODE_AFFINE:
            G2S_SetBG2ControlAffine(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        case BGMODE_EXTENDED:
            G2S_SetBG2Control256x16Pltt(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        }
        G2S_SetBG2Priority(setup->priority);
        G2S_BG2Mosaic(setup->mosaic);
        break;
    case 7:
        switch (mode) {
        default:
        case BGMODE_TEXT:
            G2S_SetBG3ControlText(screenSize, setup->colorMode, setup->screenBase, setup->charBase);
            break;
        case BGMODE_AFFINE:
            G2S_SetBG3ControlAffine(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        case BGMODE_EXTENDED:
            G2S_SetBG3Control256x16Pltt(screenSize, setup->areaOverflow, setup->screenBase, setup->charBase);
            break;
        }
        G2S_SetBG3Priority(setup->priority);
        G2S_BG3Mosaic(setup->mosaic);
        break;
    }

    data_02146a44->bgs[bg].rotation = 0;
    data_02146a44->bgs[bg].scaleX = FX32_ONE;
    data_02146a44->bgs[bg].scaleY = FX32_ONE;
    data_02146a44->bgs[bg].centerX = 0;
    data_02146a44->bgs[bg].centerY = 0;

    if (setup->screenSize != 0) {
        data_02146a44->bgs[bg].screen = GFL_HeapAllocate(data_02146a44->heapId, setup->screenSize, FALSE, "bg_sys.c", 561);
        MIi_CpuClear16(0, data_02146a44->bgs[bg].screen, setup->screenSize);
        data_02146a44->bgs[bg].screenSize = setup->screenSize;
        data_02146a44->bgs[bg].screenOffset = setup->screenOffset;
    } else {
        data_02146a44->bgs[bg].screen = NULL;
        data_02146a44->bgs[bg].screenSize = 0;
        data_02146a44->bgs[bg].screenOffset = 0;
    }

    data_02146a44->bgs[bg].resolution = setup->resolution;
    data_02146a44->bgs[bg].mode = mode;
    data_02146a44->bgs[bg].colorMode = setup->colorMode;
    if (mode == BGMODE_TEXT && setup->colorMode == GX_BG_COLORMODE_16) {
        data_02146a44->bgs[bg].tileSize = BGSYS_TILE_SIZE_16;
    } else {
        data_02146a44->bgs[bg].tileSize = BGSYS_TILE_SIZE_256;
    }

    GFL_BGSysMoveBG(bg, BG_MOVE_SET_X, setup->x);
    GFL_BGSysMoveBG(bg, BG_MOVE_SET_Y, setup->y);
    GFL_BGSysSetBGTransformIdentity(bg);

    data_02146a44->charBase[bg] = setup->charBase * 0x4000;
    data_02146a44->screenBase[bg] = setup->screenBase * 0x800;
    data_02146a44->charSize[bg] = setup->charSize;
    GFL_BGSysAllocScreen(bg, 0, setup->screenSize);
}

void GFL_BGSysReleaseBG(u8 bg) {
    if (data_02146a44->bgs[bg].screen != NULL) {
        GFL_BGSysFreeScreenMemory(bg, 0, data_02146a44->bgs[bg].screenSize);
        GFL_HeapFree(data_02146a44->bgs[bg].screen);
        data_02146a44->bgs[bg].screen = NULL;
    }
}

void GFL_BGSysSet3DBGPriority(u16 priority) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(priority);
}

void GFL_BGSysSetBGPriority(u32 bg, u32 priority) {
    switch (bg) {
    case 0:
        G2_SetBG0Priority(priority);
        break;
    case 1:
        G2_SetBG1Priority(priority);
        break;
    case 2:
        G2_SetBG2Priority(priority);
        break;
    case 3:
        G2_SetBG3Priority(priority);
        break;
    case 4:
        G2S_SetBG0Priority(priority);
        break;
    case 5:
        G2S_SetBG1Priority(priority);
        break;
    case 6:
        G2S_SetBG2Priority(priority);
        break;
    case 7:
        G2S_SetBG3Priority(priority);
        break;
    }
}

void GFL_BGSysSetBGEnabled(u8 bg, u8 enabled) {
    switch (bg) {
    case 0:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, enabled);
        break;
    case 1:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, enabled);
        break;
    case 2:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, enabled);
        break;
    case 3:
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, enabled);
        break;
    case 4:
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0, enabled);
        break;
    case 5:
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, enabled);
        break;
    case 6:
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG2, enabled);
        break;
    case 7:
        GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, enabled);
        break;
    }
}

void GFL_BGSysMoveBG(u8 bg, u8 op, int value) {
    int x;
    int y;

    GFL_BGSysAdjustBGMoveCoord(&data_02146a44->bgs[bg], op, value);
    x = data_02146a44->bgs[bg].x;
    y = data_02146a44->bgs[bg].y;

    switch (bg) {
    case 0:
        G2_SetBG0Offset(x, y);
        break;
    case 1:
        G2_SetBG1Offset(x, y);
        func_0203d814(x, y);
        break;
    case 2:
        if (data_02146a44->bgs[2].mode == BGMODE_TEXT) {
            G2_SetBG2Offset(x, y);
        } else {
            GFL_BGSysSetBGTransformIdentity(2);
        }
        break;
    case 3:
        if (data_02146a44->bgs[3].mode == BGMODE_TEXT) {
            G2_SetBG3Offset(x, y);
        } else {
            GFL_BGSysSetBGTransformIdentity(3);
        }
        break;
    case 4:
        G2S_SetBG0Offset(x, y);
        break;
    case 5:
        G2S_SetBG1Offset(x, y);
        break;
    case 6:
        if (data_02146a44->bgs[6].mode == BGMODE_TEXT) {
            G2S_SetBG2Offset(x, y);
        } else {
            GFL_BGSysSetBGTransformIdentity(6);
        }
        break;
    case 7:
        if (data_02146a44->bgs[7].mode == BGMODE_TEXT) {
            G2S_SetBG3Offset(x, y);
        } else {
            GFL_BGSysSetBGTransformIdentity(7);
        }
        break;
    }
}

int GFL_BGSysGetBGOffsetX(u8 bg) {
    return data_02146a44->bgs[bg].x;
}

int GFL_BGSysGetBGOffsetY(u16 bg) {
    return data_02146a44->bgs[bg].y;
}

void GFL_BGSysSetBGTransformEx(u8 bg, u32 op, int value, const MtxFx22 *mtx, int centerX, int centerY) {
    GFL_BGSysAdjustBGMoveCoord(&data_02146a44->bgs[bg], op, value);
    GFL_BGSysSetBGTransform(bg, mtx, centerX, centerY);
}

void GFL_BGSysSetBGTransform(u8 bg, const MtxFx22 *mtx, int centerX, int centerY) {
    switch (bg) {
    case 2:
        G2_SetBG2Affine(mtx, centerX, centerY, data_02146a44->bgs[bg].x, data_02146a44->bgs[bg].y);
        break;
    case 3:
        G2_SetBG3Affine(mtx, centerX, centerY, data_02146a44->bgs[bg].x, data_02146a44->bgs[bg].y);
        break;
    case 6:
        G2S_SetBG2Affine(mtx, centerX, centerY, data_02146a44->bgs[bg].x, data_02146a44->bgs[bg].y);
        break;
    case 7:
        G2S_SetBG3Affine(mtx, centerX, centerY, data_02146a44->bgs[bg].x, data_02146a44->bgs[bg].y);
        break;
    }
}

static void GFL_BGSysTransferUncomp(const void *src, void *dest, u32 size) {
    if (size == 0) {
        MI_UncompressLZ8(src, dest);
        return;
    }
    if ((uintptr_t)src % 4 == 0 && (uintptr_t)dest % 4 == 0 && (u16)size % 4 == 0) {
        MIi_CpuCopy32(src, dest, size);
        return;
    }
    MIi_CpuCopy16(src, dest, size);
}

void GFL_BGSysLoadScr(u8 bg) {
    GFL_BGSysLoadScrCore(bg, data_02146a44->bgs[bg].screen, data_02146a44->bgs[bg].screenSize, data_02146a44->bgs[bg].screenOffset);
}

void GFL_BGSysLoadScrCore(u8 bg, const void *src, u32 size, u32 offset) {
    if (size == 0) {
        if (data_02146a44->bgs[bg].screen != NULL) {
            void *screen = data_02146a44->bgs[bg].screen;

            GFL_BGSysTransferUncomp(src, screen, size);
            GFL_BGSysUploadScr(bg, screen, data_02146a44->bgs[bg].screenOffset * 2, data_02146a44->bgs[bg].screenSize);
            return;
        } else {
            u32 uncompSize = MI_GetUncompressedSize(src);
            void *buf = GFL_HeapAllocate(HEAPID_TAIL(data_02146a44->heapId), uncompSize, FALSE, "bg_sys.c", 1252);

            GFL_BGSysTransferUncomp(src, buf, size);
            GFL_BGSysUploadScr(bg, buf, offset * 2, uncompSize);
            GFL_HeapFree(buf);
            return;
        }
    }
    GFL_BGSysUploadScr(bg, src, offset * 2, size);
}

void GFL_BGSysBufferScrDefault(u8 bg, const void *src, u32 size) {
    GFL_BGSysBufferScr(bg, src, size, 0);
}

void GFL_BGSysBufferScr(u8 bg, const void *src, u32 size, u32 offset) {
    GFL_BGSysTransferUncomp(src, (u16 *)data_02146a44->bgs[bg].screen + offset, size);
}

void GFL_BGSysLoadChar(u8 bg, const void *src, u32 size, u32 offset) {
    if (data_02146a44->bgs[bg].colorMode == GX_BG_COLORMODE_16) {
        GFL_BGSysLoadCharCore(bg, src, size, offset * BGSYS_TILE_SIZE_16);
    } else {
        GFL_BGSysLoadCharCore(bg, src, size, offset * BGSYS_TILE_SIZE_256);
    }
}

u32 GFL_BGSysLoadCharDynamic(u32 bg, const void *src, u32 size) {
    u32 pos = GFL_BGSysAllocChar(bg, size, FALSE);

    GFL_BGSysLoadCharCore(bg, src, size, pos * BGSYS_TILE_SIZE_16);
    return pos;
}

void GFL_BGSysClearCharCore(u8 bg, u32 size, u32 offset, HeapID heapId) {
    void *buf = GFL_HeapAllocate(HEAPID_TAIL(heapId), size, FALSE, "bg_sys.c", 1362);

    MI_CpuFill8(buf, 0, size);
    GFL_BGSysUploadChar(bg, buf, offset, size);
    GFL_HeapFree(buf);
}

void GFL_BGSysFillChar(u32 bg, u32 fillIndex, u32 tileCount, u32 offset) {
    void *buf;
    u32 size;

    switch (bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (data_02146a44->bgs[bg].tileSize == BGSYS_TILE_SIZE_16) {
            GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_MAIN],
                               (data_02146a44->charBase[bg] + offset * data_02146a44->bgs[bg].tileSize) / 0x20, tileCount);
        } else {
            GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_MAIN],
                               (data_02146a44->charBase[bg] + offset * data_02146a44->bgs[bg].tileSize) / 0x20, tileCount * 2);
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (data_02146a44->bgs[bg].tileSize == BGSYS_TILE_SIZE_16) {
            GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_SUB],
                               (data_02146a44->charBase[bg] + offset * data_02146a44->bgs[bg].tileSize) / 0x20, tileCount);
        } else {
            GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_SUB],
                               (data_02146a44->charBase[bg] + offset * data_02146a44->bgs[bg].tileSize) / 0x20, tileCount * 2);
        }
        break;
    }

    size = data_02146a44->bgs[bg].tileSize * tileCount;
    buf = GFL_HeapAllocate(HEAPID_TAIL(data_02146a44->heapId), size, FALSE, "bg_sys.c", 1421);
    if (data_02146a44->bgs[bg].tileSize == BGSYS_TILE_SIZE_16) {
        fillIndex = (fillIndex << 12) | (fillIndex << 8) | (fillIndex << 4) | fillIndex;
        fillIndex |= fillIndex << 16;
    } else {
        fillIndex = (fillIndex << 24) | (fillIndex << 16) | (fillIndex << 8) | fillIndex;
    }
    MIi_CpuClearFast(fillIndex, buf, size);
    GFL_BGSysUploadChar(bg, buf, offset * data_02146a44->bgs[bg].tileSize, size);
    GFL_HeapFree(buf);
}

void GFL_BGSysFreeFilledChar(u32 bg, u32 tileCount, u32 offset) {
    switch (bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (data_02146a44->bgs[bg].tileSize == BGSYS_TILE_SIZE_16) {
            GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_MAIN],
                               (data_02146a44->charBase[bg] + data_02146a44->bgs[bg].tileSize * offset) / 0x20, tileCount);
        } else {
            GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_MAIN],
                               (data_02146a44->charBase[bg] + data_02146a44->bgs[bg].tileSize * offset) / 0x20, tileCount * 2);
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (data_02146a44->bgs[bg].tileSize == BGSYS_TILE_SIZE_16) {
            GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_SUB],
                               (data_02146a44->charBase[bg] + data_02146a44->bgs[bg].tileSize * offset) / 0x20, tileCount);
        } else {
            GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_SUB],
                               (data_02146a44->charBase[bg] + data_02146a44->bgs[bg].tileSize * offset) / 0x20, tileCount * 2);
        }
        break;
    }
}

void GFL_BGSysUploadStdPalette(u8 bg, const void *src, u16 size, u16 offset) {
    DC_FlushRange(src, size);
    if (bg < BGSYS_BG_SUB) {
        GX_LoadBGPltt(src, offset, size);
    } else {
        GXS_LoadBGPltt(src, offset, size);
    }
}

void GFL_BGSysResetStdPalette(u8 bg, GXRgb color) {
    GFL_BGSysUploadStdPalette(bg, &color, sizeof(GXRgb), 0);
}

static u16 GFL_BGSysConvCoordsToTileIndex(u8 x, u8 y, u8 resolution) {
    u16 index;

    switch (resolution) {
    case BGRES_128x128:
        index = y * 16 + x;
        break;
    case BGRES_256x256:
    case BGRES_256x512:
        index = y * 32 + x;
        break;
    case BGRES_512x256:
        index = ((x >> 5) * 32 + y) * 32 + (x & 0x1f);
        break;
    case BGRES_512x512:
        index = (x >> 5) + (y >> 5) * 2;
        index *= 1024;
        index += (y & 0x1f) * 32 + (x & 0x1f);
        break;
    case BGRES_1024x1024:
        index = 0;
    }
    return index;
}

static u16 GFL_BGSysConvCoordsToTileIndexEx(u8 x, u8 y, u8 width, u8 height) {
    u8 block = 0;
    u16 index = 0;
    s16 widthPast32 = width - 32;
    s16 heightPast32 = height - 32;

    if (x / 32) {
        block += 1;
    }
    if (y / 32) {
        block += 2;
    }

    switch (block) {
    case 0:
        if (widthPast32 >= 0) {
            index += y * 32 + x;
        } else {
            index += y * width + x;
        }
        break;
    case 1:
        if (heightPast32 >= 0) {
            index += 1024;
        } else {
            index += 32 * height;
        }
        index += y * widthPast32 + (x & 0x1f);
        break;
    case 2:
        index += width * 32;
        if (widthPast32 >= 0) {
            index += (y & 0x1f) * 32 + x;
        } else {
            index += (y & 0x1f) * width + x;
        }
        break;
    case 3:
        index += width * 32 + heightPast32 * 32;
        index += (y & 0x1f) * widthPast32 + (x & 0x1f);
        break;
    }
    return index;
}

void GFL_BGSysLoadScrAreaAll(u8 bg, const void *src, u8 x, u8 y, u8 width, u8 height) {
    GFL_BGSysLoadScrArea(bg, x, y, width, height, src, 0, 0, width, height);
}

void GFL_BGSysLoadScrArea(u8 bg, u8 x, u8 y, u8 width, u8 height, const void *src, u8 srcX, u8 srcY, u8 srcWidth,
                          u8 srcHeight) {
    if (data_02146a44->bgs[bg].mode == BGMODE_TEXT) {
        GFL_BGSysLoadScrArea_TEXT(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight, FALSE);
    } else if (data_02146a44->bgs[bg].mode == BGMODE_AFFINE) {
        GFL_BGSysLoadScrArea_AFFINE(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight, FALSE);
    } else {
        GFL_BGSysLoadScrArea_EXTENDED(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight,
                                      FALSE);
    }
}

void GFL_BGSysLoadScrAreaLarge(u8 bg, u8 x, u8 y, u8 width, u8 height, const void *src, u8 srcX, u8 srcY, u8 srcWidth,
                               u8 srcHeight) {
    if (data_02146a44->bgs[bg].mode == BGMODE_TEXT) {
        GFL_BGSysLoadScrArea_TEXT(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight, TRUE);
    } else if (data_02146a44->bgs[bg].mode == BGMODE_AFFINE) {
        GFL_BGSysLoadScrArea_AFFINE(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight, TRUE);
    } else {
        GFL_BGSysLoadScrArea_EXTENDED(&data_02146a44->bgs[bg], x, y, width, height, src, srcX, srcY, srcWidth, srcHeight,
                                      TRUE);
    }
}

void GFL_BGSysFillScrArea(u8 bg, u16 tile, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    if (data_02146a44->bgs[bg].mode == BGMODE_TEXT) {
        GFL_BGSysFillScrArea_TEXT(&data_02146a44->bgs[bg], tile, x, y, width, height, palette);
    } else if (data_02146a44->bgs[bg].mode == BGMODE_AFFINE) {
        GFL_BGSysFillScrArea_AFFINE(&data_02146a44->bgs[bg], tile, x, y, width, height);
    } else {
        GFL_BGSysFillScrArea_EXTENDED(&data_02146a44->bgs[bg], tile, x, y, width, height, palette);
    }
}

void GFL_BGSysSetScrPaletteNo(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    u16 *screen = data_02146a44->bgs[bg].screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;
    u16 index;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(data_02146a44->bgs[bg].resolution, &tilesX, &tilesY);
    for (i = y; i < y + height; i++) {
        if (i >= tilesY) {
            break;
        }
        for (j = x; j < x + width; j++) {
            if (j >= tilesX) {
                break;
            }
            index = GFL_BGSysConvCoordsToTileIndex(j, i, data_02146a44->bgs[bg].resolution);
            screen[index] = (screen[index] & 0xfff) | (palette << 12);
        }
    }
}

void GFL_BGSysClearBG(u8 bg) {
    GFL_BGSysClearChar(bg);
    GFL_BGSysClearScr(bg);
}

void GFL_BGSysClearChar(u8 bg) {
    GFL_BGSysClearCharCore(bg, data_02146a44->charSize[bg], 0, data_02146a44->heapId);
}

void GFL_BGSysClearScr(u8 bg) {
    if (data_02146a44->bgs[bg].screen != NULL) {
        MIi_CpuClear16(0, data_02146a44->bgs[bg].screen, data_02146a44->bgs[bg].screenSize);
        GFL_BGSysLoadScr(bg);
    }
}

void GFL_BGSysFillScr(u8 bg, u16 map) {
    if (data_02146a44->bgs[bg].screen != NULL) {
        MIi_CpuClear16(map, data_02146a44->bgs[bg].screen, data_02146a44->bgs[bg].screenSize);
        GFL_BGSysLoadScr(bg);
    }
}

void GFL_BGSysFillScrAsync(u8 bg, u16 map) {
    if (data_02146a44->bgs[bg].screen != NULL) {
        MIi_CpuClear16(map, data_02146a44->bgs[bg].screen, data_02146a44->bgs[bg].screenSize);
        GFL_BGSysQueueScrLoad(bg);
    }
}

void *GFL_BGSysGetBGCharAddress(u8 bg) {
    switch (bg) {
    case 0:
        return G2_GetBG0CharPtr();
    case 1:
        return G2_GetBG1CharPtr();
    case 2:
        return G2_GetBG2CharPtr();
    case 3:
        return G2_GetBG3CharPtr();
    case 4:
        return G2S_GetBG0CharPtr();
    case 5:
        return G2S_GetBG1CharPtr();
    case 6:
        return G2S_GetBG2CharPtr();
    case 7:
        return G2S_GetBG3CharPtr();
    }
    return NULL;
}

void *GFL_BGSysIsScrHeapExists(u8 bg) {
    return data_02146a44->bgs[bg].screen;
}

int GFL_BGSysGetBGOffsetX2(u8 bg) {
    return data_02146a44->bgs[bg].x;
}

int GFL_BGSysGetBGOffsetY2(u8 bg) {
    return data_02146a44->bgs[bg].y;
}

u8 GFL_BGSysGetBGMode(u8 bg) {
    return data_02146a44->bgs[bg].mode;
}

u8 GFL_BGSysGetBGColorPaletteMode(u8 bg) {
    return data_02146a44->bgs[bg].colorMode;
}

u8 GFL_BGSysGetBGBytesPerTile(u8 bg) {
    return data_02146a44->bgs[bg].tileSize;
}

u8 GFL_BGSysGetBGPriority(u8 bg) {
    switch (bg) {
    case 0: {
        GXBg01Control control = G2_GetBG0Control();
        return control.priority;
    }
    case 1: {
        GXBg01Control control = G2_GetBG1Control();
        return control.priority;
    }
    case 2:
        switch (data_02146a44->bgs[bg].mode) {
        default:
        case BGMODE_TEXT: {
            GXBg23ControlText control = G2_GetBG2ControlText();
            return control.priority;
        }
        case BGMODE_AFFINE: {
            GXBg23ControlAffine control = G2_GetBG2ControlAffine();
            return control.priority;
        }
        case BGMODE_EXTENDED: {
            GXBg23Control256x16Pltt control = G2_GetBG2Control256x16Pltt();
            return control.priority;
        }
        }
    case 3:
        switch (data_02146a44->bgs[bg].mode) {
        default:
        case BGMODE_TEXT: {
            GXBg23ControlText control = G2_GetBG3ControlText();
            return control.priority;
        }
        case BGMODE_AFFINE: {
            GXBg23ControlAffine control = G2_GetBG3ControlAffine();
            return control.priority;
        }
        case BGMODE_EXTENDED: {
            GXBg23Control256x16Pltt control = G2_GetBG3Control256x16Pltt();
            return control.priority;
        }
        }
    case 4: {
        GXBg01Control control = G2S_GetBG0Control();
        return control.priority;
    }
    case 5: {
        GXBg01Control control = G2S_GetBG1Control();
        return control.priority;
    }
    case 6:
        switch (data_02146a44->bgs[bg].mode) {
        default:
        case BGMODE_TEXT: {
            GXBg23ControlText control = G2S_GetBG2ControlText();
            return control.priority;
        }
        case BGMODE_AFFINE: {
            GXBg23ControlAffine control = G2S_GetBG2ControlAffine();
            return control.priority;
        }
        case BGMODE_EXTENDED: {
            GXBg23Control256x16Pltt control = G2S_GetBG2Control256x16Pltt();
            return control.priority;
        }
        }
    case 7:
        switch (data_02146a44->bgs[bg].mode) {
        default:
        case BGMODE_TEXT: {
            GXBg23ControlText control = G2S_GetBG3ControlText();
            return control.priority;
        }
        case BGMODE_AFFINE: {
            GXBg23ControlAffine control = G2S_GetBG3ControlAffine();
            return control.priority;
        }
        case BGMODE_EXTENDED: {
            GXBg23Control256x16Pltt control = G2S_GetBG3Control256x16Pltt();
            return control.priority;
        }
        }
    }
    return 0;
}

void GFL_BGSysUpdate(void) {
    if (data_02146a44 != NULL) {
        GFL_BGSysFlushTransform();
        GFL_BGSysLoadQueuedScreens();
        data_02146a44->transformRequests = 0;
        data_02146a44->screenRequests = 0;
    }
}

static void GFL_BGSysLoadQueuedScreens(void) {
    if ((data_02146a44->screenRequests & (1 << 0)) != 0) {
        GFL_BGSysUploadScr(0, data_02146a44->bgs[0].screen, data_02146a44->bgs[0].screenOffset * 2, data_02146a44->bgs[0].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 1)) != 0) {
        GFL_BGSysUploadScr(1, data_02146a44->bgs[1].screen, data_02146a44->bgs[1].screenOffset * 2, data_02146a44->bgs[1].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 2)) != 0) {
        GFL_BGSysUploadScr(2, data_02146a44->bgs[2].screen, data_02146a44->bgs[2].screenOffset * 2, data_02146a44->bgs[2].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 3)) != 0) {
        GFL_BGSysUploadScr(3, data_02146a44->bgs[3].screen, data_02146a44->bgs[3].screenOffset * 2, data_02146a44->bgs[3].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 4)) != 0) {
        GFL_BGSysUploadScr(4, data_02146a44->bgs[4].screen, data_02146a44->bgs[4].screenOffset * 2, data_02146a44->bgs[4].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 5)) != 0) {
        GFL_BGSysUploadScr(5, data_02146a44->bgs[5].screen, data_02146a44->bgs[5].screenOffset * 2, data_02146a44->bgs[5].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 6)) != 0) {
        GFL_BGSysUploadScr(6, data_02146a44->bgs[6].screen, data_02146a44->bgs[6].screenOffset * 2, data_02146a44->bgs[6].screenSize);
    }
    if ((data_02146a44->screenRequests & (1 << 7)) != 0) {
        GFL_BGSysUploadScr(7, data_02146a44->bgs[7].screen, data_02146a44->bgs[7].screenOffset * 2, data_02146a44->bgs[7].screenSize);
    }
}

void GFL_BGSysQueueScrLoad(u8 bg) {
    data_02146a44->screenRequests |= 1 << bg;
}

static void GFL_BGSysFlushTransform(void) {
    if ((data_02146a44->transformRequests & (1 << 0)) != 0) {
        G2_SetBG0Offset(data_02146a44->bgs[0].x, data_02146a44->bgs[0].y);
    }
    if ((data_02146a44->transformRequests & (1 << 1)) != 0) {
        G2_SetBG1Offset(data_02146a44->bgs[1].x, data_02146a44->bgs[1].y);
        func_0203d814(data_02146a44->bgs[1].x, data_02146a44->bgs[1].y);
    }
    if ((data_02146a44->transformRequests & (1 << 2)) != 0) {
        if (data_02146a44->bgs[2].mode == BGMODE_TEXT) {
            G2_SetBG2Offset(data_02146a44->bgs[2].x, data_02146a44->bgs[2].y);
        } else {
            MtxFx22 mtx;
            MAT2_SetScaleRot(&mtx, data_02146a44->bgs[2].rotation, data_02146a44->bgs[2].scaleX, data_02146a44->bgs[2].scaleY, MAT2_ROT_DEG);
            G2_SetBG2Affine(&mtx, data_02146a44->bgs[2].centerX, data_02146a44->bgs[2].centerY, data_02146a44->bgs[2].x, data_02146a44->bgs[2].y);
        }
    }
    if ((data_02146a44->transformRequests & (1 << 3)) != 0) {
        if (data_02146a44->bgs[3].mode == BGMODE_TEXT) {
            G2_SetBG3Offset(data_02146a44->bgs[3].x, data_02146a44->bgs[3].y);
        } else {
            MtxFx22 mtx;
            MAT2_SetScaleRot(&mtx, data_02146a44->bgs[3].rotation, data_02146a44->bgs[3].scaleX, data_02146a44->bgs[3].scaleY, MAT2_ROT_DEG);
            G2_SetBG3Affine(&mtx, data_02146a44->bgs[3].centerX, data_02146a44->bgs[3].centerY, data_02146a44->bgs[3].x, data_02146a44->bgs[3].y);
        }
    }
    if ((data_02146a44->transformRequests & (1 << 4)) != 0) {
        G2S_SetBG0Offset(data_02146a44->bgs[4].x, data_02146a44->bgs[4].y);
    }
    if ((data_02146a44->transformRequests & (1 << 5)) != 0) {
        G2S_SetBG1Offset(data_02146a44->bgs[5].x, data_02146a44->bgs[5].y);
    }
    if ((data_02146a44->transformRequests & (1 << 6)) != 0) {
        if (data_02146a44->bgs[6].mode == BGMODE_TEXT) {
            G2S_SetBG2Offset(data_02146a44->bgs[6].x, data_02146a44->bgs[6].y);
        } else {
            MtxFx22 mtx;
            MAT2_SetScaleRot(&mtx, data_02146a44->bgs[6].rotation, data_02146a44->bgs[6].scaleX, data_02146a44->bgs[6].scaleY, MAT2_ROT_DEG);
            G2S_SetBG2Affine(&mtx, data_02146a44->bgs[6].centerX, data_02146a44->bgs[6].centerY, data_02146a44->bgs[6].x, data_02146a44->bgs[6].y);
        }
    }
    if ((data_02146a44->transformRequests & (1 << 7)) != 0) {
        if (data_02146a44->bgs[7].mode == BGMODE_TEXT) {
            G2S_SetBG3Offset(data_02146a44->bgs[7].x, data_02146a44->bgs[7].y);
        } else {
            MtxFx22 mtx;
            MAT2_SetScaleRot(&mtx, data_02146a44->bgs[7].rotation, data_02146a44->bgs[7].scaleX, data_02146a44->bgs[7].scaleY, MAT2_ROT_DEG);
            G2S_SetBG3Affine(&mtx, data_02146a44->bgs[7].centerX, data_02146a44->bgs[7].centerY, data_02146a44->bgs[7].x, data_02146a44->bgs[7].y);
        }
    }
}

void GFL_BGSysMoveBGReq(u8 bg, u32 op, int value) {
    GFL_BGSysAdjustBGMoveCoord(&data_02146a44->bgs[bg], op, value);
    data_02146a44->transformRequests |= 1 << bg;
}

void GFL_BGSysRotateBGReq(u8 bg, u32 op, u16 value) {
    GFL_BGSysAdjustBGRotateCoord(&data_02146a44->bgs[bg], op, value);
    data_02146a44->transformRequests |= 1 << bg;
}

void GFL_BGSysScaleBGReq(u8 bg, u32 op, fx32 value) {
    GFL_BGSysAdjustBGScaleCoord(&data_02146a44->bgs[bg], op, value);
    data_02146a44->transformRequests |= 1 << bg;
}

void GFL_BGSysAdjustBGOriginReq(u8 bg, u32 op, int value) {
    GFL_BGSysAdjustBGOriginCoord(&data_02146a44->bgs[bg], op, value);
    data_02146a44->transformRequests |= 1 << bg;
}

BOOL func_02041ff4(u8 bg, u16 x, u16 y, const u16 *colors) {
    u8 *chars;
    u16 index;
    u8 pixelX;
    u8 pixelY;
    u8 color;
    u8 i;

    if (data_02146a44->bgs[bg].screen == NULL) {
        return FALSE;
    }

    // Black shifts the coordinates (asr), where Black 2 divides them by 8
    index = GFL_BGSysConvCoordsToTileIndex(x >> 3, y >> 3, data_02146a44->bgs[bg].resolution);
    chars = GFL_BGSysGetBGCharAddress(bg);
    pixelX = x & 7;
    pixelY = y & 7;

    if (data_02146a44->bgs[bg].colorMode == GX_BG_COLORMODE_16) {
        u16 *screen = data_02146a44->bgs[bg].screen;
        u8 *tile = GFL_HeapAllocate(HEAPID_TAIL(data_02146a44->heapId), 64, FALSE, "bg_sys.c", 2679);

        chars += (screen[index] & 0x3ff) * BGSYS_TILE_SIZE_16;
        for (i = 0; i < BGSYS_TILE_SIZE_16; i++) {
            tile[i * 2] = chars[i] & 0xf;
            tile[i * 2 + 1] = chars[i] >> 4;
        }
        func_02043130((screen[index] >> 10) & 3, tile, data_02146a44->heapId);
        color = tile[pixelX + pixelY * 8];
        GFL_HeapFree(tile);

        if ((*colors & (1 << color)) != 0) {
            return TRUE;
        }
    } else {
        if (data_02146a44->bgs[bg].mode != BGMODE_AFFINE) {
            u16 *screen = data_02146a44->bgs[bg].screen;
            u8 *tile = GFL_HeapAllocate(HEAPID_TAIL(data_02146a44->heapId), 64, FALSE, "bg_sys.c", 2703);

            // BUG: MI_CpuCopy8 takes the source first, so this copies the uninitialized buffer over the tile in VRAM
#ifdef BUGFIX
            MI_CpuCopy8(&chars[(screen[index] & 0x3ff) * BGSYS_TILE_SIZE_256], tile, 64);
#else
            MI_CpuCopy8(tile, &chars[(screen[index] & 0x3ff) * BGSYS_TILE_SIZE_256], 64);
#endif
            func_02043130((screen[index] >> 10) & 3, tile, data_02146a44->heapId);
            color = tile[pixelX + pixelY * 8];
            GFL_HeapFree(tile);
        } else {
            u8 *screen = data_02146a44->bgs[bg].screen;
            color = chars[screen[index] * BGSYS_TILE_SIZE_256 + pixelX + pixelY * 8];
        }

        // BUG: The list is never advanced, so this loops forever unless the first color matches
        while (TRUE) {
            if (*colors == 0xffff) {
                break;
            }
            if (color == (u8)*colors) {
                return TRUE;
            }
#ifdef BUGFIX
            colors++;
#endif
        }
    }
    return FALSE;
}

static u8 GFL_BGSysCalcScreenSizeMode(u8 resolution, u8 mode) {
    switch (mode) {
    case BGMODE_TEXT:
        if (resolution == BGRES_256x256) {
            return GX_BG_SCRSIZE_TEXT_256x256;
        }
        if (resolution == BGRES_256x512) {
            return GX_BG_SCRSIZE_TEXT_256x512;
        }
        if (resolution == BGRES_512x256) {
            return GX_BG_SCRSIZE_TEXT_512x256;
        }
        if (resolution == BGRES_512x512) {
            return GX_BG_SCRSIZE_TEXT_512x512;
        }
        break;
    case BGMODE_AFFINE:
        if (resolution == BGRES_128x128) {
            return GX_BG_SCRSIZE_AFFINE_128x128;
        }
        if (resolution == BGRES_256x256) {
            return GX_BG_SCRSIZE_AFFINE_256x256;
        }
        if (resolution == BGRES_512x512) {
            return GX_BG_SCRSIZE_AFFINE_512x512;
        }
        if (resolution == BGRES_1024x1024) {
            return GX_BG_SCRSIZE_AFFINE_1024x1024;
        }
        break;
    case BGMODE_EXTENDED:
        if (resolution == BGRES_128x128) {
            return GX_BG_SCRSIZE_256x16PLTT_128x128;
        }
        if (resolution == BGRES_256x256) {
            return GX_BG_SCRSIZE_256x16PLTT_256x256;
        }
        if (resolution == BGRES_512x512) {
            return GX_BG_SCRSIZE_256x16PLTT_512x512;
        }
        if (resolution == BGRES_1024x1024) {
            return GX_BG_SCRSIZE_256x16PLTT_1024x1024;
        }
        break;
    }
    return 0;
}

static void GFL_BGSysGetTileDimensions(u8 resolution, u8 *width, u8 *height) {
    switch (resolution) {
    case BGRES_128x128:
        *width = 16;
        *height = 16;
        break;
    case BGRES_256x256:
        *width = 32;
        *height = 32;
        break;
    case BGRES_256x512:
        *width = 32;
        *height = 64;
        break;
    case BGRES_512x256:
        *width = 64;
        *height = 32;
        break;
    case BGRES_512x512:
        *width = 64;
        *height = 64;
        break;
    case BGRES_1024x1024:
        *width = 128;
        *height = 128;
        break;
    }
}

static void GFL_BGSysAdjustBGMoveCoord(BGSysBG *bg, u32 op, int value) {
    switch (op) {
    case BG_MOVE_SET_X:
        bg->x = value;
        break;
    case BG_MOVE_RIGHT:
        bg->x += value;
        break;
    case BG_MOVE_LEFT:
        bg->x -= value;
        break;
    case BG_MOVE_SET_Y:
        bg->y = value;
        break;
    case BG_MOVE_DOWN:
        bg->y += value;
        break;
    case BG_MOVE_UP:
        bg->y -= value;
        break;
    }
}

static void GFL_BGSysSetBGTransformIdentity(u8 bg) {
    MtxFx22 mtx;

    MAT2_SetScaleRot(&mtx, 0, FX32_ONE, FX32_ONE, MAT2_ROT_IDX);
    GFL_BGSysSetBGTransform(bg, &mtx, 0, 0);
}

static void GFL_BGSysUploadScr(u8 bg, const void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    switch (bg) {
    case 0:
        GX_LoadBG0Scr(src, offset, size);
        break;
    case 1:
        GX_LoadBG1Scr(src, offset, size);
        break;
    case 2:
        GX_LoadBG2Scr(src, offset, size);
        break;
    case 3:
        GX_LoadBG3Scr(src, offset, size);
        break;
    case 4:
        GXS_LoadBG0Scr(src, offset, size);
        break;
    case 5:
        GXS_LoadBG1Scr(src, offset, size);
        break;
    case 6:
        GXS_LoadBG2Scr(src, offset, size);
        break;
    case 7:
        GXS_LoadBG3Scr(src, offset, size);
        break;
    }
}

static void GFL_BGSysLoadCharCore(u8 bg, const void *src, u32 size, u32 offset) {
    if (size == 0) {
        u32 uncompSize = MI_GetUncompressedSize(src);
        void *buf = GFL_HeapAllocate(HEAPID_TAIL(data_02146a44->heapId), uncompSize, FALSE, "bg_sys.c", 2963);

        GFL_BGSysTransferUncomp(src, buf, size);
        GFL_BGSysUploadChar(bg, buf, offset, uncompSize);
        GFL_HeapFree(buf);
        return;
    }
    GFL_BGSysUploadChar(bg, src, offset, size);
}

static void GFL_BGSysUploadChar(u8 bg, const void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    switch (bg) {
    case 0:
        GX_LoadBG0Char(src, offset, size);
        break;
    case 1:
        GX_LoadBG1Char(src, offset, size);
        break;
    case 2:
        GX_LoadBG2Char(src, offset, size);
        break;
    case 3:
        GX_LoadBG3Char(src, offset, size);
        break;
    case 4:
        GXS_LoadBG0Char(src, offset, size);
        break;
    case 5:
        GXS_LoadBG1Char(src, offset, size);
        break;
    case 6:
        GXS_LoadBG2Char(src, offset, size);
        break;
    case 7:
        GXS_LoadBG3Char(src, offset, size);
        break;
    }
}

static void GFL_BGSysLoadScrArea_TEXT(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u16 *src, u8 srcX, u8 srcY,
                                      u8 srcWidth, u8 srcHeight, u8 large) {
    u16 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    if (large == FALSE) {
        for (i = 0; i < height; i++) {
            if ((u8)(y + i) < tilesY && (u8)(srcY + i) < srcHeight) {
                for (j = 0; j < width; j++) {
                    if ((u8)(x + j) < tilesX && (u8)(srcX + j) < srcWidth) {
                        screen[GFL_BGSysConvCoordsToTileIndex(x + j, y + i, bg->resolution)] =
                            src[(srcY + i) * srcWidth + srcX + j];
                    }
                }
            }
        }
    } else {
        for (i = 0; i < height; i++) {
            if (y + i >= tilesY || srcY + i >= srcHeight) {
                break;
            }
            for (j = 0; j < width; j++) {
                if (x + j >= tilesX || srcX + j >= srcWidth) {
                    break;
                }
                screen[GFL_BGSysConvCoordsToTileIndex(x + j, y + i, bg->resolution)] =
                    src[GFL_BGSysConvCoordsToTileIndexEx(srcX + j, srcY + i, srcWidth, srcHeight)];
            }
        }
    }
}

static void GFL_BGSysLoadScrArea_AFFINE(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u8 *src, u8 srcX, u8 srcY,
                                        u8 srcWidth, u8 srcHeight, u8 large) {
    u8 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    if (large == FALSE) {
        for (i = 0; i < height; i++) {
            if (y + i >= tilesY || srcY + i >= srcHeight) {
                break;
            }
            for (j = 0; j < width; j++) {
                if (x + j >= tilesX || srcX + j >= srcWidth) {
                    break;
                }
                screen[GFL_BGSysConvCoordsToTileIndex(x + j, y + i, bg->resolution)] =
                    src[(srcY + i) * srcWidth + srcX + j];
            }
        }
    } else {
        for (i = 0; i < height; i++) {
            if (y + i >= tilesY || srcY + i >= srcHeight) {
                break;
            }
            for (j = 0; j < width; j++) {
                if (x + j >= tilesX || srcX + j >= srcWidth) {
                    break;
                }
                screen[GFL_BGSysConvCoordsToTileIndex(x + j, y + i, bg->resolution)] =
                    src[GFL_BGSysConvCoordsToTileIndexEx(srcX + j, srcY + i, srcWidth, srcHeight)];
            }
        }
    }
}

static void GFL_BGSysLoadScrArea_EXTENDED(BGSysBG *bg, u8 x, u8 y, u8 width, u8 height, const u16 *src, u8 srcX,
                                          u8 srcY, u8 srcWidth, u8 srcHeight, u8 large) {
    u16 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    if (large == FALSE) {
        for (i = 0; i < height; i++) {
            if ((u8)(y + i) < tilesY && (u8)(srcY + i) < srcHeight) {
                for (j = 0; j < width; j++) {
                    if ((u8)(x + j) < tilesX && (u8)(srcX + j) < srcWidth) {
                        screen[(y + i) * tilesX + x + j] = src[(srcY + i) * srcWidth + srcX + j];
                    }
                }
            }
        }
    } else {
        for (i = 0; i < height; i++) {
            if (y + i >= tilesY || srcY + i >= srcHeight) {
                break;
            }
            for (j = 0; j < width; j++) {
                if (x + j >= tilesX || srcX + j >= srcWidth) {
                    break;
                }
                screen[(y + i) * tilesX + x + j] =
                    src[GFL_BGSysConvCoordsToTileIndexEx(srcX + j, srcY + i, srcWidth, srcHeight)];
            }
        }
    }
}

static void GFL_BGSysFillScrArea_TEXT(BGSysBG *bg, u16 tile, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    u16 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;
    u16 index;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    for (i = y; i < y + height; i++) {
        if (i >= tilesY) {
            break;
        }
        for (j = x; j < x + width; j++) {
            if (j >= tilesX) {
                break;
            }
            index = GFL_BGSysConvCoordsToTileIndex(j, i, bg->resolution);
            if (palette == BGSYS_FILL_TILE_PALETTE) {
                screen[index] = tile;
            } else if (palette == BGSYS_FILL_KEEP_PALETTE) {
                screen[index] = (screen[index] & 0xf000) + tile;
            } else {
                screen[index] = (palette << 12) + tile;
            }
        }
    }
}

static void GFL_BGSysFillScrArea_AFFINE(BGSysBG *bg, u8 tile, u8 x, u8 y, u8 width, u8 height) {
    u8 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    for (i = y; i < y + height; i++) {
        if (i >= tilesY) {
            break;
        }
        for (j = x; j < x + width; j++) {
            if (j >= tilesX) {
                break;
            }
            screen[GFL_BGSysConvCoordsToTileIndex(j, i, bg->resolution)] = tile;
        }
    }
}

static void GFL_BGSysFillScrArea_EXTENDED(BGSysBG *bg, u16 tile, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    u16 *screen = bg->screen;
    u8 tilesX;
    u8 tilesY;
    u8 i;
    u8 j;

    if (screen == NULL) {
        return;
    }
    GFL_BGSysGetTileDimensions(bg->resolution, &tilesX, &tilesY);
    for (i = y; i < y + height; i++) {
        if (i >= tilesY) {
            break;
        }
        for (j = x; j < x + width; j++) {
            if (j >= tilesX) {
                break;
            }
            if (palette == BGSYS_FILL_TILE_PALETTE) {
                screen[tilesX * i + j] = tile;
            } else if (palette == BGSYS_FILL_KEEP_PALETTE) {
                screen[tilesX * i + j] = (screen[tilesX * i + j] & 0xf000) + tile;
            } else {
                screen[tilesX * i + j] = (palette << 12) + tile;
            }
        }
    }
}

void func_02042fb0(u32 bg, u32 x, u32 y, u16 map) {
    u16 *screen = data_02146a44->bgs[bg].screen;
    u32 resolution = data_02146a44->bgs[bg].resolution;

    if (screen != NULL) {
        screen[GFL_BGSysConvCoordsToTileIndex(x, y, resolution)] = map;
    }
}

static void GFL_BGSysAllocScreen(u8 bg, u32 offset, u32 size) {
    u32 partial;

    switch (bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        partial = (size % 0x20) != 0 ? 1 : 0;
        GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_MAIN], (offset + data_02146a44->screenBase[bg]) / 0x20,
                           size / 0x20 + partial);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        partial = (size % 0x20) != 0 ? 1 : 0;
        GFL_AreaManSetBits(data_02146a44->charAreas[BGSYS_ENGINE_SUB], (offset + data_02146a44->screenBase[bg]) / 0x20,
                           size / 0x20 + partial);
        break;
    }
}

static void GFL_BGSysFreeScreenMemory(u8 bg, u32 offset, u32 size) {
    u32 partial;

    switch (bg) {
    case 0:
    case 1:
    case 2:
    case 3:
        partial = (size % 0x20) != 0 ? 1 : 0;
        GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_MAIN], (offset + data_02146a44->screenBase[bg]) / 0x20,
                           size / 0x20 + partial);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        partial = (size % 0x20) != 0 ? 1 : 0;
        GFL_AreaManDeAlloc(data_02146a44->charAreas[BGSYS_ENGINE_SUB], (offset + data_02146a44->screenBase[bg]) / 0x20,
                           size / 0x20 + partial);
        break;
    }
}

static void func_02043130(u8 flip, u8 *tile, u32 heapId) {
    u8 *buf;
    u8 i;
    u8 j;

    if (flip == 0) {
        return;
    }
    buf = GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), 64, FALSE, "bg_sys.c", 3400);
    if (flip & 1) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                buf[i * 8 + j] = tile[i * 8 + (7 - j)];
            }
        }
        // BUG: MI_CpuCopy8 takes the source first, so this copies the tile over the flipped one, and the tile is left
        // as it was
#ifdef BUGFIX
        MI_CpuCopy8(buf, tile, 64);
#else
        MI_CpuCopy8(tile, buf, 64);
#endif
    }
    if (flip & 2) {
        // BUG: The same, so the buffer's rows are copied over the tile upside down. After the horizontal flip above the
        // buffer holds the unflipped tile, and the tile is only flipped vertically; otherwise the buffer is
        // uninitialized, and the tile gets its contents
#ifdef BUGFIX
        for (i = 0; i < 8; i++) {
            MI_CpuCopy8(&tile[(7 - i) * 8], &buf[i * 8], 8);
        }
        MI_CpuCopy8(buf, tile, 64);
#else
        for (i = 0; i < 8; i++) {
            MI_CpuCopy8(&buf[i * 8], &tile[(7 - i) * 8], 8);
        }
        MI_CpuCopy8(tile, buf, 64);
#endif
    }
    GFL_HeapFree(buf);
}

static void GFL_BGSysAdjustBGRotateCoord(BGSysBG *bg, u32 op, u16 value) {
    switch (op) {
    case BG_ROTATE_SET:
        bg->rotation = value;
        break;
    case BG_ROTATE_ADD:
        bg->rotation += value;
        break;
    case BG_ROTATE_SUB:
        bg->rotation -= value;
        break;
    }
}

static void GFL_BGSysAdjustBGScaleCoord(BGSysBG *bg, u32 op, fx32 value) {
    switch (op) {
    case BG_SCALE_SET_X:
        bg->scaleX = value;
        break;
    case BG_SCALE_ADD_X:
        bg->scaleX += value;
        break;
    case BG_SCALE_SUB_X:
        bg->scaleX -= value;
        break;
    case BG_SCALE_SET_Y:
        bg->scaleY = value;
        break;
    case BG_SCALE_ADD_Y:
        bg->scaleY += value;
        break;
    case BG_SCALE_SUB_Y:
        bg->scaleY -= value;
        break;
    }
}

static void GFL_BGSysAdjustBGOriginCoord(BGSysBG *bg, u32 op, int value) {
    switch (op) {
    case BG_CENTER_SET_X:
        bg->centerX = value;
        break;
    case BG_CENTER_ADD_X:
        bg->centerX += value;
        break;
    case BG_CENTER_SUB_X:
        bg->centerX -= value;
        break;
    case BG_CENTER_SET_Y:
        bg->centerY = value;
        break;
    case BG_CENTER_ADD_Y:
        bg->centerY += value;
        break;
    case BG_CENTER_SUB_Y:
        bg->centerY -= value;
        break;
    }
}
