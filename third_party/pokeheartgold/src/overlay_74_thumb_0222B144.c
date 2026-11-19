#include "global.h"
#include "sprite_resources_header.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_74_mystery_gift_animation.h"

struct SysTask;
struct SysTaskQueue;
enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
void Sprite_SetAnimationFrame(Sprite *sprite, u16 frame);
void Sprite_SetDrawPriority(Sprite *sprite, u32 priority);
Sprite *ov74_0222B0C8(MysteryGiftAnimationManager *animMan, int vramType);
void Sprite_Delete(Sprite *gfxElem);
void Sprite_UpdateAnim(Sprite *sprite, fx32 frames);
void SysTask_Destroy(SysTask *task);

void ov74_0222B144(MysteryGiftAnimationManager *animMan);
void ov74_0222B19C(MysteryGiftAnimationManager *animMan);
void ov74_0222B1F4(MysteryGiftAnimationManager *animMan);
void ov74_0222B20C(MysteryGiftAnimationManager *animMan);
void ov74_0222B224(MysteryGiftAnimationManager *animMan);
void ov74_0222B288(MysteryGiftAnimationManager *animMan);
void ov74_0222B2C4(MysteryGiftAnimationManager *animMan);
void ov74_0222B30C(MysteryGiftAnimationManager *animMan);
void ov74_0222B344(MysteryGiftAnimationManager *animMan);
void ov74_0222B374(MysteryGiftAnimationManager *animMan);

void ov74_0222B144(MysteryGiftAnimationManager *animMan) {
    animMan->topScreenLargeParticle.sprite = ov74_0222B0C8(animMan, NNS_G2D_VRAM_TYPE_2DMAIN);

    Sprite_TryChangeAnimSeq(animMan->topScreenLargeParticle.sprite, 2);
    Sprite_SetAnimationFrame(animMan->topScreenLargeParticle.sprite, 0);
    Sprite_SetDrawFlag(animMan->topScreenLargeParticle.sprite, 0);
    Sprite_SetDrawPriority(animMan->topScreenLargeParticle.sprite, 0);
    Sprite_SetAffineOverwriteMode(animMan->topScreenLargeParticle.sprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    VecFx32 *spritePos = (VecFx32 *)Sprite_GetMatrixPtr(animMan->topScreenLargeParticle.sprite);
    spritePos->x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
    spritePos->y = ((fx32)(((192 / 2) > 0) ? ((192 / 2) * (1 << 12) + 0.5f) : ((192 / 2) * (1 << 12) - 0.5f)));
}

void ov74_0222B19C(MysteryGiftAnimationManager *animMan) {
    animMan->bottomScreenLargeParticle.sprite = ov74_0222B0C8(animMan, NNS_G2D_VRAM_TYPE_2DSUB);

    Sprite_TryChangeAnimSeq(animMan->bottomScreenLargeParticle.sprite, 4);
    Sprite_SetAnimationFrame(animMan->bottomScreenLargeParticle.sprite, 0);
    Sprite_SetDrawFlag(animMan->bottomScreenLargeParticle.sprite, 0);
    Sprite_SetDrawPriority(animMan->bottomScreenLargeParticle.sprite, 0);
    Sprite_SetAffineOverwriteMode(animMan->bottomScreenLargeParticle.sprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    VecFx32 *spritePos = (VecFx32 *)Sprite_GetMatrixPtr(animMan->bottomScreenLargeParticle.sprite);
    spritePos->x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
    spritePos->y = ((fx32)(((256) > 0) ? ((256) * (1 << 12) + 0.5f) : ((256) * (1 << 12) - 0.5f)));
}

void ov74_0222B1F4(MysteryGiftAnimationManager *animMan) {
    Sprite_Delete(animMan->topScreenLargeParticle.sprite);
    animMan->topScreenLargeParticle.sprite = 0L;
}

void ov74_0222B20C(MysteryGiftAnimationManager *animMan) {
    Sprite_Delete(animMan->bottomScreenLargeParticle.sprite);
    animMan->bottomScreenLargeParticle.sprite = 0L;
}

void ov74_0222B224(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        if (animMan->topScreenSmallParticles[i].sprite) {
            Sprite_UpdateAnim(animMan->topScreenSmallParticles[i].sprite, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
        }

        if (animMan->bottomScreenSmallParticles[i].sprite) {
            Sprite_UpdateAnim(animMan->bottomScreenSmallParticles[i].sprite, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
        }
    }

    if (animMan->topScreenLargeParticle.sprite) {
        Sprite_UpdateAnim(animMan->topScreenLargeParticle.sprite, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
    }

    if (animMan->bottomScreenLargeParticle.sprite) {
        Sprite_UpdateAnim(animMan->bottomScreenLargeParticle.sprite, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
    }
}

void ov74_0222B288(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        animMan->topScreenSmallParticles[i].sprite = ov74_0222B0C8(animMan, NNS_G2D_VRAM_TYPE_2DMAIN);
        Sprite_TryChangeAnimSeq(animMan->topScreenSmallParticles[i].sprite, 1);
        Sprite_SetAnimationFrame(animMan->topScreenSmallParticles[i].sprite, 0);
    }
}

void ov74_0222B2C4(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        if (i < (80 / 4)) {
            animMan->topScreenSmallParticles[i].sprite = ov74_0222B0C8(animMan, NNS_G2D_VRAM_TYPE_2DMAIN);
            Sprite_TryChangeAnimSeq(animMan->topScreenSmallParticles[i].sprite, 1);
            Sprite_SetAnimationFrame(animMan->topScreenSmallParticles[i].sprite, 0);
        } else {
            animMan->topScreenSmallParticles[i].sprite = 0L;
        }
    }
}

void ov74_0222B30C(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        animMan->bottomScreenSmallParticles[i].sprite = ov74_0222B0C8(animMan, NNS_G2D_VRAM_TYPE_2DSUB);
        Sprite_TryChangeAnimSeq(animMan->bottomScreenSmallParticles[i].sprite, 5);
        Sprite_SetAnimationFrame(animMan->bottomScreenSmallParticles[i].sprite, 0);
    }
}

void ov74_0222B344(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        if (animMan->topScreenSmallParticles[i].sprite == 0L) {
            continue;
        }

        Sprite_Delete(animMan->topScreenSmallParticles[i].sprite);
        animMan->topScreenSmallParticles[i].sprite = 0L;
        SysTask_Destroy(animMan->topScreenSmallParticles[i].animationSysTask);
    }
}

void ov74_0222B374(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        Sprite_Delete(animMan->bottomScreenSmallParticles[i].sprite);
        animMan->bottomScreenSmallParticles[i].sprite = 0L;
        SysTask_Destroy(animMan->bottomScreenSmallParticles[i].animationSysTask);
    }
}
