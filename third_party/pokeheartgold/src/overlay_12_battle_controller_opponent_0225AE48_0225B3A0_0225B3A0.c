#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "battle/battle.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_07_ball_rotation.h"
#include "battle/battle_message_structs.h"

struct UnkBallData;

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

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler);

void ov12_0225B3A0(BattleSystem *battleSys, OpponentData *battlerData, MoveAnimation *animation);

void ov12_0225B3A0(BattleSystem *battleSys, OpponentData *battlerData, MoveAnimation *animation) {
    BattlerSpriteContext battlerSpriteCtx;

    ov12_02261CA8(battleSys, animation, &battlerSpriteCtx, battlerData->battler);
    ov07_02234A20(&battlerSpriteCtx, HEAP_ID_BATTLE);
    BattleController_EmitClearCommand(battleSys, battlerData->battler, animation->command);
}
