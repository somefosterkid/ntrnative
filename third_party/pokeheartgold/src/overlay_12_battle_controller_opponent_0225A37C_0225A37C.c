#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_74_main_menu_util.h"
#include "battle/battle_message_structs.h"

struct UnkBallData;

struct BattleMonOBJData;

struct BattleSystem;

struct OpponentData;

struct OpponentDataSysTaskFuncs;

struct BgConfig;

struct BattleHpBar;

struct ManagedSprite;

struct NARC;

struct PokemonSprite;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct OpponentDataCursorPos;

struct SlideTrainerInData;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskManager;

struct TrainerSlideInMessage;

struct Window;

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

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
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
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
    SpriteAnimFrame animFrames[10];
};

typedef struct BattleMonOBJData BattleMonOBJData;

extern const s16 ov07_022377DC[][2];

extern const s16 ov07_02237818[][3];

void ov12_0225D990(SysTask *task, void *data);

ManagedSprite *ov12_02261B2C(BattleSystem *battleSys, int side, int trainerClass, int battlerType, int unk, s16 x, s16 y);

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

void ov12_0225A37C(BattleSystem *battleSys, OpponentData *battlerData, TrainerSlideInMessage *message);

void ov12_0225A37C(BattleSystem *battleSys, OpponentData *battlerData, TrainerSlideInMessage *message) {
    PokepicManager *unused = (struct PokepicManager *)(BattleSystem_GetPokepicManager(battleSys));
    SlideTrainerInData *slideTrainerInData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SlideTrainerInData));
    slideTrainerInData->state = 0;

    if (battlerData->battlerType & 1) {
        slideTrainerInData->face = 2;
    } else {
        slideTrainerInData->face = 0;
    }

    slideTrainerInData->managedSprite = battlerData->managedSprite = ov12_02261B2C(battleSys,
        battlerData->battlerType & 1,
        message->trainerType,
        battlerData->battlerType,
        0,
        ov07_02237818[battlerData->battlerType & 1][0],
        ov07_02237818[battlerData->battlerType & 1][1]);
    slideTrainerInData->x = ov07_022377DC[battlerData->battlerType & 1][message->posIn];
    slideTrainerInData->battleSys = battleSys;
    slideTrainerInData->command = message->command;
    slideTrainerInData->battler = battlerData->battler;

    SysTask_CreateOnMainQueue(ov12_0225D990, slideTrainerInData, 0);
}
