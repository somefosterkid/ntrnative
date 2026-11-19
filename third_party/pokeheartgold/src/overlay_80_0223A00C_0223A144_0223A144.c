#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "overlay_80_0223A00C.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "linear_interpolation_task.h"
#include "unk_02013534_font_system.h"

typedef struct FrontierBrainNameObject FrontierBrainNameObject;

typedef struct FrontierBrainBannerAnimation FrontierBrainBannerAnimation;

typedef struct FrontierBrainMugshotSlideInManager FrontierBrainMugshotSlideInManager;

typedef struct FrontierBrainVsSpriteAnimation FrontierBrainVsSpriteAnimation;

typedef struct FrontierBrainEncounterEffectAnimations FrontierBrainEncounterEffectAnimations;

typedef struct FrontierBrainEncounterEffect FrontierBrainEncounterEffect;

typedef struct FrontierBrainsEncounterParams FrontierBrainEncounterParams;

struct FrontierBrainNameObject {
    TextOBJ *fontOAM;
    UnkStruct_02021AC8 charTransferAllocation;
    u16 width;
};

struct FrontierBrainBannerAnimation {
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
};

struct FrontierBrainMugshotSlideInManager {
    int xOffset;
    int state;
};

struct FrontierBrainVsSpriteAnimation {
    s16 taskStartDelay;
    s16 tasksStarted;
    ManagedSprite *vsSprites[4];
    LinearInterpolationTaskFX32 tasks[4];
};

struct FrontierBrainEncounterEffectAnimations {
    LinearInterpolationTaskFX32 unused;
    FrontierBrainVsSpriteAnimation vsSpriteAnimation;
    s32 stateDelay;
    FrontierBrainNameObject brainName;
    ManagedSprite *mugshotSprite;
};

struct FrontierBrainEncounterEffect {
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
};

struct FrontierBrainsEncounterParams {
    u32 name;
    u8 mugshotPalette;
    u8 mugshotTiles;
    u8 mugshotCells;
    u8 mugshotAnims;
    u8 bannerPalette;
    u8 bannerTiles;
    u8 bannerTilemap;
    u8 unused;
};

BOOL ov80_0223A174(FrontierBrainEncounterEffect *encEffect, u32 heapID, const FrontierBrainEncounterParams *encounterParams);

void ov80_0223A0C0(void *, SysTask *);

extern void *ov80_0223DB98;

void ov80_0223A144(SysTask *task, void *data) {
    BOOL BVar1;

    BVar1 = ov80_0223A174(data, 0x65, (FrontierBrainEncounterParams *)(&ov80_0223DB98 + (u32) * (u8 *)((u8 *)data + 0x2c) * 3));
    if (BVar1 == 1) {
        ov80_0223A0C0(data, task);
    }
    return;
}
