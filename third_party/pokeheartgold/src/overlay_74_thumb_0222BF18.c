#include "global.h"
#include "sprite_resources_header.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_74_mystery_gift_animation.h"

struct SysTask;
struct SysTaskQueue;
void BG_SetMaskColor(u8 bgLayer, u16 mask);
enum MysteryGiftAnimBrightnessFadeDirection {
    FADE_TOWARDS_NORMAL,
    FADE_TOWARDS_BLACK,
    FADE_TOWARDS_WHITE
};
void ov74_0222BF08(MysteryGiftAnimationManager *animMan);
void ov74_0222BED8(MysteryGiftAnimationManager *animMan);

void ov74_0222BF18(void);
BOOL ov74_0222BF2C(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed);
BOOL ov74_0222BFA0(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed);

void ov74_0222BF18(void) {
    BG_SetMaskColor(GF_BG_LYR_MAIN_1, (((0) << 10) | ((0) << 5) | (0)));
    BG_SetMaskColor(GF_BG_LYR_SUB_1, (((0) << 10) | ((0) << 5) | (0)));
}

BOOL ov74_0222BF2C(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed) {
    BOOL reachedLimit = 1;

    switch (direction) {
    case FADE_TOWARDS_NORMAL:
        if (animMan->blendBrightness > 0) {
            if (animMan->blendBrightness - speed > 0) {
                animMan->blendBrightness -= speed;
                reachedLimit = 0;
            } else {
                animMan->blendBrightness = 0;
            }
        } else if (animMan->blendBrightness < 0) {
            if (animMan->blendBrightness + speed < 0) {
                animMan->blendBrightness += speed;
                reachedLimit = 0;
            } else {
                animMan->blendBrightness = 0;
            }
        }
        break;
    case FADE_TOWARDS_BLACK:
        if (animMan->blendBrightness - speed > -16) {
            animMan->blendBrightness -= speed;
            reachedLimit = 0;
        } else {
            animMan->blendBrightness = -16;
        }
        break;
    case FADE_TOWARDS_WHITE:
        if (animMan->blendBrightness + speed < 16) {
            animMan->blendBrightness += speed;
            reachedLimit = 0;
        } else {
            animMan->blendBrightness = 16;
        }
        break;
    }

    ov74_0222BF08(animMan);
    return reachedLimit;
}

BOOL ov74_0222BFA0(MysteryGiftAnimationManager *animMan, enum MysteryGiftAnimBrightnessFadeDirection direction, s8 speed) {
    BOOL reachedLimit = 1;

    switch (direction) {
    case FADE_TOWARDS_NORMAL:
        if (animMan->blendBrightness > 0) {
            if (animMan->blendBrightness - speed > 0) {
                animMan->blendBrightness -= speed;
                reachedLimit = 0;
            } else {
                animMan->blendBrightness = 0;
            }
        } else if (animMan->blendBrightness < 0) {
            if (animMan->blendBrightness + speed < 0) {
                animMan->blendBrightness += speed;
                reachedLimit = 0;
            } else {
                animMan->blendBrightness = 0;
            }
        }
        break;
    case FADE_TOWARDS_BLACK:
        if (animMan->blendBrightness - speed > -16) {
            animMan->blendBrightness -= speed;
            reachedLimit = 0;
        } else {
            animMan->blendBrightness = -16;
        }
        break;
    case FADE_TOWARDS_WHITE:
        if (animMan->blendBrightness + speed < 16) {
            animMan->blendBrightness += speed;
            reachedLimit = 0;
        } else {
            animMan->blendBrightness = 16;
        }
        break;
    }

    ov74_0222BED8(animMan);
    return reachedLimit;
}
