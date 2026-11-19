#include "global.h"

#include "battle/battler_sprite_context.h"

#include "battle/battle_controller.h"

#include "heap.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task_api.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_12_sprite_to_oamdata.h"

struct BallRotation;

struct BattleMonOBJData;

struct PokemonSprite;

struct PokemonSpriteTransforms;

typedef struct HealthBox {
    u8 state;
    ManagedSprite *mainSprite;
    ManagedSprite *arrowSprite;
    BattleSystem *battleSys;
    SysTask *task_10;
    Window unused_14;
    u8 battler;
    u8 type;
    u8 selectedPartySlot;
    u8 numSafariBalls;
    s32 curHP;
    s32 maxHP;
    s32 damage;
    s32 hpTemp;
    s32 curExp;
    s32 maxExp;
    s32 expReward;
    s32 expTemp;
    u8 level;
    u8 gender;
    u8 status;
    u8 caughtSpecies;
    u8 command;
    u8 delay;
    u8 expSoundTimer;
    u8 scrollDirection : 1;
    u8 doneScrolling : 1;
    u8 unused_4F_2 : 1;
    u8 numberMode : 1;
    u8 padding : 4;
    SysTask *bobbingAnimTask;
    u16 degrees;
} HealthBox;

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

typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;

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
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
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

void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler);

void ov12_0225B200(BattleSystem *battleSys, OpponentData *battlerData, MoveAnimation *animation);

void ov12_0225B200(BattleSystem *battleSys, OpponentData *battlerData, MoveAnimation *animation) {
    BattlerSpriteContext battlerSpriteCtx;

    ov12_02261CA8(battleSys, animation, &battlerSpriteCtx, battlerData->battler);
    ov07_0223494C(&battlerSpriteCtx, HEAP_ID_BATTLE);
    BattleController_EmitClearCommand(battleSys, battlerData->battler, animation->command);
}

void ov12_02260CDC(SysTask *task, void *data);

void ov12_0225B234(BattleSystem *battleSys, OpponentData *battlerData);

void ov12_0225B234(BattleSystem *battleSys, OpponentData *battlerData) {
    SpriteToOAMData *spriteToOAMData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SpriteToOAMData));

    spriteToOAMData->state = 0;
    spriteToOAMData->battleSys = battleSys;
    spriteToOAMData->battlerData = battlerData;
    spriteToOAMData->command = battlerData->data[0];
    spriteToOAMData->battler = battlerData->battler;
    spriteToOAMData->monSprite = battlerData->monSprite;

    SysTask_CreateOnMainQueue(ov12_02260CDC, spriteToOAMData, 0);
}

void ov12_02260D28(SysTask *task, void *data);

void ov12_0225B26C(BattleSystem *battleSys, OpponentData *battlerData);

void ov12_0225B26C(BattleSystem *battleSys, OpponentData *battlerData) {
    SpriteToOAMData *spriteToOAMData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SpriteToOAMData));

    spriteToOAMData->state = 0;
    spriteToOAMData->battleSys = battleSys;
    spriteToOAMData->battlerData = battlerData;
    spriteToOAMData->command = battlerData->data[0];
    spriteToOAMData->battler = battlerData->battler;
    spriteToOAMData->monSprite = battlerData->monSprite;

    SysTask_CreateOnMainQueue(ov12_02260D28, spriteToOAMData, 0);
}
