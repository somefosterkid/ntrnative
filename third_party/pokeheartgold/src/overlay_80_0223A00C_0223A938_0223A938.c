#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "bg_window.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct BgConfig;

struct FrontierBrainEncounterEffectAnimations;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

void *GfGfxLoader_GetPlttData(enum NarcId narcID, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void Heap_Free(void *ptr);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

typedef struct FrontierBrainBannerAnimation {
    s32 top;
    s32 bottom;
    u8 isFinished;
    u8 state;
    u8 left0;
    u8 top0;
    u8 right0;
    u8 bottom0;
    u8 left1;
    u8 top1;
    u8 right1;
    u8 bottom1;
} FrontierBrainBannerAnimation;

typedef struct FrontierBrainMugshotSlideInManager {
    int xOffset;
    int state;
} FrontierBrainMugshotSlideInManager;

typedef struct FrontierBrainEncounterEffectAnimations FrontierBrainEncounterEffectAnimations;

typedef struct FrontierBrainEncounterEffect {
    int state;
    int unused[2];
    FrontierBrainEncounterEffectAnimations *anims;
    BgConfig *bgConfig;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *currentBannerPalette;
    u16 *effectIsFinished;
    NARC *narc;
    s16 xAnchor;
    s16 yAnchor;
    u8 facility;
    UnkStruct_02012744 *unk_30;
    SysTask *vBlankCallback;
    int bannerPaletteTimer;
    int bannerPaletteIdx;
    u16 allPalettes[8 * (16)];
    SysTask *bannerPaletteTask;
    FrontierBrainBannerAnimation bannerAnim;
    FrontierBrainMugshotSlideInManager mugshotSlideIn;
    int mugshotPlttOffset;
    u32 plttOffsets;
} FrontierBrainEncounterEffect;

typedef struct FrontierBrainsEncounterParams {
    u32 name;
    u8 mugshotPalette;
    u8 mugshotTiles;
    u8 mugshotCells;
    u8 mugshotAnims;
    u8 bannerPalette;
    u8 bannerTiles;
    u8 bannerTilemap;
    u8 unused;
} FrontierBrainEncounterParams;

void ov80_0223AA4C(SysTask *task, void *taskData);

void ov80_0223A938(FrontierBrainEncounterEffect *encEffect, const FrontierBrainEncounterParams *encParams);

void ov80_0223A938(FrontierBrainEncounterEffect *encEffect, const FrontierBrainEncounterParams *encParams) {
    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    G2_SetWndOutsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ) ^ GX_WND_PLANEMASK_BG1, 1);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);

    PaletteData_LoadNarc(encEffect->currentBannerPalette, (enum NarcId)109, encParams->bannerPalette, HEAP_ID_101, PLTTBUF_MAIN_BG, ((16) * sizeof(u16)), ((12) * (16)));
    GfGfxLoader_LoadCharDataFromOpenNarc(encEffect->narc, encParams->bannerTiles, encEffect->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(encEffect->narc, encParams->bannerTilemap, encEffect->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, HEAP_ID_101);
    BgTilemapRectChangePalette(encEffect->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 32, 12);
    ScheduleBgTilemapBufferTransfer(encEffect->bgConfig, GF_BG_LYR_MAIN_1);

    NNSG2dPaletteData *plttData;
    void *pltt = GfGfxLoader_GetPlttData((enum NarcId)109, encParams->bannerPalette, &plttData, HEAP_ID_101);

    MI_CpuCopy16(plttData->pRawData, encEffect->allPalettes, 8 * ((16) * sizeof(u16)));
    Heap_Free(pltt);

    encEffect->bannerPaletteTask = SysTask_CreateOnMainQueue(ov80_0223AA4C, encEffect, 1100);
}
