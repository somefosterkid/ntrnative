#include "global.h"

#include "constants/battle.h"
#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "msgdata.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_12_battle_message_wait_task.h"

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

MsgData *BattleSystem_GetMessageLoader(BattleSystem *battleSys);

void ov12_022605D0(SysTask *task, void *data);

void ov12_022619E4(BattleSystem *battleSys, OpponentData *battlerData, EscapeMsgMessage *message, BattleMessage *battleMsg);

void ov12_0225B2F8(BattleSystem *battleSys, OpponentData *battlerData, EscapeMsgMessage *message);

void ov12_0225B2F8(BattleSystem *battleSys, OpponentData *battlerData, EscapeMsgMessage *message) {
    BattleMessageWaitTask *waitTask;
    MsgData *msgLoader;
    BattleMessage battleMsg;

    ov12_022619E4(battleSys, battlerData, message, &battleMsg);

    msgLoader = BattleSystem_GetMessageLoader(battleSys);
    waitTask = (BattleMessageWaitTask *)Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleMessageWaitTask));

    waitTask->battleSys = battleSys;
    waitTask->command = BATTLE_COMMAND_PRINT_ESCAPE_MESSAGE;
    waitTask->battler = battlerData->battler;
    waitTask->msgIdx = BattleSystem_PrintBattleMessage(battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(battleSys));

    SysTask_CreateOnMainQueue(ov12_022605D0, waitTask, 0);
}
