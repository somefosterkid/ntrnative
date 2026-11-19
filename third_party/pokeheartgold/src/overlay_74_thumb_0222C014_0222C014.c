#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_74_mystery_gift_animation.h"

void ov74_0222AF28(MysteryGiftAnimationManager *animMan);

void ov74_0222B3A4(MysteryGiftAnimationManager *animMan);

void ov74_0222BF08(void *arg0);

s32 ov74_0222B144(MysteryGiftAnimationManager *);

s32 ov74_0222B288(MysteryGiftAnimationManager *);

static void ov74_0222C014(MysteryGiftAnimationManager *arg0);

static void ov74_0222C014(MysteryGiftAnimationManager *arg0) {
    arg0->blendBrightness = 0;
    ov74_0222BF08((u8 *)arg0);
    ov74_0222AF28(arg0);
    ov74_0222B288(arg0);
    ov74_0222B3A4(arg0);
    ov74_0222B144(arg0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}

struct MainMenuSpriteManager;

struct MysteryGiftAnimationManager;

struct MysteryGiftParticle;

enum MysteryGiftAnimBrightnessFadeDirection {
    FADE_TOWARDS_NORMAL,
    FADE_TOWARDS_BLACK,
    FADE_TOWARDS_WHITE
};

enum MysteryGiftAnimationStage {
    MG_ANIMATION_STAGE_SETUP = 0,
    MG_ANIMATION_STAGE_FADE_BG_BLACK,
    MG_ANIMATION_STAGE_PARTICLES_GATHERING,
    MG_ANIMATION_STAGE_SETUP_IMPLOSION,
    MG_ANIMATION_STAGE_IMPLODE,
    MG_ANIMATION_STAGE_FLASH_WHITE,
    MG_ANIMATION_STAGE_MOVE_TO_BOTTOM_SCREEN,
    MG_ANIMATION_STAGE_SPIRAL_OUT,
    MG_ANIMATION_STAGE_FADE_BG_WHITE,
    MG_ANIMATION_STAGE_RETURN_BG_NORMAL,
    MG_ANIMATION_STAGE_DONE = 255,
};

void ov74_0222B19C(MysteryGiftAnimationManager *animMan);

void ov74_0222B1F4(MysteryGiftAnimationManager *animMan);

void ov74_0222B20C(MysteryGiftAnimationManager *animMan);

void ov74_0222B224(MysteryGiftAnimationManager *animMan);

void ov74_0222B2C4(MysteryGiftAnimationManager *animMan);

void ov74_0222B30C(MysteryGiftAnimationManager *animMan);

void ov74_0222B344(MysteryGiftAnimationManager *animMan);

void ov74_0222B374(MysteryGiftAnimationManager *animMan);

void ov74_0222B5F8(MysteryGiftAnimationManager *animMan);

void ov74_0222B760(MysteryGiftAnimationManager *animMan);

void ov74_0222BF18(void);

BOOL ov74_0222BF2C(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed);

BOOL ov74_0222BFA0(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed);

void ov74_0222C04C(SysTask *sysTask, void *param);

void ov74_0222C04C(SysTask *sysTask, void *param) {
    MysteryGiftAnimationManager *animMan = param;
    BOOL goToNextStage;

    switch (animMan->animationStage) {
    case MG_ANIMATION_STAGE_SETUP: {
        ov74_0222C014(animMan);
        animMan->animationStage = MG_ANIMATION_STAGE_FADE_BG_BLACK;
    } break;
    case MG_ANIMATION_STAGE_FADE_BG_BLACK: {
        goToNextStage = ov74_0222BF2C(animMan, FADE_TOWARDS_BLACK, 2);

        if (goToNextStage) {
            animMan->animationStage = MG_ANIMATION_STAGE_PARTICLES_GATHERING;
        }
    } break;
    case MG_ANIMATION_STAGE_PARTICLES_GATHERING: {
        int numParticlesDone = 0;

        goToNextStage = 1;

        for (int i = 0; i < 80; i++) {
            if (animMan->topScreenSmallParticles[i].animationDone) {
                numParticlesDone++;
                continue;
            }

            goToNextStage = 0;
        }

        if (numParticlesDone > 50) {
            Sprite_TryChangeAnimSeq(animMan->topScreenLargeParticle.sprite, 4);
        } else if (numParticlesDone > 30) {
            Sprite_TryChangeAnimSeq(animMan->topScreenLargeParticle.sprite, 3);
        } else if (numParticlesDone > 7) {
            Sprite_SetDrawFlag(animMan->topScreenLargeParticle.sprite, 1);
        }

        if (goToNextStage || (numParticlesDone == (80 - 1))) {
            animMan->animationStage = MG_ANIMATION_STAGE_SETUP_IMPLOSION;
            ov74_0222B344(animMan);
        }
    } break;
    case MG_ANIMATION_STAGE_SETUP_IMPLOSION:
        if (*animMan->animationStatusPtr == MG_ANIMATION_STATUS_PROCEED_IMPLOSION) {
            ov74_0222B2C4(animMan);
            ov74_0222B5F8(animMan);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
            ov74_0222BF18();
            animMan->animationStage = MG_ANIMATION_STAGE_IMPLODE;
        }
        break;
    case MG_ANIMATION_STAGE_IMPLODE: {
        int numParticlesDone = 0;

        goToNextStage = 1;

        for (int i = 0; i < 80; i++) {
            if (animMan->topScreenSmallParticles[i].sprite == 0L) {
                continue;
            }

            if (animMan->topScreenSmallParticles[i].animationDone) {
                numParticlesDone++;
                continue;
            }

            goToNextStage = 0;
        }

        ov74_0222BFA0(animMan, FADE_TOWARDS_WHITE, 2);

        if (goToNextStage || (numParticlesDone == ((80 / 4) - 1))) {
            PlaySE(1562);
            animMan->animationStage = MG_ANIMATION_STAGE_FLASH_WHITE;
            ov74_0222B344(animMan);
            ov74_0222B19C(animMan);
        }
    } break;
    case MG_ANIMATION_STAGE_FLASH_WHITE:
        goToNextStage = ov74_0222BFA0(animMan, FADE_TOWARDS_WHITE, 2);

        if (goToNextStage) {
            ov74_0222BFA0(animMan, FADE_TOWARDS_BLACK, 2);
            animMan->animationStage = MG_ANIMATION_STAGE_MOVE_TO_BOTTOM_SCREEN;
        }
        break;
    case MG_ANIMATION_STAGE_MOVE_TO_BOTTOM_SCREEN:
        goToNextStage = ov74_0222BFA0(animMan, FADE_TOWARDS_BLACK, 2);

        if (goToNextStage == 0) {
            break;
        }

        VecFx32 *topLargeParticlePos = (VecFx32 *)Sprite_GetMatrixPtr(animMan->topScreenLargeParticle.sprite);
        VecFx32 *bottomLargeParticlePos = (VecFx32 *)Sprite_GetMatrixPtr(animMan->bottomScreenLargeParticle.sprite);

        if (topLargeParticlePos->y < ((fx32)(((256 - 32) > 0) ? ((256 - 32) * (1 << 12) + 0.5f) : ((256 - 32) * (1 << 12) - 0.5f)))) {
            topLargeParticlePos->y += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
        }

        fx32 topLargeParticleYPastScreen = topLargeParticlePos->y - ((fx32)(((192) > 0) ? ((192) * (1 << 12) + 0.5f) : ((192) * (1 << 12) - 0.5f)));

        if (topLargeParticleYPastScreen > ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)))) {
            if (bottomLargeParticlePos->y < ((fx32)(((384) > 0) ? ((384) * (1 << 12) + 0.5f) : ((384) * (1 << 12) - 0.5f)))) {
                bottomLargeParticlePos->y += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));

                if (Sprite_GetDrawFlag(animMan->bottomScreenLargeParticle.sprite) == 0) {
                    Sprite_SetDrawFlag(animMan->bottomScreenLargeParticle.sprite, 1);
                }
            } else {
                animMan->animationStage = MG_ANIMATION_STAGE_SPIRAL_OUT;
                ov74_0222B1F4(animMan);
                ov74_0222B30C(animMan);
                ov74_0222B760(animMan);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
            }
        }
        break;
    case MG_ANIMATION_STAGE_SPIRAL_OUT:
        goToNextStage = 1;

        int numParticlesDone = 0;

        for (int i = 0; i < 80; i++) {
            if (animMan->bottomScreenSmallParticles[i].animationDone) {
                numParticlesDone++;
                continue;
            }

            goToNextStage = 0;
        }

        if (numParticlesDone > 7) {
            ov74_0222BF2C(animMan, FADE_TOWARDS_WHITE, 1);
        }

        if (numParticlesDone > 50) {
            Sprite_SetDrawFlag(animMan->bottomScreenLargeParticle.sprite, 0);
        } else if (numParticlesDone > 30) {
            Sprite_TryChangeAnimSeq(animMan->bottomScreenLargeParticle.sprite, 2);
        } else if (numParticlesDone > 7) {
            Sprite_TryChangeAnimSeq(animMan->bottomScreenLargeParticle.sprite, 3);
        }

        if (goToNextStage) {
            animMan->animationStage = MG_ANIMATION_STAGE_FADE_BG_WHITE;
            ov74_0222B374(animMan);
            ov74_0222B20C(animMan);
        }
        break;
    case MG_ANIMATION_STAGE_FADE_BG_WHITE: {
        goToNextStage = ov74_0222BF2C(animMan, FADE_TOWARDS_WHITE, 1);

        if (goToNextStage) {
            animMan->animationStage = MG_ANIMATION_STAGE_RETURN_BG_NORMAL;
        }
    } break;
    case MG_ANIMATION_STAGE_RETURN_BG_NORMAL: {
        goToNextStage = ov74_0222BF2C(animMan, FADE_TOWARDS_NORMAL, 2);

        if (goToNextStage) {
            animMan->animationStage = MG_ANIMATION_STAGE_DONE;
        }
    } break;
    default:
        *animMan->animationStatusPtr = MG_ANIMATION_STATUS_DONE;
        SysTask_Destroy(sysTask);
        Heap_Free(animMan);
        return;
    }

    ov74_0222B224(animMan);
}
