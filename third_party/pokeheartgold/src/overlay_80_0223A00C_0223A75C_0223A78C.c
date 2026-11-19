#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;

struct UnkStruct_02012744_t;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

typedef struct LinearInterpolationTaskFX32 {
    fx32 currentValue;
    fx32 startValue;
    fx32 delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskFX32;

typedef struct FrontierBrainNameObject {
    FontOAM *fontOAM;
    UnkStruct_02021AC8 charTransferAllocation;
    u16 width;
} FrontierBrainNameObject;

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

typedef struct FrontierBrainVsSpriteAnimation {
    s16 taskStartDelay;
    s16 tasksStarted;
    ManagedSprite *vsSprites[4];
    LinearInterpolationTaskFX32 tasks[4];
} FrontierBrainVsSpriteAnimation;

typedef struct FrontierBrainEncounterEffectAnimations {
    LinearInterpolationTaskFX32 unused;
    FrontierBrainVsSpriteAnimation vsSpriteAnimation;
    s32 stateDelay;
    FrontierBrainNameObject brainName;
    ManagedSprite *mugshotSprite;
} FrontierBrainEncounterEffectAnimations;

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

void ov80_0223A8C4(LinearInterpolationTaskFX32 *task, fx32 startValue, fx32 endValue, int numSteps);

extern const ManagedSpriteTemplate ov80_0223DB64;

void ov80_0223A78C(FrontierBrainEncounterEffect *encEffect, FrontierBrainVsSpriteAnimation *vsSpriteAnimation, fx32 x, fx32 y, u32 heapID);

void ov80_0223A78C(FrontierBrainEncounterEffect *encEffect, FrontierBrainVsSpriteAnimation *vsSpriteAnimation, fx32 x, fx32 y, u32 heapID) {
    ManagedSpriteTemplate vsTemplate = ov80_0223DB64;

    vsTemplate.x = x >> FX32_SHIFT;
    vsTemplate.y = y >> FX32_SHIFT;

    vsSpriteAnimation->taskStartDelay = 0;
    vsSpriteAnimation->tasksStarted = 0;

    for (int i = 0; i < 4; i++) {
        vsSpriteAnimation->vsSprites[i] = SpriteSystem_NewSprite(encEffect->spriteSys, encEffect->spriteMan, &vsTemplate);
        ManagedSprite_SetDrawFlag(vsSpriteAnimation->vsSprites[i], FALSE);

        if (i != 3) {
            Sprite_SetAffineOverwriteMode(vsSpriteAnimation->vsSprites[i]->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
            Sprite_SetAnimCtrlSeq(vsSpriteAnimation->vsSprites[i]->sprite, 1);
            ov80_0223A8C4(&vsSpriteAnimation->tasks[i], FX32_CONST(2), FX32_CONST(1), 6);
        } else {
            ov80_0223A8C4(&vsSpriteAnimation->tasks[i], FX32_CONST(1), FX32_CONST(1), 6);
        }
    }
}
