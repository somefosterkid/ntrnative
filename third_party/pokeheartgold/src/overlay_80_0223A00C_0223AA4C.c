#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "overlay_80_0223A00C.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02013534.h"

struct TextOBJ;
struct FrontierBrainEncounterEffectAnimations;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
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
void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);
void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 x, s16 y, fx32 offset);
typedef struct LinearInterpolationTaskFX32 {
    fx32 currentValue;
    fx32 startValue;
    fx32 delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskFX32;
typedef struct FrontierBrainNameObject {
    TextOBJ *fontOAM;
    UnkStruct_02021AC8 charTransferAllocation;
    u16 width;
} FrontierBrainNameObject;
typedef struct FrontierBrainVsSpriteAnimation {
    s16 taskStartDelay;
    s16 tasksStarted;
    ManagedSprite *vsSprites[4];
    LinearInterpolationTaskFX32 tasks[4];
} FrontierBrainVsSpriteAnimation;
struct FrontierBrainEncounterEffectAnimations {
    LinearInterpolationTaskFX32 unused;
    FrontierBrainVsSpriteAnimation vsSpriteAnimation;
    s32 stateDelay;
    FrontierBrainNameObject brainName;
    ManagedSprite *mugshotSprite;
};

void ov80_0223AA80(FrontierBrainEncounterEffect *encEffect, BOOL isClosing);
BOOL ov80_0223AB94(FrontierBrainEncounterEffect *encEffect, FrontierBrainEncounterEffectAnimations *anims, FrontierBrainMugshotSlideInManager *mugshotSlideIn);

void ov80_0223AA4C(SysTask *task, void *taskData) {
    FrontierBrainEncounterEffect *encEffect = taskData;

    encEffect->bannerPaletteTimer++;

    if (encEffect->bannerPaletteTimer < 0) {
        return;
    }

    encEffect->bannerPaletteTimer = 0;
    encEffect->bannerPaletteIdx++;

    if (encEffect->bannerPaletteIdx >= 8) {
        encEffect->bannerPaletteIdx = 0;
    }

    PaletteData_LoadPalette(encEffect->currentBannerPalette, &encEffect->allPalettes[encEffect->bannerPaletteIdx * (16)], PLTTBUF_MAIN_BG, ((12) * (16)), ((16) * sizeof(u16)));
}

static void ov80_0223AAD0(SysTask *task, void *data);
static void ov80_0223AB34(SysTask *task, void *data);

void ov80_0223AA80(FrontierBrainEncounterEffect *encEffect, BOOL isClosing) {
    FrontierBrainBannerAnimation *bannerAnim = &encEffect->bannerAnim;
    MI_CpuClear8(bannerAnim, sizeof(FrontierBrainBannerAnimation));

    if (!isClosing) {
        bannerAnim->top = (10 * 8) << 8;
        bannerAnim->bottom = (10 * 8) << 8;

        SysTask_CreateOnMainQueue(ov80_0223AAD0, bannerAnim, 1000);
    } else {
        bannerAnim->top = ((10 * 8) - (8 * 8 + 4) / 2) << 8;
        bannerAnim->bottom = ((10 * 8) + (8 * 8 + 4) / 2) << 8;

        SysTask_CreateOnMainQueue(ov80_0223AB34, bannerAnim, 1000);
    }
}

static void ov80_0223AAD0(SysTask *task, void *data) {
    FrontierBrainBannerAnimation *bannerAnim = data;

    switch (bannerAnim->state) {
    case 0:
        bannerAnim->top -= 0x800;
        bannerAnim->bottom += 0x800;

        if (bannerAnim->top <= ((10 * 8) << 8) - (((8 * 8 + 4) / 2) << 8)) {
            bannerAnim->top = ((10 * 8) - (8 * 8 + 4) / 2) << 8;
            bannerAnim->bottom = ((10 * 8) + (8 * 8 + 4) / 2) << 8;
            bannerAnim->state++;
        }

        bannerAnim->left0 = 0;
        bannerAnim->top0 = bannerAnim->top >> 8;
        bannerAnim->right0 = 255;
        bannerAnim->bottom0 = bannerAnim->bottom >> 8;

        bannerAnim->left1 = 1;
        bannerAnim->top1 = bannerAnim->top >> 8;
        bannerAnim->right1 = 0;
        bannerAnim->bottom1 = bannerAnim->bottom >> 8;
        break;
    default:
        bannerAnim->isFinished = 1;
        SysTask_Destroy(task);
        return;
    }
}

static void ov80_0223AB34(SysTask *task, void *data) {
    FrontierBrainBannerAnimation *bannerAnim = data;

    switch (bannerAnim->state) {
    case 0:
        bannerAnim->top += 0x800;
        bannerAnim->bottom -= 0x800;

        if (bannerAnim->top >= ((10 * 8) << 8)) {
            bannerAnim->top = (10 * 8) << 8;
            bannerAnim->bottom = (10 * 8) << 8;
            bannerAnim->state++;
        }

        bannerAnim->left0 = 0;
        bannerAnim->top0 = bannerAnim->top >> 8;
        bannerAnim->right0 = 256 - 1;
        bannerAnim->bottom0 = bannerAnim->bottom >> 8;

        bannerAnim->left1 = 1;
        bannerAnim->top1 = bannerAnim->top >> 8;
        bannerAnim->right1 = 0;
        bannerAnim->bottom1 = bannerAnim->bottom >> 8;
        break;
    default:
        bannerAnim->isFinished = 1;
        SysTask_Destroy(task);
        return;
    }
}

BOOL ov80_0223AB94(FrontierBrainEncounterEffect *encEffect, FrontierBrainEncounterEffectAnimations *anims, FrontierBrainMugshotSlideInManager *mugshotSlideIn) {
    switch (mugshotSlideIn->state) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(anims->mugshotSprite, 256 + encEffect->xAnchor, 80 + encEffect->yAnchor, ((fx32)(((512) > 0) ? ((512) * (1 << 12) + 0.5f) : ((512) * (1 << 12) - 0.5f))));
        ManagedSprite_SetDrawFlag(anims->mugshotSprite, 1);
        mugshotSlideIn->xOffset = 256 << 8;
        mugshotSlideIn->state++;
        break;
    case 1:
        mugshotSlideIn->xOffset -= 3840;

        if (mugshotSlideIn->xOffset <= 208 << 8) {
            mugshotSlideIn->xOffset = 208 << 8;
            mugshotSlideIn->state++;
        }

        ManagedSprite_SetPositionXYWithSubscreenOffset(anims->mugshotSprite, mugshotSlideIn->xOffset >> 8, 80 + encEffect->yAnchor, ((fx32)(((512) > 0) ? ((512) * (1 << 12) + 0.5f) : ((512) * (1 << 12) - 0.5f))));
        break;
    default:
        return 1;
    }

    return 0;
}
