#include "global.h"

#include "overlay_80_022310C4.h"
#include "player_avatar.h"
#include "save.h"

struct BattleFrontierSave;
struct BattleHallSave;
struct FieldBattleDTO;
struct Party;
struct Pokemon;
enum BattleFrontierStatsIndex {
    STAT_TOWER_RECORD_STREAK_SINGLE = 0,
    STAT_TOWER_LATEST_STREAK_SINGLE,
    STAT_TOWER_RECORD_STREAK_DOUBLE,
    STAT_TOWER_LATEST_STREAK_DOUBLE,
    STAT_TOWER_RECORD_STREAK_MULTI,
    STAT_TOWER_LATEST_STREAK_MULTI,
    STAT_TOWER_RECORD_STREAK_LINK_MULTI,
    STAT_TOWER_LATEST_STREAK_LINK_MULTI,
    STAT_TOWER_RECORD_STREAK_WIFI,
    STAT_TOWER_LATEST_STREAK_WIFI,
    STAT_FACTORY_RECORD_STREAK_50_SINGLE,
    STAT_FACTORY_LATEST_STREAK_50_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_SINGLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_RECORD_STREAK_50_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_50_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_50_MULTI,
    STAT_FACTORY_LATEST_STREAK_50_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI,
    STAT_HALL_CURRENT_STREAK_SINGLE,
    STAT_HALL_LATEST_STREAK_SINGLE,
    STAT_HALL_LATEST_SPECIES_SINGLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_SINGLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_SINGLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_SINGLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_SINGLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_SINGLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_SINGLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_SINGLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_SINGLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_SINGLE,
    STAT_HALL_CURRENT_STREAK_DOUBLE,
    STAT_HALL_LATEST_STREAK_DOUBLE,
    STAT_HALL_LATEST_SPECIES_DOUBLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_DOUBLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_DOUBLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_DOUBLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_DOUBLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_DOUBLE,
    STAT_HALL_CURRENT_STREAK_MULTI,
    STAT_HALL_LATEST_STREAK_MULTI,
    STAT_HALL_LATEST_SPECIES_MULTI,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI,
    STAT_CASTLE_RECORD_STREAK_SINGLE,
    STAT_CASTLE_LATEST_STREAK_SINGLE,
    STAT_CASTLE_LATEST_CP_SINGLE,
    STAT_CASTLE_SPENT_CP_SINGLE,
    STAT_CASTLE_RECORD_CP_SINGLE,
    STAT_CASTLE_RANK_HEALING_SINGLE,
    STAT_CASTLE_RANK_RENTALS_SINGLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_SINGLE,
    STAT_CASTLE_RECORD_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_CP_DOUBLE,
    STAT_CASTLE_SPENT_CP_DOUBLE,
    STAT_CASTLE_RECORD_CP_DOUBLE,
    STAT_CASTLE_RANK_HEALING_DOUBLE,
    STAT_CASTLE_RANK_RENTALS_DOUBLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_DOUBLE,
    STAT_CASTLE_RECORD_STREAK_MULTI,
    STAT_CASTLE_LATEST_STREAK_MULTI,
    STAT_CASTLE_LATEST_CP_MULTI,
    STAT_CASTLE_SPENT_CP_MULTI,
    STAT_CASTLE_RECORD_CP_MULTI,
    STAT_CASTLE_RANK_HEALING_MULTI,
    STAT_CASTLE_RANK_RENTALS_MULTI,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI,
    STAT_ARCADE_RECORD_STREAK_SINGLE,
    STAT_ARCADE_LATEST_STREAK_SINGLE,
    STAT_ARCADE_RECORD_STREAK_DOUBLE,
    STAT_ARCADE_LATEST_STREAK_DOUBLE,
    STAT_ARCADE_RECORD_STREAK_MULTI,
    STAT_ARCADE_LATEST_STREAK_MULTI,

    STATS_NUM_SOLO_STATS,

