#include "global.h"
#include "sprite_resources_header.h"
#include "filesystem_files_def.h"
#include "battle/battle.h"
#include "overlay_07_ball_rotation.h"

struct BallRotation;

struct BattleContext;

struct BattleScriptTaskData;

struct BattleSystem;

struct CharTransferAllocation;

struct FieldConditions;

struct TextOBJ;

struct ManagedSprite;

struct MoveContestData;

struct MoveEffectsData;

struct MoveSelectMenuData;

struct MoveTable;

struct SelfTurnFlags;

struct SideConditions;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SysTask;

struct SysTaskQueue;

struct TurnFlags;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum BattleContextParam {
    BATTLECTX_SIDE_CONDITIONS_MASK = 0,
    BATTLECTX_SIDE_MIST_TURNS = 1,
    BATTLECTX_SELECTED_PARTY_SLOT = 2,
    BATTLECTX_TOTAL_TURNS = 3,
    BATTLECTX_LEVEL_UP_MONS = 4,
    BATTLECTX_SAFARI_ESCAPE_COUNT = 5,
    BATTLECTX_TOTAL_FAINTED_FOR = 6,
    BATTLECTX_TOTAL_DAMAGE_FOR = 7,
    BATTLECTX_ACTION_FOR = 8,
    BATTLECTX_AICTX_DEFENDER = 9,
    BATTLECTX_SWITCHING_MASK = 10,
    BATTLECTX_AICTX_SELECTED_TARGET = 11,
    BATTLECTX_ACTION_TEMP_FOR = 12,
    BATTLECTX_CONTROL_COMMAND = 13,
    BATTLECTX_NEXT_CONTROL_COMMAND = 14,
};

typedef struct MoveContestData {
    u8 effect;
    u8 type;
    u8 padding02[2];
} MoveContestData;

typedef struct MoveTable {
    u16 effect;
    u8 class;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 effectChance;
    u16 range;
    s8 priority;
    u8 flags;
    MoveContestData contest;
} MoveTable;

typedef struct MoveEffectsData {
    u32 disabledTurns : 3;
    u32 encoredTurns : 3;
    u32 chargedTurns : 2;
    u32 tauntedTurns : 3;
    u32 protectSuccessTurns : 2;
    u32 perishSongTurns : 2;
    u32 rolloutCount : 3;
    u32 furyCutterCount : 3;
    u32 stockpileCount : 3;
    u32 stockpileDefBoosts : 3;
    u32 stockpileSpDefBoosts : 3;
    u32 truant : 1;
    u32 flashFire : 1;

    u32 lockOnTarget : 2;
    u32 mimickedMoveSlot : 4;
    u32 bindTarget : 2;
    u32 meanLookTarget : 2;
    u32 lastResortCount : 3;
    u32 magnetRiseTurns : 3;
    u32 healBlockTurns : 3;
    u32 embargoTurns : 3;
    u32 canUnburden : 1;
    u32 metronomeTurns : 4;
    u32 micleBerry : 1;
    u32 custapBerry : 1;
    u32 quickClaw : 1;
    u32 meFirst : 1;
    u32 padding0004_1F : 1;

    int rechargeTurnNumber;
    int fakeOutTurnNumber;
    int slowStartTurnNumber;
    int meFirstTurnNumber;
    int substituteHP;
    u32 transformedPID;

    u16 disabledMove;
    u16 bindingMove;
    u16 encoredMove;
    u16 encoredMoveSlot;
    u16 lastResortMoves[4];
    u16 choiceLockedMove;
    u16 transformedGender;

    int itemHPRecovery;
} MoveEffectsData;

typedef struct FieldConditions {
    u32 weatherTurns;
    u8 futureSightTurns[4];
    u8 wishTurns[4];
    u16 futureSightMove[4];
    int futureSightAttacker[4];
    s32 futureSightDamage[4];
    u8 wishTarget[4];
} FieldConditions;

typedef struct SideConditions {
    u32 reflectUser : 2;
    u32 reflectTurns : 3;
    u32 lightScreenUser : 2;
    u32 lightScreenTurns : 3;
    u32 mistUser : 2;
    u32 mistTurns : 3;
    u32 safeguardUser : 2;
    u32 safeguardTurns : 3;
    u32 followMe : 1;
    u32 followMeUser : 2;
    u32 knockedOffItemsMask : 6;
    u32 padding00_1D : 3;

    u32 spikesLayers : 2;
    u32 toxicSpikesLayers : 2;
    u32 padding04_04 : 28;
} SideConditions;

