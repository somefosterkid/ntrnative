#ifndef POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_ANIMATION_H
#define POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_ANIMATION_H

// The Mystery Gift receive animation (ov74_0222C014 sets it up; the particles gather, implode and spiral out):
// the one definition, with its DS layout (0x30A4 bytes; spriteMan at 0xC, its resource managers at spriteMan + 0x12C).

#include "overlay_74_main_menu_util.h"
#include "sprite_resources_header.h"
#include "struct_defs/resdat_narc_structs.h"

struct Sprite;
struct SpriteResource;
struct SysTask;

enum MysteryGiftAnimationStatus {
    MG_ANIMATION_STATUS_DONE = 0,
    MG_ANIMATION_STATUS_BEGIN,
    MG_ANIMATION_STATUS_PROCEED_IMPLOSION
};

typedef struct MysteryGiftParticle {
    int unused_00;
    int timeSinceCenterReached;
    BOOL animationDone;
    struct Sprite *sprite;
    VecFx32 animationCenter;
    fx32 speedToCenter;
    fx32 centerZoneRadius;
    int inFinalAnimationPhase;
    int angleAroundCenter;
    int animationType;
    int apparitionDelay;
    int movementAngle;
    int speed;
    int spiralOutAcceleration;
    int horizontalSpeed;
    int verticalSpeed;
    struct SysTask *animationSysTask;
} MysteryGiftParticle;

typedef struct MysteryGiftAnimationManager {
    int animationStage;
    int unused_04;
    s8 blendBrightness;
    MainMenuSpriteManager *spriteMan;
    SpriteResourcesHeader spriteResourcesHeaders[2];
    struct SpriteResource *spriteResources[2][GF_GFX_RES_TYPE_MAX];
    MysteryGiftParticle topScreenSmallParticles[80];
    MysteryGiftParticle bottomScreenSmallParticles[80];
    MysteryGiftParticle topScreenLargeParticle;
    MysteryGiftParticle bottomScreenLargeParticle;
    u32 *animationStatusPtr; // MysteryGiftAppData.animationStatus, holds enum MysteryGiftAnimationStatus values
} MysteryGiftAnimationManager;

#ifdef __MWERKS__
typedef char MysteryGiftParticle_size_check[(sizeof(MysteryGiftParticle) == 0x4C) ? 1 : -1];
typedef char MysteryGiftAnimationManager_spriteMan_check[(offsetof(MysteryGiftAnimationManager, spriteMan) == 0xC) ? 1 : -1];
typedef char MysteryGiftAnimationManager_spriteResources_check[(offsetof(MysteryGiftAnimationManager, spriteResources) == 0x58) ? 1 : -1];
typedef char MysteryGiftAnimationManager_animationStatusPtr_check[(offsetof(MysteryGiftAnimationManager, animationStatusPtr) == 0x30A0) ? 1 : -1];
typedef char MysteryGiftAnimationManager_size_check[(sizeof(MysteryGiftAnimationManager) == 0x30A4) ? 1 : -1];
typedef char MainMenuSpriteManager_resourceCollections_check[(offsetof(MainMenuSpriteManager, resourceCollections) == 0x12C) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_ANIMATION_H