    STAT_TOWER_WFC_STREAK_ACTIVE = STATS_NUM_SOLO_STATS,
    STAT_TOWER_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE_2,
    STAT_HALL_WFC_STREAK_ACTIVE,
    STAT_HALL_WFC_STREAK_ACTIVE_2,
    STAT_CASTLE_WFC_STREAK_ACTIVE,
    STAT_CASTLE_WFC_STREAK_ACTIVE_2,
    STAT_ARCADE_WFC_STREAK_ACTIVE,
    STAT_ARCADE_WFC_STREAK_ACTIVE_2,

    STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,

    STAT_TOWER_RECORD_STREAK_MODE_6 = STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,
    STAT_TOWER_LATEST_STREAK_MODE_6,
    STAT_FACTORY_RECORD_STREAK_50_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_50_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_HALL_CURRENT_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_SPECIES_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI_WFC,
    STAT_CASTLE_RECORD_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_CP_MULTI_WFC,
    STAT_CASTLE_SPENT_CP_MULTI_WFC,
    STAT_CASTLE_RECORD_CP_MULTI_WFC,
    STAT_CASTLE_RANK_HEALING_MULTI_WFC,
    STAT_CASTLE_RANK_RENTALS_MULTI_WFC,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI_WFC,
    STAT_ARCADE_RECORD_STREAK_MULTI_WFC,
    STAT_ARCADE_LATEST_STREAK_MULTI_WFC,

    STATS_NUM_STATS_TOTAL,
};
typedef struct BattleFrontierSave BattleFrontierSave;
enum BattleFrontierStatsIndex sub_0205C0A0(u8 challengeType, u8 pokemonType);
BattleFrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
u16 sub_02031108(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
typedef struct Pokemon Pokemon;
enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
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
typedef struct FieldBattleDTO FieldBattleDTO;
typedef struct FrontierTrainer {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} FrontierTrainer;
typedef struct FrontierPokemon {
    struct {
        u16 species : 11;
        u16 form : 5;
    };
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 personality;
    union {
        struct {
            u32 hpIV : 5;
            u32 atkIV : 5;
            u32 defIV : 5;
            u32 speedIV : 5;
            u32 spAtkIV : 5;
            u32 spDefIV : 5;
            u32 setSpeciesAsNickname : 1;
            u32 : 1;
        };
        u32 combinedIVs;
    };
    union {
        struct {
            u8 hpEV;
            u8 atkEV;
            u8 defEV;
            u8 speedEV;
            u8 spAtkEV;
            u8 spDefEV;
        };
        u8 evList[STAT_MAX];
    };
    union {
        struct {
            u8 move0PPUps : 2;
            u8 move1PPUps : 2;
            u8 move2PPUps : 2;
            u8 move3PPUps : 2;
        };
        u8 combinedPPUps;
    };
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[10 + 1];
} FrontierPokemon;
typedef struct FrontierOpponent {
    FrontierTrainer trainer;
    FrontierPokemon pokemon[4];
} FrontierOpponent;
typedef struct BattleHallSave BattleHallSave;
u8 sub_02030BD0(u8 idx, u8 *typeRanks);
void sub_02030BF4(u8 pokemonType, u8 *typeRanks, u8 value);
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
u16 ov80_022379C8(BattleHall *battleHall);
fx32 ov80_02237A40(u32 level);

void ov80_022319B0(BattleHall *battleHall);
void ov80_02231A04(BattleHall *battleHall);
u16 ov80_02231A14(BattleHall *battleHall);

u16 ov80_022318D0(SaveData *saveData, u8 challengeType, u8 type, u16 *type1Rank, u16 *type2Rank) {
    u16 rankPair = FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), sub_0205C0A0(challengeType, type), sub_0205C268(sub_0205C0A0(challengeType, type)));
    rankPair &= 0xff;

    *type1Rank = rankPair & 0xf;
    *type2Rank = rankPair >> 4;

    if (type % 2 == 0) {
        return *type1Rank;
    } else {
        return *type2Rank;
    }
}

