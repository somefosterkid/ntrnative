#include "global.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"
#include "save_frontier.h"
#include "constants/heap.h"

struct FrontierSave;

struct BattleHall;

struct BattleHallSave;

struct BattleHallStreakFlags;

struct FieldBattleDTO;

struct FrontierOpponent;

struct FrontierPokemon;

struct Party;

struct Pokemon;

struct SaveData;

struct SaveVarsFlags;

typedef struct SaveData SaveData;

typedef struct Pokemon Pokemon;

void *Heap_Alloc(u32 heapID, u32 size);

enum PokemonDataParam {
    MON_DATA_PERSONALITY = 0,
    MON_DATA_IS_PARTY_DECRYPTED = 1,
    MON_DATA_IS_BOX_DECRYPTED = 2,
    MON_DATA_CHECKSUM_FAILED = 3,
    MON_DATA_CHECKSUM = 4,
    MON_DATA_SPECIES = 5,
    MON_DATA_HELD_ITEM = 6,
    MON_DATA_OT_ID = 7,
    MON_DATA_EXPERIENCE = 8,
    MON_DATA_FRIENDSHIP = 9,
    MON_DATA_ABILITY = 10,
    MON_DATA_MARKINGS = 11,
    MON_DATA_LANGUAGE = 12,
    MON_DATA_HP_EV = 13,
    MON_DATA_ATK_EV = 14,
    MON_DATA_DEF_EV = 15,
    MON_DATA_SPEED_EV = 16,
    MON_DATA_SPATK_EV = 17,
    MON_DATA_SPDEF_EV = 18,
    MON_DATA_COOL = 19,
    MON_DATA_BEAUTY = 20,
    MON_DATA_CUTE = 21,
    MON_DATA_SMART = 22,
    MON_DATA_TOUGH = 23,
    MON_DATA_SHEEN = 24,
    MON_DATA_SINNOH_CHAMP_RIBBON = 25,
    MON_DATA_ABILITY_RIBBON = 26,
    MON_DATA_GREAT_ABILITY_RIBBON = 27,
    MON_DATA_DOUBLE_ABILITY_RIBBON = 28,
    MON_DATA_MULTI_ABILITY_RIBBON = 29,
    MON_DATA_PAIR_ABILITY_RIBBON = 30,
    MON_DATA_WORLD_ABILITY_RIBBON = 31,
    MON_DATA_ALERT_RIBBON = 32,
    MON_DATA_SHOCK_RIBBON = 33,
    MON_DATA_DOWNCAST_RIBBON = 34,
    MON_DATA_CARELESS_RIBBON = 35,
    MON_DATA_RELAX_RIBBON = 36,
    MON_DATA_SNOOZE_RIBBON = 37,
    MON_DATA_SMILE_RIBBON = 38,
    MON_DATA_GORGEOUS_RIBBON = 39,
    MON_DATA_ROYAL_RIBBON = 40,
    MON_DATA_GORGEOUS_ROYAL_RIBBON = 41,
    MON_DATA_FOOTPRINT_RIBBON = 42,
    MON_DATA_RECORD_RIBBON = 43,
    MON_DATA_HISTORY_RIBBON = 44,
    MON_DATA_LEGEND_RIBBON = 45,
    MON_DATA_RED_RIBBON = 46,
    MON_DATA_GREEN_RIBBON = 47,
    MON_DATA_BLUE_RIBBON = 48,
    MON_DATA_FESTIVAL_RIBBON = 49,
    MON_DATA_CARNIVAL_RIBBON = 50,
    MON_DATA_CLASSIC_RIBBON = 51,
    MON_DATA_PREMIER_RIBBON = 52,
    MON_DATA_UNUSED_RIBBON_53 = 53,
    MON_DATA_MOVE1 = 54,
    MON_DATA_MOVE2 = 55,
    MON_DATA_MOVE3 = 56,
    MON_DATA_MOVE4 = 57,
    MON_DATA_MOVE1_PP = 58,
    MON_DATA_MOVE2_PP = 59,
    MON_DATA_MOVE3_PP = 60,
    MON_DATA_MOVE4_PP = 61,
    MON_DATA_MOVE1_PP_UPS = 62,
    MON_DATA_MOVE2_PP_UPS = 63,
    MON_DATA_MOVE3_PP_UPS = 64,
    MON_DATA_MOVE4_PP_UPS = 65,
    MON_DATA_MOVE1_MAX_PP = 66,
    MON_DATA_MOVE2_MAX_PP = 67,
    MON_DATA_MOVE3_MAX_PP = 68,
    MON_DATA_MOVE4_MAX_PP = 69,
    MON_DATA_HP_IV = 70,
    MON_DATA_ATK_IV = 71,
    MON_DATA_DEF_IV = 72,
    MON_DATA_SPEED_IV = 73,
    MON_DATA_SPATK_IV = 74,
    MON_DATA_SPDEF_IV = 75,
    MON_DATA_IS_EGG = 76,
    MON_DATA_HAS_NICKNAME = 77,
    MON_DATA_COOL_RIBBON = 78,
    MON_DATA_COOL_RIBBON_SUPER = 79,
    MON_DATA_COOL_RIBBON_HYPER = 80,
    MON_DATA_COOL_RIBBON_MASTER = 81,
    MON_DATA_BEAUTY_RIBBON = 82,
    MON_DATA_BEAUTY_RIBBON_SUPER = 83,
    MON_DATA_BEAUTY_RIBBON_HYPER = 84,
    MON_DATA_BEAUTY_RIBBON_MASTER = 85,
    MON_DATA_CUTE_RIBBON = 86,
    MON_DATA_CUTE_RIBBON_SUPER = 87,
    MON_DATA_CUTE_RIBBON_HYPER = 88,
    MON_DATA_CUTE_RIBBON_MASTER = 89,
    MON_DATA_SMART_RIBBON = 90,
    MON_DATA_SMART_RIBBON_SUPER = 91,
    MON_DATA_SMART_RIBBON_HYPER = 92,
    MON_DATA_SMART_RIBBON_MASTER = 93,
    MON_DATA_TOUGH_RIBBON = 94,
    MON_DATA_TOUGH_RIBBON_SUPER = 95,
    MON_DATA_TOUGH_RIBBON_HYPER = 96,
    MON_DATA_TOUGH_RIBBON_MASTER = 97,
    MON_DATA_CHAMPION_RIBBON = 98,
    MON_DATA_WINNING_RIBBON = 99,
    MON_DATA_VICTORY_RIBBON = 100,
    MON_DATA_ARTIST_RIBBON = 101,
    MON_DATA_EFFORT_RIBBON = 102,
    MON_DATA_MARINE_RIBBON = 103,
    MON_DATA_LAND_RIBBON = 104,
    MON_DATA_SKY_RIBBON = 105,
    MON_DATA_COUNTRY_RIBBON = 106,
    MON_DATA_NATIONAL_RIBBON = 107,
    MON_DATA_EARTH_RIBBON = 108,
    MON_DATA_WORLD_RIBBON = 109,
    MON_DATA_FATEFUL_ENCOUNTER = 110,
    MON_DATA_GENDER = 111,
    MON_DATA_FORM = 112,
    MON_DATA_UNUSED_113 = 113,
    MON_DATA_UNUSED_114 = 114,
    MON_DATA_EGG_LOCATION_PTHGSS = 115,
    MON_DATA_MET_LOCATION_PTHGSS = 116,
    MON_DATA_NICKNAME = 117,
    MON_DATA_NICKNAME_AND_FLAG = 118,
    MON_DATA_NICKNAME_STRING = 119,
    MON_DATA_NICKNAME_STRING_AND_FLAG = 120,
    MON_DATA_UNUSED_121 = 121,
    MON_DATA_MET_GAME = 122,
    MON_DATA_SUPER_COOL_RIBBON = 123,
    MON_DATA_SUPER_COOL_RIBBON_GREAT = 124,
    MON_DATA_SUPER_COOL_RIBBON_ULTRA = 125,
    MON_DATA_SUPER_COOL_RIBBON_MASTER = 126,
    MON_DATA_SUPER_BEAUTY_RIBBON = 127,
    MON_DATA_SUPER_BEAUTY_RIBBON_GREAT = 128,
    MON_DATA_SUPER_BEAUTY_RIBBON_ULTRA = 129,
    MON_DATA_SUPER_BEAUTY_RIBBON_MASTER = 130,
    MON_DATA_SUPER_CUTE_RIBBON = 131,
    MON_DATA_SUPER_CUTE_RIBBON_GREAT = 132,
    MON_DATA_SUPER_CUTE_RIBBON_ULTRA = 133,
    MON_DATA_SUPER_CUTE_RIBBON_MASTER = 134,
    MON_DATA_SUPER_SMART_RIBBON = 135,
    MON_DATA_SUPER_SMART_RIBBON_GREAT = 136,
    MON_DATA_SUPER_SMART_RIBBON_ULTRA = 137,
    MON_DATA_SUPER_SMART_RIBBON_MASTER = 138,
    MON_DATA_SUPER_TOUGH_RIBBON = 139,
    MON_DATA_SUPER_TOUGH_RIBBON_GREAT = 140,
    MON_DATA_SUPER_TOUGH_RIBBON_ULTRA = 141,
    MON_DATA_SUPER_TOUGH_RIBBON_MASTER = 142,
    MON_DATA_UNUSED_RIBBON_143 = 143,
    MON_DATA_OT_NAME = 144,
    MON_DATA_OT_NAME_STRING = 145,
    MON_DATA_EGG_YEAR = 146,
    MON_DATA_EGG_MONTH = 147,
    MON_DATA_EGG_DAY = 148,
    MON_DATA_MET_YEAR = 149,
    MON_DATA_MET_MONTH = 150,
    MON_DATA_MET_DAY = 151,
    MON_DATA_EGG_LOCATION = 152,
    MON_DATA_MET_LOCATION = 153,
    MON_DATA_POKERUS = 154,
    MON_DATA_POKEBALL = 155,
    MON_DATA_MET_LEVEL = 156,
    MON_DATA_OT_GENDER = 157,
    MON_DATA_MET_TERRAIN = 158,
    MON_DATA_UNUSED_159 = 159,
    MON_DATA_STATUS = 160,
    MON_DATA_LEVEL = 161,
    MON_DATA_BALL_CAPSULE_ID = 162,
    MON_DATA_HP = 163,
    MON_DATA_MAX_HP = 164,
    MON_DATA_ATK = 165,
    MON_DATA_DEF = 166,
    MON_DATA_SPEED = 167,
    MON_DATA_SP_ATK = 168,
    MON_DATA_SP_DEF = 169,
    MON_DATA_MAIL = 170,
    MON_DATA_BALL_CAPSULE = 171,
    MON_DATA_SPECIES_EXISTS = 172,
    MON_DATA_SANITY_IS_EGG = 173,
    MON_DATA_SPECIES_OR_EGG = 174,
    MON_DATA_COMBINED_IVS = 175,
    MON_DATA_PRINT_GENDER = 176,
    MON_DATA_TYPE_1 = 177,
    MON_DATA_TYPE_2 = 178,
    MON_DATA_SPECIES_NAME = 179,
};

