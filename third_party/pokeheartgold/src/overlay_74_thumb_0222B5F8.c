#include "global.h"
#include "sprite_resources_header.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_74_mystery_gift_animation.h"

struct Sprite;
struct SysTask;
struct SysTaskQueue;
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
fx32 GF_SinDeg(u16 degrees);
fx32 GF_CosDeg(u16 degrees);
u16 LCRandom(void);
SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);
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
typedef struct Vec2F32 {
    f32 x;
    f32 y;
} Vec2F32;
void Sprite_SetAnimationFrame(Sprite *sprite, u16 frame);
static BOOL ov74_0222B950(s16 centerX, s16 centerY, f32 particleX, f32 particleY, f32 *outX, f32 *outY, f32 length, s16 minDistanceToCenter);

void ov74_0222B5F8(MysteryGiftAnimationManager *animMan);
void ov74_0222B760(MysteryGiftAnimationManager *animMan);
static BOOL ov74_0222B950(s16 centerX, s16 centerY, f32 particleX, f32 particleY, f32 *outX, f32 *outY, f32 length, s16 minDistanceToCenter);
void ov74_0222BA48(SysTask *sysTask, void *particle);
void ov74_0222BED8(MysteryGiftAnimationManager *animMan);
void ov74_0222BEF0(MysteryGiftAnimationManager *animMan);

void ov74_0222B5F8(MysteryGiftAnimationManager *animMan) {
    VecFx32 spritePos;

    for (int i = 0; i < 80; i++) {
        if (animMan->topScreenSmallParticles[i].sprite == 0L) {
            continue;
        }

        animMan->topScreenSmallParticles[i].animationCenter.x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].animationCenter.y = ((fx32)(((192 / 2) > 0) ? ((192 / 2) * (1 << 12) + 0.5f) : ((192 / 2) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].speedToCenter = ((fx32)(((12) > 0) ? ((12) * (1 << 12) + 0.5f) : ((12) * (1 << 12) - 0.5f)));
        animMan->topScreenSmallParticles[i].centerZoneRadius = ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f)));

        animMan->topScreenSmallParticles[i].animationType = PARTICLE_ANIMATION_TYPE_RUSH_TO_CENTER;
        animMan->topScreenSmallParticles[i].movementAngle = 0;
        animMan->topScreenSmallParticles[i].timeSinceCenterReached = 0;
        animMan->topScreenSmallParticles[i].animationDone = 0;

        if (i > 30) {
            animMan->topScreenSmallParticles[i].apparitionDelay = 15 + (LCRandom() % 10);
        } else if (i > 20) {
            animMan->topScreenSmallParticles[i].apparitionDelay = 10 + (LCRandom() % 10);
        } else if (i > 10) {
            animMan->topScreenSmallParticles[i].apparitionDelay = 10 + (LCRandom() % 5);
        } else {
            animMan->topScreenSmallParticles[i].apparitionDelay = 5 + (LCRandom() % 5);
        }

        animMan->topScreenSmallParticles[i].apparitionDelay = 0;
        animMan->topScreenSmallParticles[i].horizontalSpeed = animMan->topScreenSmallParticles[i].speed;
        animMan->topScreenSmallParticles[i].verticalSpeed = animMan->topScreenSmallParticles[i].speed;

        int angle = LCRandom() % 360;
        int distance = 64 + (LCRandom() % 32);

        spritePos.x = animMan->topScreenSmallParticles[i].animationCenter.x + (GF_SinDeg(angle) * distance);
        spritePos.y = animMan->topScreenSmallParticles[i].animationCenter.y + (GF_CosDeg(angle) * distance);

        Sprite_SetMatrix(animMan->topScreenSmallParticles[i].sprite, &spritePos);

        Sprite_TryChangeAnimSeq(animMan->topScreenSmallParticles[i].sprite, 1);
        Sprite_SetDrawFlag(animMan->topScreenSmallParticles[i].sprite, 0);

        animMan->topScreenSmallParticles[i].animationSysTask = SysTask_CreateOnMainQueue(ov74_0222BA48, &animMan->topScreenSmallParticles[i], 6);
    }
}

