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

struct CommandSetData;

struct FieldConditions;

struct TextOBJ;

struct Fraction;

struct ManagedSprite;

struct MoveContestData;

struct MoveEffectsData;

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

enum BattleMonParam {
    BATTLEMON_SPECIES = 0,
    BATTLEMON_ATTACK = 1,
    BATTLEMON_DEFENSE = 2,
    BATTLEMON_SPEED = 3,
    BATTLEMON_SP_ATTACK = 4,
    BATTLEMON_SP_DEFENSE = 5,
    BATTLEMON_MOVE_1 = 6,
    BATTLEMON_MOVE_2 = 7,
    BATTLEMON_MOVE_3 = 8,
    BATTLEMON_MOVE_4 = 9,
    BATTLEMON_HP_IV = 10,
    BATTLEMON_ATTACK_IV = 11,
    BATTLEMON_DEFENSE_IV = 12,
    BATTLEMON_SPEED_IV = 13,
    BATTLEMON_SP_ATTACK_IV = 14,
    BATTLEMON_SP_DEFENSE_IV = 15,
    BATTLEMON_IS_EGG = 16,
    BATTLEMON_HAS_NICKNAME = 17,
    BATTLEMON_HP_STAGE = 18,
    BATTLEMON_ATTACK_STAGE = 19,
    BATTLEMON_DEFENSE_STAGE = 20,
    BATTLEMON_SPEED_STAGE = 21,
    BATTLEMON_SP_ATTACK_STAGE = 22,
    BATTLEMON_SP_DEFENSE_STAGE = 23,
    BATTLEMON_ACCURACY_STAGE = 24,
    BATTLEMON_EVASION_STAGE = 25,
    BATTLEMON_ABILITY = 26,
    BATTLEMON_TYPE_1 = 27,
    BATTLEMON_TYPE_2 = 28,
    BATTLEMON_GENDER = 29,
    BATTLEMON_IS_SHINY = 30,
    BATTLEMON_CUR_PP_1 = 31,
    BATTLEMON_CUR_PP_2 = 32,
    BATTLEMON_CUR_PP_3 = 33,
    BATTLEMON_CUR_PP_4 = 34,
    BATTLEMON_PP_UPS_1 = 35,
    BATTLEMON_PP_UPS_2 = 36,
    BATTLEMON_PP_UPS_3 = 37,
    BATTLEMON_PP_UPS_4 = 38,
    BATTLEMON_MAX_PP_1 = 39,
    BATTLEMON_MAX_PP_2 = 40,
    BATTLEMON_MAX_PP_3 = 41,
    BATTLEMON_MAX_PP_4 = 42,
    BATTLEMON_LEVEL = 43,
    BATTLEMON_FRIENDSHIP = 44,
    BATTLEMON_NICKNAME = 45,
    BATTLEMON_NICKNAME_STRING = 46,
    BATTLEMON_CUR_HP = 47,
    BATTLEMON_MAX_HP = 48,
    BATTLEMON_OT_NAME = 49,
    BATTLEMON_EXP = 50,
    BATTLEMON_PERSONALITY = 51,
    BATTLEMON_STATUS = 52,
    BATTLEMON_VOLATILE_STATUS = 53,
    BATTLEMON_OT_ID = 54,
    BATTLEMON_HELD_ITEM = 55,
    BATTLEMON_TIMES_DAMAGED = 56,
    BATTLEMON_TRAINER_MESSAGE_FLAGS = 57,
    BATTLEMON_OT_GENDER = 58,
    BATTLEMON_MOVE_EFFECTS_MASK = 59,
    BATTLEMON_MOVE_EFFECTS_TEMP = 60,
    BATTLEMON_DISABLED_TURNS = 61,
    BATTLEMON_ENCORED_TURNS = 62,
    BATTLEMON_CHARGED_TURNS = 63,
    BATTLEMON_TAUNTED_TURNS = 64,
    BATTLEMON_SUCCESSFUL_PROTECT_TURNS = 65,
    BATTLEMON_PERISH_SONG_TURNS = 66,
    BATTLEMON_ROLLOUT_COUNT = 67,
    BATTLEMON_FURY_CUTTER_COUNT = 68,
    BATTLEMON_STOCKPILE_COUNT = 69,
    BATTLEMON_STOCKPILE_DEF_BOOSTS = 70,
    BATTLEMON_STOCKPILE_SPDEF_BOOSTS = 71,
    BATTLEMON_TRUANT = 72,
    BATTLEMON_FLASH_FIRE = 73,
    BATTLEMON_LOCK_ON_TARGET = 74,
    BATTLEMON_MIMICKED_MOVE_SLOT = 75,
    BATTLEMON_BIND_TARGET = 76,
    BATTLEMON_MEAN_LOOK_TARGET = 77,
    BATTLEMON_LAST_RESORT_COUNT = 78,
    BATTLEMON_MAGNET_RISE_TURNS = 79,
    BATTLEMON_HEAL_BLOCK_TURNS = 80,
    BATTLEMON_EMBARGO_TURNS = 81,
    BATTLEMON_CAN_UNBURDEN = 82,
    BATTLEMON_METRONOME_TURNS = 83,
    BATTLEMON_MICLE_BERRY = 84,
    BATTLEMON_CUSTAP_BERRY = 85,
    BATTLEMON_QUICK_CLAW = 86,
    BATTLEMON_RECHARGE_TURN_NUMBER = 87,
    BATTLEMON_FAKE_OUT_TURN_NUMBER = 88,
    BATTLEMON_SLOW_START_TURN_NUMBER = 89,
    BATTLEMON_SUBSTITUTE_HP = 90,
    BATTLEMON_TRANSFORMED_PERSONALITY = 91,
    BATTLEMON_DISABLED_MOVE = 92,
    BATTLEMON_ENCORED_MOVE = 93,
    BATTLEMON_BINDING_MOVE = 94,
    BATTLEMON_ITEM_HP_RECOVERY = 95,
    BATTLEMON_SLOW_START_ANNOUNCED = 96,
    BATTLEMON_SLOW_START_FINISHED = 97,
    BATTLEMON_FORM_NUM = 98,
    BATTLEMON_UNUSED_99 = 99,
    BATTLEMON_TEMP = 100,
};