enum PokemonType {
    TYPE_NORMAL = 0,
    TYPE_FIGHTING = 1,
    TYPE_FLYING = 2,
    TYPE_POISON = 3,
    TYPE_GROUND = 4,
    TYPE_ROCK = 5,
    TYPE_BUG = 6,
    TYPE_GHOST = 7,
    TYPE_STEEL = 8,
    TYPE_MYSTERY = 9,
    TYPE_FIRE = 10,
    TYPE_WATER = 11,
    TYPE_GRASS = 12,
    TYPE_ELECTRIC = 13,
    TYPE_PSYCHIC = 14,
    TYPE_ICE = 15,
    TYPE_DRAGON = 16,
    TYPE_DARK = 17,
    NUM_POKEMON_TYPES = 18,
};

typedef struct Party Party;

Party *SaveArray_Party_Alloc(enum HeapID heapID);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

Party *SaveArray_Party_Get(SaveData *saveData);

Pokemon *AllocMonZeroed(enum HeapID heapID);

u32 GetMonData(Pokemon *mon, enum PokemonDataParam param, void *dest);

void ov80_0222A840(SaveData *saveData);

void sub_02030AF8(BattleHallSave *challenge);

BattleHallSave *sub_02030B04(SaveData *saveData);

u32 sub_02030B88(BattleHallSave *save, u8 field, u8 index, u8 unused, void *unused2);