void ov74_0222B760(MysteryGiftAnimationManager *animMan) {
    int i;
    VecFx32 spritePos;

    for (i = 0; i < 80; i++) {
        animMan->bottomScreenSmallParticles[i].animationCenter.x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
        animMan->bottomScreenSmallParticles[i].animationCenter.y = ((fx32)(((384) > 0) ? ((384) * (1 << 12) + 0.5f) : ((384) * (1 << 12) - 0.5f)));
        animMan->bottomScreenSmallParticles[i].speedToCenter = ((fx32)(((4) > 0) ? ((4) * (1 << 12) + 0.5f) : ((4) * (1 << 12) - 0.5f)));
        animMan->bottomScreenSmallParticles[i].centerZoneRadius = ((fx32)(((5) > 0) ? ((5) * (1 << 12) + 0.5f) : ((5) * (1 << 12) - 0.5f)));
        animMan->bottomScreenSmallParticles[i].animationType = PARTICLE_ANIMATION_TYPE_SPIRAL_OUT_0 + (i % 2);
        animMan->bottomScreenSmallParticles[i].movementAngle = 0;
        animMan->bottomScreenSmallParticles[i].timeSinceCenterReached = 0;
        animMan->bottomScreenSmallParticles[i].spiralOutAcceleration = 2 + (LCRandom() % 4);
        animMan->bottomScreenSmallParticles[i].speedToCenter = ((fx32)((((i / 30) + 4) > 0) ? (((i / 30) + 4) * (1 << 12) + 0.5f) : (((i / 30) + 4) * (1 << 12) - 0.5f)));

        if (i > 60) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 70 + (LCRandom() % 10);
        } else if (i > 50) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 60 + (LCRandom() % 10);
        } else if (i > 40) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 50 + (LCRandom() % 5);
        } else if (i > 30) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 40 + (LCRandom() % 10);
        } else if (i > 20) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 30 + (LCRandom() % 10);
        } else if (i > 10) {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 20 + (LCRandom() % 5);
        } else {
            animMan->bottomScreenSmallParticles[i].speed = 5;
            animMan->bottomScreenSmallParticles[i].apparitionDelay = 10 + (LCRandom() % 5);
        }

        animMan->bottomScreenSmallParticles[i].horizontalSpeed = animMan->bottomScreenSmallParticles[i].speed;
        animMan->bottomScreenSmallParticles[i].verticalSpeed = animMan->bottomScreenSmallParticles[i].speed;

        spritePos = *(VecFx32 *)Sprite_GetMatrixPtr(animMan->bottomScreenSmallParticles[i].sprite);

        spritePos.x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
        spritePos.y = ((fx32)(((384) > 0) ? ((384) * (1 << 12) + 0.5f) : ((384) * (1 << 12) - 0.5f)));

        Sprite_SetMatrix(animMan->bottomScreenSmallParticles[i].sprite, &spritePos);
        Sprite_SetDrawFlag(animMan->bottomScreenSmallParticles[i].sprite, 1);

        animMan->bottomScreenSmallParticles[i].animationSysTask = SysTask_CreateOnMainQueue(ov74_0222BA48, &animMan->bottomScreenSmallParticles[i], 6);
    }
}

static BOOL ov74_0222B950(s16 centerX, s16 centerY, f32 particleX, f32 particleY, f32 *outX, f32 *outY, f32 length, s16 minDistanceToCenter) {
    Vec2F32 outVec, vecToCenter, zeroVec;

    zeroVec.x = 0;
    zeroVec.y = 0;

    vecToCenter.x = (centerX - particleX);
    vecToCenter.y = (centerY - particleY);

    outVec.x = 0;
    outVec.y = 0;

    f32 distSquared = (vecToCenter.x * vecToCenter.x) + (vecToCenter.y * vecToCenter.y);

    f32 dist = ((f32)((FX_Sqrt((fx32)(((distSquared) > 0) ? ((distSquared) * (1 << 12) + 0.5f) : ((distSquared) * (1 << 12) - 0.5f)))) / (f32)(1 << 12)));

    if (dist < length || minDistanceToCenter > dist || dist == 0) {
        return 0;
    }

    outVec.x = (vecToCenter.x * length) / dist;
    outVec.y = (vecToCenter.y * length) / dist;

    *outX = (outVec.x + zeroVec.x);
    *outY = (outVec.y + zeroVec.y);

    return 1;
}

