#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_07_ball_rotation.h"

struct BallRotation;
struct BattleMonOBJData;
struct PokemonSprite;
struct PokemonSpriteTransforms;
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
typedef struct BattleMonOBJData BattleMonOBJData;
typedef struct SavedCursorPosition {
    s8 actionMenuX;
    s8 actionMenuY;
    s8 moveMenuX;
    s8 moveMenuY;
    s8 targetSelectX;
    s8 targetSelectY;
    s8 targetingLayout;
    u8 padding;
} SavedCursorPosition;
typedef struct BattlerDataSysTaskFuncs {
    SysTaskFunc setCommandSelection;
    SysTaskFunc showMoveSelectMenu;
    SysTaskFunc showTargetSelectMenu;
    SysTaskFunc showBagMenu;
    SysTaskFunc showPartyMenu;
    SysTaskFunc showYesNoMenu;
} BattlerDataSysTaskFuncs;
void ov12_0225D890(SysTask *task, void *data);
typedef struct SlideTrainerOutData {
    BattleSystem *battleSys;
    OpponentData *battlerData;
    u8 command;
    u8 battler;
    u8 state;
    u8 face;
    int unused;
} SlideTrainerOutData;

void ov12_0225A334(BattleSystem *battleSys, OpponentData *battlerData);

void ov12_0225A334(BattleSystem *battleSys, OpponentData *battlerData) {
    SlideTrainerOutData *slideTrainerOutData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SlideTrainerOutData));

    slideTrainerOutData->state = 0;
    slideTrainerOutData->battleSys = battleSys;
    slideTrainerOutData->command = battlerData->data[0];
    slideTrainerOutData->battler = battlerData->battler;
    slideTrainerOutData->battlerData = battlerData;

    if (battlerData->battlerType & 1) {
        slideTrainerOutData->face = 2;
    } else {
        slideTrainerOutData->face = 0;
    }

    SysTask_CreateOnMainQueue(ov12_0225D890, slideTrainerOutData, 0);
}
