#include "global.h"

#include "math_util.h"
#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcade;
struct BattleArcadeSave;
struct FieldBattleDTO;
struct Party;
struct Pokemon;
BOOL ov80_0222BA7C(BattleArcade *battleArcade);
BOOL ov80_0222BAE0(BattleArcade *battleArcade);
BOOL ov80_0222BB54(BattleArcade *battleArcade);
BOOL ov80_0222BC48(BattleArcade *battleArcade, u16 param1);
BOOL ov80_0222BC94(BattleArcade *battleArcade, u16 param1);
BOOL ov80_0222BCE0(BattleArcade *battleArcade);
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
typedef struct BattleArcadeSave BattleArcadeSave;
u8 BattleArcade_GetMonCount(u8 challengeType, BOOL includePartnerMons);
u8 BattleArcade_GetOpponentMonCount(u8 challengeType, BOOL includeBothOpponents);
typedef void (*BattleArcadeEffectFunc)(BattleArcade *, Party *, u8);
void ov80_02234ECC(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_0223509C(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_022351AC(BattleArcade *arcade, Party *party, u8 partySize);
extern BattleArcadeEffectFunc ov80_0223DCB8[32];
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
u32 GetMonData(Pokemon *mon, int param, void *dest);
void SetMonData(Pokemon *mon, int param, const void *value);
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
u16 ov80_02238498(BattleArcade *battleArcade);
extern const u16 sBerryPoolTier1[8];
extern const u16 sBerryPoolTier2[20];
extern const u16 sBerryPoolTier3[10];
extern const u16 *ov80_0223DCA0[3];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov80_02234E50(BattleArcade *arcade, u32 command, u32 arg);
#else
BOOL ov80_02234E50(BattleArcade *arcade, u16 command, u16 arg);
#endif
void ov80_02234E98(BattleArcade *arcade, u8 effect);
void ov80_02234ECC(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_02234F28(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_02234FAC(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_02235024(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_0223509C(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_02235118(BattleArcade *arcade, Party *party, u8 partySize);
void ov80_022351AC(BattleArcade *arcade, Party *party, u8 partySize);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov80_02234E50(BattleArcade *arcade, u32 command, u32 arg) {
#else
BOOL ov80_02234E50(BattleArcade *arcade, u16 command, u16 arg) {
#endif
    int success;

    switch (command) {
    case 0:
        success = ov80_0222BA7C(arcade);
        break;
    case 1:
        success = ov80_0222BAE0(arcade);
        break;
    case 2:
        success = ov80_0222BB54(arcade);
        break;
    case 3:
        success = ov80_0222BC48(arcade, arg);
        break;
    case 4:
        success = ov80_0222BC94(arcade, arg);
        break;
    case 5:
        success = ov80_0222BCE0(arcade);
        break;
    }

    return success;
}

void ov80_02234E98(BattleArcade *arcade, u8 effect) {
    u8 partySize;
    Party *party;
    if (effect < 9) {
        party = arcade->opponentsParty;
        partySize = BattleArcade_GetOpponentMonCount(arcade->challengeType, 1);
    } else {
        party = arcade->playersParty;
        partySize = BattleArcade_GetMonCount(arcade->challengeType, 1);
    }

    BattleArcadeEffectFunc func = ov80_0223DCB8[effect];
    func(arcade, party, partySize);
}

void ov80_02234ECC(BattleArcade *arcade, Party *party, u8 partySize) {
    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP, 0L);
        u32 hp = maxHP * 1.2;
        hp -= maxHP;
        hp = maxHP - hp;

        SetMonData(mon, MON_DATA_HP, &hp);
    }
}

void ov80_02234F28(BattleArcade *arcade, Party *party, u8 partySize) {
    int numImmune = 0;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        u32 type1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
        u32 type2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);
        u32 ability = GetMonData(mon, MON_DATA_ABILITY, 0L);

        if (type1 == TYPE_POISON || type2 == TYPE_POISON || type1 == TYPE_STEEL || type2 == TYPE_STEEL || ability == ABILITY_IMMUNITY) {
            numImmune++;
        } else {
            u32 newStatus = (1 << 3);
            SetMonData(mon, MON_DATA_STATUS, &newStatus);
        }
    }

    if (numImmune >= partySize) {
        arcade->immuneToEffect = 1;
    }
}

void ov80_02234FAC(BattleArcade *arcade, Party *party, u8 partySize) {
    int numImmune = 0;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        u32 type1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
        u32 type2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);
        u32 ability = GetMonData(mon, MON_DATA_ABILITY, 0L);

        if (type1 == TYPE_GROUND || type2 == TYPE_GROUND || ability == ABILITY_LIMBER) {
            numImmune++;
        } else {
            u32 newStatus = (1 << 6);
            SetMonData(mon, MON_DATA_STATUS, &newStatus);
        }
    }

    if (numImmune >= partySize) {
        arcade->immuneToEffect = 1;
    }
}

void ov80_02235024(BattleArcade *arcade, Party *party, u8 partySize) {
    int numImmune = 0;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        u32 type1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
        u32 type2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);
        u32 ability = GetMonData(mon, MON_DATA_ABILITY, 0L);

        if (type1 == TYPE_FIRE || type2 == TYPE_FIRE || ability == ABILITY_WATER_VEIL) {
            numImmune++;
        } else {
            u32 newStatus = (1 << 4);
            SetMonData(mon, MON_DATA_STATUS, &newStatus);
        }
    }

    if (numImmune >= partySize) {
        arcade->immuneToEffect = 1;
    }
}

