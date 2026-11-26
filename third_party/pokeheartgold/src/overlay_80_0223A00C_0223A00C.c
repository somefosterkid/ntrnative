#include "global.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite_system.h"

struct BgConfig;

struct FrontierBrainEncounterEffectAnimations;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct UnkStruct_02012744_t;

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct BgConfig BgConfig;

void *BgGetCharPtr(u8 bgLayer);

void ScheduleSetBgPosText(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

void *SysTask_GetData(SysTask *task);

SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);

SysTask *CreateSysTaskAndEnvironment(SysTaskFunc callback, int paramSize, u32 priority, enum HeapID heapID);

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

void ov80_0223A0EC(SysTask *task, void *data);

void ov80_0223A144(SysTask *task, void *data);

void ov80_0223A938(FrontierBrainEncounterEffect *encEffect, const FrontierBrainEncounterParams *encParams);

extern const FrontierBrainEncounterParams ov80_0223DB98[6];

void ov80_0223A00C(int facility, BgConfig *bgConfig, SpriteSystem *spriteSys, SpriteManager *spriteMan, PaletteData *plttData, u16 *isFinishedPtr, s16 x, s16 y);

void ov80_0223A00C(int facility, BgConfig *bgConfig, SpriteSystem *spriteSys, SpriteManager *spriteMan, PaletteData *plttData, u16 *isFinishedPtr, s16 x, s16 y) {
    SysTask *task = CreateSysTaskAndEnvironment(ov80_0223A144, sizeof(FrontierBrainEncounterEffect), 1000, HEAP_ID_101);
    FrontierBrainEncounterEffect *encEffect = SysTask_GetData(task);

    encEffect->bgConfig = bgConfig;
    encEffect->spriteSys = spriteSys;
    encEffect->spriteMan = spriteMan;
    encEffect->currentBannerPalette = plttData;
    encEffect->effectIsFinished = isFinishedPtr;
    encEffect->xAnchor = x;
    encEffect->yAnchor = y;
    encEffect->narc = NARC_New((enum NarcId)109, HEAP_ID_101);

    if (encEffect->effectIsFinished != NULL) {
        *encEffect->effectIsFinished = FALSE;
    }

    encEffect->facility = facility - 1;

    MI_CpuClear32(BgGetCharPtr(GF_BG_LYR_MAIN_1), 0x8000);

    ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_1, BG_OFFSET_UPDATE_SET_X, 0);
    ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_1, BG_OFFSET_UPDATE_SET_Y, 0);

    ov80_0223A938(encEffect, &ov80_0223DB98[encEffect->facility]);

    encEffect->vBlankCallback = SysTask_CreateOnVWaitQueue(ov80_0223A0EC, encEffect, 1);
}
