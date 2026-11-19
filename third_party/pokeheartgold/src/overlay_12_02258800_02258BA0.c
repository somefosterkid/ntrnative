#include "global.h"
#include "sprite_resources_header.h"

#include "seal_case.h"
#include "filesystem_files_def.h"
#include "unk_02013534.h"
#include "item.h"
#include "overlay_07_ball_rotation.h"
#include "pokemon_types_def.h"
#include "battle/battle.h"

struct BallRotation;
struct BattleContext;
struct BattleSystem;
struct TextOBJ;
enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
};
enum ItemDataParam {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_EFFECT_PARAM,
    ITEM_PARAM_PREVENT_TOSS,
    ITEM_PARAM_CAN_REGISTER,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_USE_FUNC,
    ITEM_PARAM_BATTLE_USE_CATEGORY,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_PARTY_USE,
    ITEM_PARAM_HEAL_SLEEP,
    ITEM_PARAM_HEAL_POISON,
    ITEM_PARAM_HEAL_BURN,
    ITEM_PARAM_HEAL_FREEZE,
    ITEM_PARAM_HEAL_PARALYSIS,
    ITEM_PARAM_HEAL_CONFUSION,
    ITEM_PARAM_HEAL_ATTRACT,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATK_STAGES,
    ITEM_PARAM_DEF_STAGES,
    ITEM_PARAM_SPATK_STAGES,
    ITEM_PARAM_SPDEF_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACC_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_GIVE_HP_EVS,
    ITEM_PARAM_GIVE_ATK_EVS,
    ITEM_PARAM_GIVE_DEF_EVS,
    ITEM_PARAM_GIVE_SPEED_EVS,
    ITEM_PARAM_GIVE_SPATK_EVS,
    ITEM_PARAM_GIVE_SPDEF_EVS,
    ITEM_PARAM_GIVE_FRIENDSHIP_LOW,
    ITEM_PARAM_GIVE_FRIENDSHIP_MED,
    ITEM_PARAM_GIVE_FRIENDSHIP_HIGH,
    ITEM_PARAM_HP_EVS,
    ITEM_PARAM_ATK_EVS,
    ITEM_PARAM_DEF_EVS,
    ITEM_PARAM_SPEED_EVS,
    ITEM_PARAM_SPATK_EVS,
    ITEM_PARAM_SPDEF_EVS,
    ITEM_PARAM_HP_RESTORED,
    ITEM_PARAM_PP_RESTORED,
    ITEM_PARAM_FRIENDSHIP_LOW,
    ITEM_PARAM_FRIENDSHIP_MED,
    ITEM_PARAM_FRIENDSHIP_HIGH,
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
typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;
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
enum CountAbilityMode {
    COUNT_ALL_BATTLERS_OUR_SIDE = 0,
    COUNT_ALIVE_BATTLERS_OUR_SIDE,
    COUNT_ALL_BATTLERS_THEIR_SIDE,
    COUNT_ALIVE_BATTLERS_THEIR_SIDE,
    COUNT_ALIVE_BATTLERS_THEIR_SIDE_FLAG,
    COUNT_ALL_BATTLERS,
    COUNT_ALL_BATTLERS_EXCEPT_ME,
    COUNT_ALL_BATTLERS_EXCEPT_ME_RETURN_BATTLER,
    COUNT_ALIVE_BATTLERS,
    COUNT_ALIVE_BATTLERS_EXCEPT_ME,
};
BattleContext *BattleSystem_GetBattleContext(BattleSystem *battleSys);
int CheckAbilityActive(BattleSystem *battleSys, BattleContext *battleCtx, enum CountAbilityMode mode, int battler, int ability);
s32 GetItemVar(BattleContext *battleCtx, u16 item, u16 paramID);
u32 GetMonData(Pokemon *mon, int param, void *dest);
enum Ability {
    ABILITY_NONE = 0,
    ABILITY_STENCH = 1,
    ABILITY_DRIZZLE = 2,
    ABILITY_SPEED_BOOST = 3,
    ABILITY_BATTLE_ARMOR = 4,
    ABILITY_STURDY = 5,
    ABILITY_DAMP = 6,
    ABILITY_LIMBER = 7,
    ABILITY_SAND_VEIL = 8,
    ABILITY_STATIC = 9,
    ABILITY_VOLT_ABSORB = 10,
    ABILITY_WATER_ABSORB = 11,
    ABILITY_OBLIVIOUS = 12,
    ABILITY_CLOUD_NINE = 13,
    ABILITY_COMPOUND_EYES = 14,
    ABILITY_INSOMNIA = 15,
    ABILITY_COLOR_CHANGE = 16,
    ABILITY_IMMUNITY = 17,
    ABILITY_FLASH_FIRE = 18,
    ABILITY_SHIELD_DUST = 19,
    ABILITY_OWN_TEMPO = 20,
    ABILITY_SUCTION_CUPS = 21,
    ABILITY_INTIMIDATE = 22,
    ABILITY_SHADOW_TAG = 23,
    ABILITY_ROUGH_SKIN = 24,
    ABILITY_WONDER_GUARD = 25,
    ABILITY_LEVITATE = 26,
    ABILITY_EFFECT_SPORE = 27,
    ABILITY_SYNCHRONIZE = 28,
    ABILITY_CLEAR_BODY = 29,
    ABILITY_NATURAL_CURE = 30,
    ABILITY_LIGHTNING_ROD = 31,
    ABILITY_SERENE_GRACE = 32,
    ABILITY_SWIFT_SWIM = 33,
    ABILITY_CHLOROPHYLL = 34,
    ABILITY_ILLUMINATE = 35,
    ABILITY_TRACE = 36,
    ABILITY_HUGE_POWER = 37,
    ABILITY_POISON_POINT = 38,
    ABILITY_INNER_FOCUS = 39,
    ABILITY_MAGMA_ARMOR = 40,
    ABILITY_WATER_VEIL = 41,
    ABILITY_MAGNET_PULL = 42,
    ABILITY_SOUNDPROOF = 43,
    ABILITY_RAIN_DISH = 44,
    ABILITY_SAND_STREAM = 45,
    ABILITY_PRESSURE = 46,
    ABILITY_THICK_FAT = 47,
    ABILITY_EARLY_BIRD = 48,
    ABILITY_FLAME_BODY = 49,
    ABILITY_RUN_AWAY = 50,
    ABILITY_KEEN_EYE = 51,
    ABILITY_HYPER_CUTTER = 52,
    ABILITY_PICKUP = 53,
    ABILITY_TRUANT = 54,
    ABILITY_HUSTLE = 55,
    ABILITY_CUTE_CHARM = 56,
    ABILITY_PLUS = 57,
    ABILITY_MINUS = 58,
    ABILITY_FORECAST = 59,
    ABILITY_STICKY_HOLD = 60,
    ABILITY_SHED_SKIN = 61,
    ABILITY_GUTS = 62,
    ABILITY_MARVEL_SCALE = 63,
    ABILITY_LIQUID_OOZE = 64,
    ABILITY_OVERGROW = 65,
    ABILITY_BLAZE = 66,
    ABILITY_TORRENT = 67,
    ABILITY_SWARM = 68,
    ABILITY_ROCK_HEAD = 69,
    ABILITY_DROUGHT = 70,
    ABILITY_ARENA_TRAP = 71,
    ABILITY_VITAL_SPIRIT = 72,
    ABILITY_WHITE_SMOKE = 73,
    ABILITY_PURE_POWER = 74,
    ABILITY_SHELL_ARMOR = 75,
    ABILITY_AIR_LOCK = 76,
    ABILITY_TANGLED_FEET = 77,
    ABILITY_MOTOR_DRIVE = 78,
    ABILITY_RIVALRY = 79,
    ABILITY_STEADFAST = 80,
    ABILITY_SNOW_CLOAK = 81,
    ABILITY_GLUTTONY = 82,
    ABILITY_ANGER_POINT = 83,
    ABILITY_UNBURDEN = 84,
    ABILITY_HEATPROOF = 85,
    ABILITY_SIMPLE = 86,
    ABILITY_DRY_SKIN = 87,
    ABILITY_DOWNLOAD = 88,
    ABILITY_IRON_FIST = 89,
    ABILITY_POISON_HEAL = 90,
    ABILITY_ADAPTABILITY = 91,
    ABILITY_SKILL_LINK = 92,
    ABILITY_HYDRATION = 93,
    ABILITY_SOLAR_POWER = 94,
    ABILITY_QUICK_FEET = 95,
    ABILITY_NORMALIZE = 96,
    ABILITY_SNIPER = 97,
    ABILITY_MAGIC_GUARD = 98,
    ABILITY_NO_GUARD = 99,
    ABILITY_STALL = 100,
    ABILITY_TECHNICIAN = 101,
    ABILITY_LEAF_GUARD = 102,
    ABILITY_KLUTZ = 103,
    ABILITY_MOLD_BREAKER = 104,
    ABILITY_SUPER_LUCK = 105,
    ABILITY_AFTERMATH = 106,
    ABILITY_ANTICIPATION = 107,
    ABILITY_FOREWARN = 108,
    ABILITY_UNAWARE = 109,
    ABILITY_TINTED_LENS = 110,
    ABILITY_FILTER = 111,
    ABILITY_SLOW_START = 112,
    ABILITY_SCRAPPY = 113,
    ABILITY_STORM_DRAIN = 114,
    ABILITY_ICE_BODY = 115,
    ABILITY_SOLID_ROCK = 116,
    ABILITY_SNOW_WARNING = 117,
    ABILITY_HONEY_GATHER = 118,
    ABILITY_FRISK = 119,
    ABILITY_RECKLESS = 120,
    ABILITY_MULTITYPE = 121,
    ABILITY_FLOWER_GIFT = 122,
    ABILITY_BAD_DREAMS = 123,
};

int ov12_02258BA0(BattleSystem *battleSys, int battler);
int ov12_02258BB4(BattleSystem *battleSys, BattleContext *battleCtx, Pokemon *mon, int move);

int ov12_02258BA0(BattleSystem *battleSys, int battler) {
    BattleContext *battleCtx = BattleSystem_GetBattleContext(battleSys);
    return battleCtx->unk_21A4[battler];
}

int ov12_02258BB4(BattleSystem *battleSys, BattleContext *battleCtx, Pokemon *mon, int move) {
    int type;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        type = GetItemVar(battleCtx, GetMonData(mon, MON_DATA_HELD_ITEM, 0L), ITEM_PARAM_NATURAL_GIFT_TYPE);
        break;

    case MOVE_JUDGMENT:
        switch (GetItemVar(battleCtx, GetMonData(mon, MON_DATA_HELD_ITEM, 0L), ITEM_PARAM_HOLD_EFFECT)) {
        case HOLD_EFFECT_ARCEUS_FIGHTING:
            type = TYPE_FIGHTING;
            break;
        case HOLD_EFFECT_ARCEUS_FLYING:
            type = TYPE_FLYING;
            break;
        case HOLD_EFFECT_ARCEUS_POISON:
            type = TYPE_POISON;
            break;
        case HOLD_EFFECT_ARCEUS_GROUND:
            type = TYPE_GROUND;
            break;
        case HOLD_EFFECT_ARCEUS_ROCK:
            type = TYPE_ROCK;
            break;
        case HOLD_EFFECT_ARCEUS_BUG:
            type = TYPE_BUG;
            break;
        case HOLD_EFFECT_ARCEUS_GHOST:
            type = TYPE_GHOST;
            break;
        case HOLD_EFFECT_ARCEUS_STEEL:
            type = TYPE_STEEL;
            break;
        case HOLD_EFFECT_ARCEUS_FIRE:
            type = TYPE_FIRE;
            break;
        case HOLD_EFFECT_ARCEUS_WATER:
            type = TYPE_WATER;
            break;
        case HOLD_EFFECT_ARCEUS_GRASS:
            type = TYPE_GRASS;
            break;
        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            type = TYPE_ELECTRIC;
            break;
        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            type = TYPE_PSYCHIC;
            break;
        case HOLD_EFFECT_ARCEUS_ICE:
            type = TYPE_ICE;
            break;
        case HOLD_EFFECT_ARCEUS_DRAGON:
            type = TYPE_DRAGON;
            break;
        case HOLD_EFFECT_ARCEUS_DARK:
            type = TYPE_DARK;
            break;
        default:
            type = TYPE_NORMAL;
            break;
        }
        break;

    case MOVE_HIDDEN_POWER:
        type = ((GetMonData(mon, MON_DATA_HP_IV, 0L) & 1) >> 0)
            | ((GetMonData(mon, MON_DATA_ATK_IV, 0L) & 1) << 1)
            | ((GetMonData(mon, MON_DATA_DEF_IV, 0L) & 1) << 2)
            | ((GetMonData(mon, MON_DATA_SPEED_IV, 0L) & 1) << 3)
            | ((GetMonData(mon, MON_DATA_SPATK_IV, 0L) & 1) << 4)
            | ((GetMonData(mon, MON_DATA_SPDEF_IV, 0L) & 1) << 5);
        type = (type * 15 / 63) + 1;

        if (type >= TYPE_MYSTERY) {
            type++;
        }
        break;

    case MOVE_WEATHER_BALL:
        if (CheckAbilityActive(battleSys, battleCtx, COUNT_ALIVE_BATTLERS, 0, ABILITY_CLOUD_NINE) == 0 && CheckAbilityActive(battleSys, battleCtx, COUNT_ALIVE_BATTLERS, 0, ABILITY_AIR_LOCK) == 0) {
            if (battleCtx->fieldCondition & (((1 << 0) | (1 << 1)) | ((1 << 2) | (1 << 3)) | ((1 << 4) | (1 << 5)) | ((1 << 6) | (1 << 7)) | (1 << 15))) {
                if (battleCtx->fieldCondition & ((1 << 0) | (1 << 1))) {
                    type = TYPE_WATER;
                }

                if (battleCtx->fieldCondition & ((1 << 2) | (1 << 3))) {
                    type = TYPE_ROCK;
                }

                if (battleCtx->fieldCondition & ((1 << 4) | (1 << 5))) {
                    type = TYPE_FIRE;
                }

                if (battleCtx->fieldCondition & ((1 << 6) | (1 << 7))) {
                    type = TYPE_ICE;
                }
            }
        }
        break;

    default:
        type = TYPE_NORMAL;
        break;
    }

    return type;
}
