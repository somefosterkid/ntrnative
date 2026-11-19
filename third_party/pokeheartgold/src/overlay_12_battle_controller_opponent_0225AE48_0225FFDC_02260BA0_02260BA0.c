#include "global.h"

#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"

struct BattleSystem;

struct SysTask;

struct SysTaskManager;

typedef struct BattleSystem BattleSystem;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
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

void SysTask_Destroy(SysTask *task);

enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};

enum ShowPartyGaugeType {
    SHOW_PARTY_GAUGE_BATTLE_START,
    SHOW_PARTY_GAUGE_MID_BATTLE,
};

enum HidePartyGaugeType {
    HIDE_PARTY_GAUGE_BATTLE_START,
    HIDE_PARTY_GAUGE_MID_BATTLE,
};

enum PartyGaugePosition {
    PARTY_GAUGE_POSITION_HIGH,
    PARTY_GAUGE_POSITION_MIDDLE,
    PARTY_GAUGE_POSITION_LOW,
};

enum HideArrowType {
    HIDE_ARROW_FADE_AND_SCROLL,
    HIDE_ARROW_FADE_IN_PLACE,
};

typedef struct {
    ManagedSprite *managedSprite;
    SysTask *task;
    enum HideArrowType hideType;
    enum PartyGaugeSide side;
    enum PartyGaugePosition position;
    s32 x;
    s16 alpha;
    u8 state;
    u8 delay;
} PartyGaugeArrow;

typedef struct {
    ManagedSprite *managedSprite;
    SysTask *task;
    enum PartyGaugeSide side;
    enum PartyGaugePosition position;
    enum HidePartyGaugeType hideType;
    s8 *pokeballCount;
    s16 *arrowAlpha;
    s32 xStart;
    s32 xEnd;
    s32 xOverflow;
    s16 delay;
    u16 sdatID;
    u8 state;
    u8 ballSlot;
    u8 flipAnimation;
    u8 startDelay;
} PartyGaugePokeballs;

typedef struct PartyGauge {
    PartyGaugeArrow arrow;
    PartyGaugePokeballs pokeballs[6];
    s8 pokeballCount;
} PartyGauge;

extern PartyGauge *PartyGauge_NewAndShow(u8 ballStatus[], enum PartyGaugeSide side, enum ShowPartyGaugeType showType, enum PartyGaugePosition pos, SpriteSystem *spriteSys, SpriteManager *spriteMan);

BOOL PartyGauge_IsArrowTaskFinished(PartyGauge *gauge);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

SpriteManager *BattleSystem_GetSpriteManager(BattleSystem *battleSys);

PartyGauge *ov12_0223A908(BattleSystem *battleSys, enum PartyGaugeSide partyGaugeSide);

void ov12_0223A914(BattleSystem *battleSys, enum PartyGaugeSide partyGaugeSide, PartyGauge *partyGauge);

u8 BattleSystem_GetFieldSide(BattleSystem *battleSys, int battler);

typedef struct PartyGaugeTask {
    BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    u8 status[6];
    u8 midBattle;
} PartyGaugeTask;

void ov12_02260BA0(SysTask *task, void *data);

void ov12_02260BA0(SysTask *task, void *data) {
    PartyGaugeTask *gaugeTask = (PartyGaugeTask *)data;
    enum PartyGaugeSide side;

    if (BattleSystem_GetFieldSide(gaugeTask->battleSys, gaugeTask->battler) == 0) {
        side = PARTY_GAUGE_OURS;
    } else {
        side = PARTY_GAUGE_THEIRS;
    }

    switch (gaugeTask->state) {
    case 0: {
        enum ShowPartyGaugeType showType;
        enum PartyGaugePosition gaugePosition;

        if (gaugeTask->midBattle == 0) {
            showType = SHOW_PARTY_GAUGE_BATTLE_START;
            gaugePosition = PARTY_GAUGE_POSITION_MIDDLE;
        } else {
            showType = SHOW_PARTY_GAUGE_MID_BATTLE;

            switch (gaugeTask->battlerType) {
            case 3:
                gaugePosition = PARTY_GAUGE_POSITION_HIGH;
                break;
            case 5:
                gaugePosition = PARTY_GAUGE_POSITION_LOW;
                break;
            default:
                gaugePosition = PARTY_GAUGE_POSITION_MIDDLE;
                break;
            }
        }

        PartyGauge *gauge = PartyGauge_NewAndShow(gaugeTask->status, side, showType, gaugePosition, BattleSystem_GetSpriteSystem(gaugeTask->battleSys), BattleSystem_GetSpriteManager(gaugeTask->battleSys));
        ov12_0223A914(gaugeTask->battleSys, side, gauge);
    }
        gaugeTask->state++;
        break;
    case 1:
        if (PartyGauge_IsArrowTaskFinished(ov12_0223A908(gaugeTask->battleSys, side)) == 1) {
            gaugeTask->state++;
        }
        break;
    default:
        BattleController_EmitClearCommand(gaugeTask->battleSys, gaugeTask->battler, gaugeTask->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}

void PartyGauge_StartHideTask(PartyGauge *gauge, enum HideArrowType hideArrowType, enum HidePartyGaugeType hideGaugeType);

BOOL PartyGauge_IsHideTaskFinished(PartyGauge *gauge);

void PartyGauge_DeleteAndFreeResources(PartyGauge *gauge);

void ov12_02260C58(SysTask *task, void *data);

void ov12_02260C58(SysTask *task, void *data) {
    PartyGaugeTask *gaugeTask = (PartyGaugeTask *)data;
    enum PartyGaugeSide side;
    enum HideArrowType hideArrowType;
    enum HidePartyGaugeType hideGaugeType;

    if (BattleSystem_GetFieldSide(gaugeTask->battleSys, gaugeTask->battler) == 0) {
        side = PARTY_GAUGE_OURS;
    } else {
        side = PARTY_GAUGE_THEIRS;
    }

    PartyGauge *gauge = ov12_0223A908(gaugeTask->battleSys, side);

    switch (gaugeTask->state) {
    case 0:
        if (gaugeTask->midBattle == 0) {
            hideArrowType = HIDE_ARROW_FADE_AND_SCROLL;
            hideGaugeType = HIDE_PARTY_GAUGE_BATTLE_START;
        } else {
            hideArrowType = HIDE_ARROW_FADE_IN_PLACE;
            hideGaugeType = HIDE_PARTY_GAUGE_MID_BATTLE;
        }

        PartyGauge_StartHideTask(gauge, hideArrowType, hideGaugeType);
        gaugeTask->state++;
        break;
    case 1:
        if (PartyGauge_IsHideTaskFinished(gauge) == 1) {
            PartyGauge_DeleteAndFreeResources(gauge);
            ov12_0223A914(gaugeTask->battleSys, side, 0L);
            gaugeTask->state++;
        }
        break;
    default:
        BattleController_EmitClearCommand(gaugeTask->battleSys, gaugeTask->battler, gaugeTask->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}