typedef struct TurnFlags {
    u32 struggling : 1;
    u32 ppDecremented : 1;
    u32 protecting : 1;
    u32 helpingHand : 1;
    u32 magicCoat : 1;
    u32 snatching : 1;
    u32 roosting : 1;
    u32 fleeing : 2;
    u32 enduring : 1;
    u32 padding00_0A : 22;

    int physicalDamageTakenFrom[4];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4];
    int specialDamageLastAttacker;
    int specialDamageAttackerMask;

    int lastDamageTaken;
    int lastAttacker;
    int assuranceDamageMask;
} TurnFlags;

typedef struct SelfTurnFlags {
    u32 skipPressureCheck : 1;
    u32 lightningRodActivated : 1;
    u32 stormDrainActivated : 1;
    u32 moldBreakerActivated : 1;
    u32 trickRoomActivated : 1;
    u32 focusItemActivated : 1;
    u32 repeatedMoveCount : 3;
    u32 padding00_09 : 23;

    int physicalDamageTaken;
    int physicalDamageLastAttacker;
    int specialDamageTaken;
    int specialDamageLastAttacker;
    int statusFlags;
    int shellBellDamageDealt;
} SelfTurnFlags;

typedef struct BattleScriptTaskData {
    BattleSystem *battleSys;
    BattleContext *battleCtx;
    BallRotation *ballRotation;
    ManagedSprite *sprites[2];
    TextOBJ *fontOAM;
    CharTransferAllocation charTransferAllocation;
    int flag;
    int seqNum;
    int ball;
    int tmpData[8];
    void *tmpPtr[2];
} BattleScriptTaskData;

void SysTask_Destroy(SysTask *task);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

int ov12_022506D4(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, u16 move, BOOL randomize, int inRange);

void ov12_022582B8(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler, int val);

void Heap_Free(void *ptr);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

BattleContext *BattleSystem_GetBattleContext(BattleSystem *battleSys);

u8 BattleSystem_GetFieldSide(BattleSystem *battleSys, int battler);

u32 BattleSystem_GetBattleSpecial(BattleSystem *battleSys);

u16 BattleSystem_Random(BattleSystem *battleSys);

u8 ov10_0221BEF4(BattleSystem *battleSys, u8 battler);

u32 MaskOfFlagNo(int index);

typedef struct MoveSelectMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u16 moves[4];
    u8 ppCur[4];
    u8 ppMax[4];
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 partySlot;
    u8 state;
    u8 unused;
    u16 invalidMoves;
} MoveSelectMenuData;

void BattleController_EmitSelectedMove(BattleSystem *battleSystem, int battler, int command);

void ov12_0225E404(SysTask *task, void *data);

void ov12_0225E404(SysTask *task, void *data) {
    MoveSelectMenuData *moveSelectMenuData = data;
    int action;
    u32 battleType = BattleSystem_GetBattleType(moveSelectMenuData->battleSys);
    BattleContext *battleCtx = BattleSystem_GetBattleContext(moveSelectMenuData->battleSys);

    if ((battleType & ((1 << 0) | (1 << 8)))
        || (BattleSystem_GetBattleSpecial(moveSelectMenuData->battleSys) & (1 << 0))
        || BattleSystem_GetFieldSide(moveSelectMenuData->battleSys, moveSelectMenuData->battler) == 0) {
        action = ov10_0221BEF4(moveSelectMenuData->battleSys, moveSelectMenuData->battler);

        switch (action) {
        case 0xFF:
            return;
            break;
        default:
            action++;
            break;
        }
    } else {
        int validMoves[4];
        int validMovesCount = 0;

        for (int i = 0; i < 4; i++) {
            if ((moveSelectMenuData->invalidMoves & MaskOfFlagNo(i)) == 0) {
                validMoves[validMovesCount] = i + 1;
                validMovesCount++;
            }
        }

        action = validMoves[BattleSystem_Random(moveSelectMenuData->battleSys) % validMovesCount];

        ov12_022582B8(moveSelectMenuData->battleSys, battleCtx, (enum BattleContextParam)(11), moveSelectMenuData->battler, ov12_022506D4(moveSelectMenuData->battleSys, battleCtx, moveSelectMenuData->battler, moveSelectMenuData->moves[action - 1], 1, 0L));
    }

    BattleController_EmitSelectedMove(moveSelectMenuData->battleSys, moveSelectMenuData->battler, action);
    BattleController_EmitClearCommand(moveSelectMenuData->battleSys, moveSelectMenuData->battler, moveSelectMenuData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}