void sub_02030BF4(u8 pokemonType, u8 *typeRanks, u8 value);

BattleHallStreakFlags *sub_02030C5C(SaveData *saveData);

u32 sub_02030CA0(BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2);

typedef struct BattleHall {
    int unused;
    u8 challengeType;
    u8 currentBattle;
    u8 saveStreak;
    u8 opponentsLevel;
    u16 currentStreak;
    u16 currentRound;
    fx32 playerLevelSqrt;
    u32 unk_10;
    int wonBattle;
    u16 trainerIDs[10 * 2];
    FrontierOpponent opponents[2];
    u8 partySlots[2];
    Party *party;
    u16 monIndices[10 * 2];
    FrontierPokemon opponentMons[10 * 2];
    u16 unk_6F0;
    u16 selectedTypeIdx2;
    u8 selectedType;
    u8 selectedTypeIdx;
    BattleHallSave *hallSave;
    SaveData *saveData;
    FieldBattleDTO *dto;
    u8 typeRanks[4][9];
    u16 heldItems[2];
    u16 commBuffer[44];
    u8 commHugeBuffer[512];
    u8 unk_984[2][512];
    u16 unk_D84[2];
    u8 unk_D88;
    Pokemon *partnersMon;
    u8 msgsReceived;
    u32 unk_D94;
} BattleHall;

