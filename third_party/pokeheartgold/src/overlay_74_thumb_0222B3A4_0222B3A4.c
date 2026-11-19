#include "global.h"

#include "math_util.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_74_mystery_gift_animation.h"

struct MainMenuSpriteManager;

struct MysteryGiftAnimationManager;

struct MysteryGiftParticle;

struct Sprite;

struct SpriteResource;

struct SpriteResourcesHeader;

struct SysTask;

struct SysTaskQueue;

enum ParticleAnimationType {
    PARTICLE_ANIMATION_TYPE_ORBIT_0 = 0,
    PARTICLE_ANIMATION_TYPE_ORBIT_1,
    PARTICLE_ANIMATION_TYPE_ORBIT_2,
    PARTICLE_ANIMATION_TYPE_ORBIT_3,

    PARTICLE_ANIMATION_TYPE_SPIRAL_OUT_0 = 4,
    PARTICLE_ANIMATION_TYPE_SPIRAL_OUT_1,

    PARTICLE_ANIMATION_TYPE_RUSH_TO_CENTER = 255,
};

void ov74_0222BA48(SysTask *sysTask, void *particle);

void ov74_0222B3A4(MysteryGiftAnimationManager *animMan);

static int hgasset_ov74_0222B3A4_presetDelays[] __attribute__((section(BWPORT_DATA_SECTION))) = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void ov74_0222B3A4(MysteryGiftAnimationManager *animMan) {
    for (int i = 0; i < 80; i++) {
        animMan->topScreenSmallParticles[i].animationCenter.x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].animationCenter.y = ((fx32)(((192 / 2) > 0) ? ((192 / 2) * (1 << 12) + 0.5f) : ((192 / 2) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].speedToCenter = ((fx32)(((4) > 0) ? ((4) * (1 << 12) + 0.5f) : ((4) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].centerZoneRadius = ((fx32)(((5) > 0) ? ((5) * (1 << 12) + 0.5f) : ((5) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].animationType = PARTICLE_ANIMATION_TYPE_ORBIT_0 + (i % 4);
        animMan->topScreenSmallParticles[i].movementAngle = 0;
        animMan->topScreenSmallParticles[i].timeSinceCenterReached = 0;

        {
            animMan->topScreenSmallParticles[i].speedToCenter = ((fx32)((((i / 10) + 4) > 0) ? (((i / 10) + 4) * (1 << 12) + 0.5f) : (((i / 10) + 4) * (1 << 12) - 0.5f)));
        }
        {
            if (i > 50) {
                animMan->topScreenSmallParticles[i].apparitionDelay = 260 + i + LCRandom() % 5;
            } else if (i > 40) {
                animMan->topScreenSmallParticles[i].apparitionDelay = 250 + i + LCRandom() % 5;
            } else if (i > 30) {
                animMan->topScreenSmallParticles[i].apparitionDelay = 240 + i + LCRandom() % 5;
            } else if (i > 25) {
                animMan->topScreenSmallParticles[i].apparitionDelay = 230 + i + LCRandom() % 5;
            } else if (i > 20) {
                animMan->topScreenSmallParticles[i].apparitionDelay = 220 + i + LCRandom() % 5;
            } else {
                __typeof__(hgasset_ov74_0222B3A4_presetDelays) presetDelays; __builtin_memcpy((void *)&presetDelays, &hgasset_ov74_0222B3A4_presetDelays, sizeof(presetDelays));

                animMan->topScreenSmallParticles[i].apparitionDelay = presetDelays[i];
            }

            animMan->topScreenSmallParticles[i].apparitionDelay = ((animMan->topScreenSmallParticles[i].apparitionDelay + 1) / 2) * 0.7;
            animMan->topScreenSmallParticles[i].apparitionDelay += 1;
        }

        animMan->topScreenSmallParticles[i].speed = 15;
        animMan->topScreenSmallParticles[i].horizontalSpeed = animMan->topScreenSmallParticles[i].speed;
        animMan->topScreenSmallParticles[i].verticalSpeed = animMan->topScreenSmallParticles[i].speed;

        VecFx32 spritePos = *(VecFx32 *)Sprite_GetMatrixPtr(animMan->topScreenSmallParticles[i].sprite);

        spritePos.x = ((fx32)(((16 + (LCRandom() % (256 - (2 * 16)))) > 0) ? ((16 + (LCRandom() % (256 - (2 * 16)))) * (1 << 12) + 0.5f) : ((16 + (LCRandom() % (256 - (2 * 16)))) * (1 << 12) - 0.5f)));
        spritePos.y = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)));

        Sprite_SetMatrix(animMan->topScreenSmallParticles[i].sprite, &spritePos);
        Sprite_SetDrawFlag(animMan->topScreenSmallParticles[i].sprite, 0);

        animMan->topScreenSmallParticles[i].animationSysTask = SysTask_CreateOnMainQueue(ov74_0222BA48, &animMan->topScreenSmallParticles[i], 6);
    }
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_74_thumb_0222B3A4_0222B3A4_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_74_thumb_0222B3A4_0222B3A4.c:hgasset_ov74_0222B3A4_presetDelays", (void *)&hgasset_ov74_0222B3A4_presetDelays, sizeof(hgasset_ov74_0222B3A4_presetDelays), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_74_thumb_0222B3A4_0222B3A4.c:hgasset_ov74_0222B3A4_presetDelays", (void *)&hgasset_ov74_0222B3A4_presetDelays, sizeof(hgasset_ov74_0222B3A4_presetDelays), "u8");
}