enum SpeciesDataParam {
    SPECIES_DATA_BASE_HP = 0,
    SPECIES_DATA_BASE_ATK = 1,
    SPECIES_DATA_BASE_DEF = 2,
    SPECIES_DATA_BASE_SPEED = 3,
    SPECIES_DATA_BASE_SP_ATK = 4,
    SPECIES_DATA_BASE_SP_DEF = 5,
    SPECIES_DATA_TYPE_1 = 6,
    SPECIES_DATA_TYPE_2 = 7,
    SPECIES_DATA_CATCH_RATE = 8,
    SPECIES_DATA_BASE_EXP_REWARD = 9,
    SPECIES_DATA_EV_HP_YIELD = 10,
    SPECIES_DATA_EV_ATK_YIELD = 11,
    SPECIES_DATA_EV_DEF_YIELD = 12,
    SPECIES_DATA_EV_SPEED_YIELD = 13,
    SPECIES_DATA_EV_SP_ATK_YIELD = 14,
    SPECIES_DATA_EV_SP_DEF_YIELD = 15,
    SPECIES_DATA_HELD_ITEM_COMMON = 16,
    SPECIES_DATA_HELD_ITEM_RARE = 17,
    SPECIES_DATA_GENDER_RATIO = 18,
    SPECIES_DATA_HATCH_CYCLES = 19,
    SPECIES_DATA_BASE_FRIENDSHIP = 20,
    SPECIES_DATA_EXP_RATE = 21,
    SPECIES_DATA_EGG_GROUP_1 = 22,
    SPECIES_DATA_EGG_GROUP_2 = 23,
    SPECIES_DATA_ABILITY_1 = 24,
    SPECIES_DATA_ABILITY_2 = 25,
    SPECIES_DATA_SAFARI_FLEE_RATE = 26,
    SPECIES_DATA_BODY_COLOR = 27,
    SPECIES_DATA_FLIP_SPRITE = 28,
    SPECIES_DATA_TM_LEARNSET_MASK_1 = 29,
    SPECIES_DATA_TM_LEARNSET_MASK_2 = 30,
    SPECIES_DATA_TM_LEARNSET_MASK_3 = 31,
    SPECIES_DATA_TM_LEARNSET_MASK_4 = 32,
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

enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};

u32 GetMonBaseStat(int monSpecies, enum SpeciesDataParam param);

typedef struct Fraction {
    u8 numerator;
    u8 denominator;
} Fraction;

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

int BattleMon_Get(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, void *buf);

void Heap_Free(void *ptr);

BattleContext *BattleSystem_GetBattleContext(BattleSystem *battleSys);

u8 ov12_0223B694(BattleSystem *battleSys);

u16 BattleSystem_Random(BattleSystem *battleSys);

typedef struct CommandSetData {
    BattleSystem *battleSys;
    void *healthbox;
    u8 command;
    u8 battler;
    u8 state;
    s8 unused_0B;
    int input;
    u8 ballStatus[PARTY_GAUGE_COUNT][6];
    u8 expPercents[6];
    u8 unused_22;
    u8 partySlot;
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 battlerType;
    u8 msgIdx;
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
} CommandSetData;

extern const Fraction ov12_0226D140[13];

void BattleController_EmitSelectedCommand(BattleSystem *battleSystem, int battler, int command);

void ov12_0225E154(SysTask *task, void *data);

void ov12_0225E154(SysTask *task, void *data) {
    CommandSetData *commandSetData = data;
    u8 escapeCount = ov12_0223B694(commandSetData->battleSys);
    u16 species = BattleMon_Get(BattleSystem_GetBattleContext(commandSetData->battleSys), commandSetData->battler, BATTLEMON_SPECIES, 0L);
    int fleeRate = GetMonBaseStat(species, SPECIES_DATA_SAFARI_FLEE_RATE);
    fleeRate = fleeRate * ov12_0226D140[escapeCount].numerator / ov12_0226D140[escapeCount].denominator;

    if (BattleSystem_Random(commandSetData->battleSys) % 255 <= fleeRate) {
        BattleController_EmitSelectedCommand(commandSetData->battleSys, commandSetData->battler, 4);
    } else {
        BattleController_EmitSelectedCommand(commandSetData->battleSys, commandSetData->battler, 5);
    }

    BattleController_EmitClearCommand(commandSetData->battleSys, commandSetData->battler, commandSetData->command);

    Heap_Free(data);
    SysTask_Destroy(task);
}