u8 ov80_0223787C(u8 challengeType);

BOOL ov80_0223792C(u8 challengeType);

enum BattleFrontierStatsIndex sub_0205C0CC(u8 challengeType);

int sub_0205C268(u32 statIndex);

FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);

u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

typedef struct SaveVarsFlags SaveVarsFlags;

SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);

u16 Save_VarsFlags_GetVar4052(SaveVarsFlags *varsFlags);

u16 ov80_022318D0(SaveData *saveData, u8 challengeType, u8 type, u16 *type1Rank, u16 *type2Rank);

void ov80_02231930(SaveData *saveData, u8 challengeType, u8 type, u8 value);

BattleHall *ov80_022310C4(SaveData *saveData, u16 resumingFromSave, u8 challengeType, u8 partySlot1, u8 partySlot2);

BattleHall *ov80_022310C4(SaveData *saveData, u16 resumingFromSave, u8 challengeType, u8 partySlot1, u8 partySlot2) {
    u16 i;

    static BattleHall *battleHall;
    battleHall = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleHall));

    MI_CpuClear8(battleHall, sizeof(BattleHall));

    battleHall->hallSave = sub_02030B04(saveData);
    battleHall->saveData = saveData;
    battleHall->unused = 11;
    battleHall->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    battleHall->partnersMon = AllocMonZeroed(HEAP_ID_FIELD2);

    BattleHallSave *currentChallenge = battleHall->hallSave;
    BattleHallStreakFlags *streakFlags = sub_02030C5C(saveData);

    u8 streakActive, partySize;
    if (!resumingFromSave) {
        battleHall->challengeType = challengeType;
        partySize = ov80_0223787C(battleHall->challengeType);
        battleHall->currentBattle = 0;

        sub_02030AF8(currentChallenge);

        if (battleHall->challengeType == 3) {
            streakActive = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(battleHall->saveData));
        } else {
            streakActive = sub_02030CA0(streakFlags, 5, battleHall->challengeType, 0, 0L);
        }

        if (streakActive == 1) {
            battleHall->currentStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(battleHall->saveData), sub_0205C0CC(battleHall->challengeType), sub_0205C268(sub_0205C0CC(battleHall->challengeType)));
        } else {
            battleHall->currentStreak = 0;

            for (i = 0; i < NUM_POKEMON_TYPES; i++) {
                ov80_02231930(battleHall->saveData, battleHall->challengeType, i, 0);
            }
        }

        battleHall->partySlots[0] = partySlot1;
        battleHall->partySlots[1] = partySlot2;
    } else {
        battleHall->challengeType = sub_02030B88(currentChallenge, 0, 0, 0, 0L);
        partySize = ov80_0223787C(battleHall->challengeType);
        battleHall->currentBattle = sub_02030B88(currentChallenge, 1, 0, 0, 0L);
        battleHall->currentStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(battleHall->saveData), sub_0205C0CC(battleHall->challengeType), sub_0205C268(sub_0205C0CC(battleHall->challengeType)));

        for (i = 0; i < partySize; i++) {
            battleHall->partySlots[i] = sub_02030B88(currentChallenge, 3, i, 0, 0L);
        }

        for (i = 0; i < 10 * 2; i++) {
            battleHall->trainerIDs[i] = sub_02030B88(currentChallenge, 2, i, 0, 0L);
        }

        for (i = 0; i < 10 * 2; i++) {
            battleHall->monIndices[i] = (u8)sub_02030B88(currentChallenge, 4, i, 0, 0L);
        }
    }

    for (i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(battleHall->saveData), battleHall->partySlots[i]);
        battleHall->heldItems[i] = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);
    }

    battleHall->unk_10 = 0;
    battleHall->currentRound = battleHall->currentStreak / 10;

    if (battleHall->challengeType == 2) {
        for (i = 0; i < NUM_POKEMON_TYPES; i++) {
            sub_02030BF4(i, &battleHall->typeRanks[2][0], 10 - 1);
        }
    } else {
        for (i = 0; i < NUM_POKEMON_TYPES; i++) {
            u16 ptr1, ptr2;
            u16 rank = ov80_022318D0(saveData, battleHall->challengeType, i, &ptr1, &ptr2);

            sub_02030BF4(i, &battleHall->typeRanks[battleHall->challengeType][0], rank);
        }
    }

    if (ov80_0223792C(battleHall->challengeType) == 1) {
        ov80_0222A840(battleHall->saveData);
    }

    return battleHall;
}