void ov80_0223509C(BattleArcade *arcade, Party *party, u8 partySize) {
    int numImmune = 0;
    u8 slot = arcade->randomIndex % partySize;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, slot);
        u32 ability = GetMonData(mon, MON_DATA_ABILITY, 0L);

        if (ability == ABILITY_INSOMNIA || ability == ABILITY_VITAL_SPIRIT) {
            numImmune++;
            slot++;

            if (slot >= partySize) {
                slot = 0;
            }
        } else {
            u32 newStatus = LCRandom() % 4 + 2;
            SetMonData(mon, MON_DATA_STATUS, &newStatus);
            break;
        }
    }

    if (numImmune >= partySize) {
        arcade->immuneToEffect = 1;
    }
}

void ov80_02235118(BattleArcade *arcade, Party *party, u8 partySize) {
    int numImmune = 0;
    u8 slot = arcade->randomIndex % partySize;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, slot);
        u32 type1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
        u32 type2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);
        u32 ability = GetMonData(mon, MON_DATA_ABILITY, 0L);

        if (type1 == TYPE_ICE || type2 == TYPE_ICE || ability == ABILITY_MAGMA_ARMOR) {
            numImmune++;
            slot++;

            if (slot >= partySize) {
                slot = 0;
            }
        } else {
            u32 newStatus = (1 << 5);
            SetMonData(mon, MON_DATA_STATUS, &newStatus);
            break;
        }
    }

    if (numImmune >= partySize) {
        arcade->immuneToEffect = 1;
    }
}

void ov80_022351AC(BattleArcade *arcade, Party *party, u8 partySize) {
    u16 currentRound = ov80_02238498(arcade);

    int poolSize;
    const u16 *itemPool;
    if (currentRound < 3) {
        itemPool = ov80_0223DCA0[0];
        poolSize = (sizeof(sBerryPoolTier1) / sizeof(*(sBerryPoolTier1)));
    } else if (currentRound < 6) {
        itemPool = ov80_0223DCA0[1];
        poolSize = (sizeof(sBerryPoolTier2) / sizeof(*(sBerryPoolTier2)));
    } else {
        itemPool = ov80_0223DCA0[2];
        poolSize = (sizeof(sBerryPoolTier3) / sizeof(*(sBerryPoolTier3)));
    }

    u16 newItem = itemPool[arcade->randomIndex % poolSize];

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        SetMonData(mon, MON_DATA_HELD_ITEM, &newItem);
    }
}