void ov80_02231930(SaveData *saveData, u8 challengeType, u8 type, u8 value) {
    u16 type1Rank, type2Rank;
    ov80_022318D0(saveData, challengeType, type, &type1Rank, &type2Rank);

    u8 currentRanks = (type2Rank << 4) | type1Rank;
    u8 nybble = type % 2;

    if (nybble == 0) {
        currentRanks = currentRanks & 0xf0;
    } else {
        currentRanks = currentRanks & 0xf;
    }

    u8 newRank = value << (4 * nybble);
    currentRanks |= newRank;

    sub_02031108(Save_Frontier_GetStatic(saveData), sub_0205C0A0(challengeType, type), sub_0205C268(sub_0205C0A0(challengeType, type)), currentRanks);
}

void ov80_022319B0(BattleHall *battleHall) {
    int type;

    if (battleHall->challengeType != 2) {
        for (type = 0; type < NUM_POKEMON_TYPES - 1; type++) {
            u8 rank = sub_02030BD0(type, &battleHall->typeRanks[battleHall->challengeType][0]);

            if (rank < 10) {
                break;
            }
        }

        if (type == NUM_POKEMON_TYPES - 1) {
            for (type = 0; type < NUM_POKEMON_TYPES - 1; type++) {
                sub_02030BF4(type, &battleHall->typeRanks[battleHall->challengeType][0], 10 - 1);
            }
        }
    }
}

void ov80_02231A04(BattleHall *battleHall) {
    int level = ov80_022379C8(battleHall);
    battleHall->playerLevelSqrt = ov80_02237A40(level);
}

u16 ov80_02231A14(BattleHall *battleHall) {
    fx32 baseLevel;
    int type;

    u8 typeRank = sub_02030BD0(battleHall->selectedTypeIdx, &battleHall->typeRanks[battleHall->challengeType][0]);
    int playersLevel = ov80_022379C8(battleHall);

    if (battleHall->challengeType == 2) {
        return playersLevel;
    }

    baseLevel = ((fx32)(((playersLevel) > 0) ? ((playersLevel) * (1 << 12) + 0.5f) : ((playersLevel) * (1 << 12) - 0.5f))) - battleHall->playerLevelSqrt * 3;
    float rankFactor = ((f32)((battleHall->playerLevelSqrt) / (f32)(1 << 12))) * 5.0;

    if (playersLevel / rankFactor < 1.0) {
        rankFactor = typeRank + 1 - 1;
    } else {
        fx32 product = ((fx32)(((typeRank * playersLevel) > 0) ? ((typeRank * playersLevel) * (1 << 12) + 0.5f) : ((typeRank * playersLevel) * (1 << 12) - 0.5f)));
        rankFactor = ((f32)((product) / (f32)(1 << 12))) / rankFactor;
    }

    float numBattledTypesFactor = 0.0;

    for (type = 0; type < NUM_POKEMON_TYPES; type++) {
        if (type == battleHall->selectedTypeIdx) {
            numBattledTypesFactor += 1.0;
        } else {
            if (sub_02030BD0(type, &battleHall->typeRanks[battleHall->challengeType][0]) > 0) {
                numBattledTypesFactor += 1.0;
            }
        }
    }

    if (numBattledTypesFactor != 0.0) {
        numBattledTypesFactor -= 1.0;
    }

    numBattledTypesFactor *= 0.5;

    float opponentsLevelFloat = ((f32)((baseLevel) / (f32)(1 << 12))) + rankFactor + numBattledTypesFactor;

    int opponentsLevel = (int)(opponentsLevelFloat);
    if (opponentsLevelFloat != (int)opponentsLevelFloat) {
        opponentsLevel++;
    }

    if (opponentsLevel > playersLevel) {
        opponentsLevel = playersLevel;
    }

    if (opponentsLevel > 100) {
        opponentsLevel = 100;
    }

    return opponentsLevel;
}
