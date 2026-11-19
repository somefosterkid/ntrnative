#include "global.h"

#include "battle/battler_sprite_context.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "gf_3d_loader.h"
#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_07_ball_rotation.h"

struct UnkBallData;

struct PokemonSprite;

struct PokemonSpriteTransforms;

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

typedef struct PokemonSprite PokemonSprite;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokepicShadow shadow;
    PokepicShadow shadowBackup;
    PokepicAnimScript animFrames[10];
};

typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    PokemonSprite *pokemonSprites[4];
} BattleMonOBJData;

typedef struct SlideTrainerOutData {
    BattleSystem *battleSys;
    OpponentData *battlerData;
    u8 command;
    u8 battler;
    u8 state;
    u8 face;
    int unused;
} SlideTrainerOutData;

void ov12_0225D890(SysTask *task, void *data) {
    SlideTrainerOutData *slideTrainerOutData = data;
    s16 x;
    s16 y;

    switch (slideTrainerOutData->state) {
    case 0:
        if (slideTrainerOutData->face == 2) {
            ManagedSprite_OffsetPositionXY(slideTrainerOutData->battlerData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(slideTrainerOutData->battlerData->managedSprite, &x, &y);

            if (x >= (256 + 40)) {
                slideTrainerOutData->state++;
            }
        } else {
            ManagedSprite_OffsetPositionXY(slideTrainerOutData->battlerData->managedSprite, -5, 0);
            ManagedSprite_GetPositionXY(slideTrainerOutData->battlerData->managedSprite, &x, &y);

            if (x <= -40) {
                slideTrainerOutData->state++;
            }
        }
        break;
    case 1:
        SpriteManager *spriteMan = BattleSystem_GetSpriteManager(slideTrainerOutData->battleSys);
        Sprite_DeleteAndFreeResources(slideTrainerOutData->battlerData->managedSprite);
        slideTrainerOutData->battlerData->managedSprite = 0L;

        SpriteManager_UnloadCharObjById(spriteMan, 20015 + slideTrainerOutData->battlerData->battlerType);
        SpriteManager_UnloadPlttObjById(spriteMan, 20010 + slideTrainerOutData->battlerData->battlerType);
        SpriteManager_UnloadCellObjById(spriteMan, 20007 + slideTrainerOutData->battlerData->battlerType);
        SpriteManager_UnloadAnimObjById(spriteMan, 20007 + slideTrainerOutData->battlerData->battlerType);
        BattleController_EmitClearCommand(slideTrainerOutData->battleSys, slideTrainerOutData->battler, slideTrainerOutData->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}

typedef struct SlideTrainerInData {
    BattleSystem *battleSys;
    ManagedSprite *managedSprite;
    u8 command;
    u8 battler;
    u8 state;
    u8 face;
    s16 x;
    u16 unused;
} SlideTrainerInData;

void ov12_0225D990(SysTask *task, void *data) {
    SlideTrainerInData *slideTrainerInData = data;
    s16 x, y;

    switch (slideTrainerInData->state) {
    case 0:
        ManagedSprite_GetPositionXY(slideTrainerInData->managedSprite, &x, &y);

        if (slideTrainerInData->face == 0) {
            x += 5;

            if (x >= slideTrainerInData->x) {
                x = slideTrainerInData->x;
                slideTrainerInData->state++;
            }
        } else {
            x -= 5;

            if (x <= slideTrainerInData->x) {
                x = slideTrainerInData->x;
                slideTrainerInData->state++;
            }
        }

        ManagedSprite_SetPositionXY(slideTrainerInData->managedSprite, x, y);
        break;
    case 1:
        BattleController_EmitClearCommand(slideTrainerInData->battleSys, slideTrainerInData->battler, slideTrainerInData->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}

enum HealthBoxScrollDirection {
    HEALTHBOX_SCROLL_IN = 0,
    HEALTHBOX_SCROLL_OUT,
};

void BattleHpBar_SetEnabled(BattleHpBar *healthbox, BOOL enable);

void ov12_02264FB0(BattleHpBar *healthbox, enum HealthBoxScrollDirection direction);

enum SlideHealthBoxInState {
    SLIDE_HEALTHBOX_IN_STATE_ENABLE = 0,
    SLIDE_HEALTHBOX_IN_STATE_WAIT,
    SLIDE_HEALTHBOX_IN_STATE_DONE,
};

void ov12_0225DA18(SysTask *task, void *data) {
    BattleHpBar *healthbox = data;

    switch (healthbox->script) {
    case SLIDE_HEALTHBOX_IN_STATE_ENABLE:
        if (healthbox->unk4D > 0) {
            healthbox->unk4D--;
            break;
        }

        ov12_02264FB0(healthbox, HEALTHBOX_SCROLL_IN);
        BattleHpBar_SetEnabled(healthbox, TRUE);
        healthbox->script++;
        break;

    case SLIDE_HEALTHBOX_IN_STATE_WAIT:
        if (healthbox->unk_4F_1 == TRUE) {
            healthbox->script++;
        }
        break;

    default:
        BattleController_EmitClearCommand(healthbox->battleSystem, healthbox->battlerId, healthbox->unk4C);
        healthbox->unk10 = NULL;
        SysTask_Destroy(task);
        break;
    }
}