void ov74_0222BA48(SysTask *sysTask, void *ptr) {
    MysteryGiftParticle *particle = ptr;
    if (particle->apparitionDelay) {
        Sprite_SetAnimationFrame(particle->sprite, 0);
        particle->apparitionDelay--;
        return;
    } else {
        Sprite_SetDrawFlag(particle->sprite, 1);
    }

    f32 towardsCenterX, towardsCenterY;

    VecFx32 spritePos = *(VecFx32 *)Sprite_GetMatrixPtr(particle->sprite);
    BOOL hasNotReachedCenter = ov74_0222B950(particle->animationCenter.x >> 12, particle->animationCenter.y >> 12, ((f32)((spritePos.x) / (f32)(1 << 12))), ((f32)((spritePos.y) / (f32)(1 << 12))), &towardsCenterX, &towardsCenterY, ((f32)((particle->speedToCenter) / (f32)(1 << 12))), ((f32)((particle->centerZoneRadius) / (f32)(1 << 12))));

    if (hasNotReachedCenter && particle->inFinalAnimationPhase == 0) {
        spritePos.x += ((fx32)(((towardsCenterX) > 0) ? ((towardsCenterX) * (1 << 12) + 0.5f) : ((towardsCenterX) * (1 << 12) - 0.5f)));
        spritePos.y += ((fx32)(((towardsCenterY) > 0) ? ((towardsCenterY) * (1 << 12) + 0.5f) : ((towardsCenterY) * (1 << 12) - 0.5f)));

        Sprite_SetMatrix(particle->sprite, &spritePos);
    } else {
        switch (particle->inFinalAnimationPhase) {
        case 0:
            particle->angleAroundCenter = FX_Atan2Idx(((fx32)(((particle->animationCenter.y - spritePos.y) > 0) ? ((particle->animationCenter.y - spritePos.y) * (1 << 12) + 0.5f) : ((particle->animationCenter.y - spritePos.y) * (1 << 12) - 0.5f))), ((fx32)(((particle->animationCenter.x - spritePos.x) > 0) ? ((particle->animationCenter.x - spritePos.x) * (1 << 12) + 0.5f) : ((particle->animationCenter.x - spritePos.x) * (1 << 12) - 0.5f))));
            particle->inFinalAnimationPhase++;
            break;
        case 1:
            if (particle->timeSinceCenterReached < 20) {
                particle->timeSinceCenterReached++;
            } else {
                particle->animationDone = 1;
            }

            particle->angleAroundCenter += 8;

            if (particle->animationType == PARTICLE_ANIMATION_TYPE_ORBIT_0) {
                particle->movementAngle += 8;
                particle->movementAngle %= 360;
                particle->horizontalSpeed = (GF_CosDeg(particle->movementAngle) * particle->speed) >> 12;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * +1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * +1);
            } else if (particle->animationType == PARTICLE_ANIMATION_TYPE_ORBIT_1) {
                particle->movementAngle += 8;
                particle->movementAngle %= 360;
                particle->verticalSpeed = (GF_SinDeg(particle->movementAngle) * particle->speed) >> 12;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * +1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * +1);
            } else if (particle->animationType == PARTICLE_ANIMATION_TYPE_ORBIT_2) {
                particle->movementAngle += 8;
                particle->movementAngle %= 360;
                particle->verticalSpeed = (GF_SinDeg(particle->movementAngle) * particle->speed) >> 12;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * -1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * -1);
            } else if (particle->animationType == PARTICLE_ANIMATION_TYPE_ORBIT_3) {
                particle->movementAngle += 8;
                particle->movementAngle %= 360;
                particle->horizontalSpeed = (GF_CosDeg(particle->movementAngle) * particle->speed) >> 12;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * -1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * -1);
            } else if (particle->animationType == PARTICLE_ANIMATION_TYPE_SPIRAL_OUT_0) {
                if (particle->speed < 96) {
                    particle->speed += particle->spiralOutAcceleration;
                }

                particle->horizontalSpeed = particle->speed;
                particle->verticalSpeed = particle->speed;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * +1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * +1);
            } else if (particle->animationType == PARTICLE_ANIMATION_TYPE_SPIRAL_OUT_1) {
                if (particle->speed < 96) {
                    particle->speed += particle->spiralOutAcceleration;
                }

                particle->horizontalSpeed = particle->speed;
                particle->verticalSpeed = particle->speed;

                spritePos.x = particle->animationCenter.x + (GF_SinDeg(particle->angleAroundCenter) * particle->horizontalSpeed * -1);
                spritePos.y = particle->animationCenter.y + (GF_CosDeg(particle->angleAroundCenter) * particle->verticalSpeed * -1);
            } else {
                particle->timeSinceCenterReached = 10;
                particle->speedToCenter += ((fx32)(((0.5) > 0) ? ((0.5) * (1 << 12) + 0.5f) : ((0.5) * (1 << 12) - 0.5f)));

                hasNotReachedCenter = ov74_0222B950(particle->animationCenter.x >> 12, particle->animationCenter.y >> 12, ((f32)((spritePos.x) / (f32)(1 << 12))), ((f32)((spritePos.y) / (f32)(1 << 12))), &towardsCenterX, &towardsCenterY, ((f32)((particle->speedToCenter) / (f32)(1 << 12))), ((f32)((0) / (f32)(1 << 12))));

                if (hasNotReachedCenter) {
                    spritePos.x += ((fx32)(((towardsCenterX) > 0) ? ((towardsCenterX) * (1 << 12) + 0.5f) : ((towardsCenterX) * (1 << 12) - 0.5f)));
                    spritePos.y += ((fx32)(((towardsCenterY) > 0) ? ((towardsCenterY) * (1 << 12) + 0.5f) : ((towardsCenterY) * (1 << 12) - 0.5f)));

                    Sprite_SetMatrix(particle->sprite, &spritePos);
                } else {
                    particle->animationDone = 1;
                }
            }

            particle->angleAroundCenter %= 360;
            Sprite_SetMatrix(particle->sprite, &spritePos);
            break;
        }
    }
}

void ov74_0222BED8(MysteryGiftAnimationManager *animMan) {
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BD, animMan->blendBrightness);
}

void ov74_0222BEF0(MysteryGiftAnimationManager *animMan) {
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BD, animMan->blendBrightness);
}
