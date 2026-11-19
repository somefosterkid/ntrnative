#include "global.h"
#include "battle_castle_save.h"

typedef struct BattleHallSave {
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 currentBattleNum;
    u8 partySlots[2];
    u16 trainerIDs[10 * 2];
    u16 monIndices[10 * 2];
} BattleHallSave;
void sub_02030BF4(u8 pokemonType, u8 *typeRanks, u8 value);
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

BOOL sub_02030B14(BattleCastleSave *save);
void sub_02030B1C(BattleCastleSave *save, BOOL didSave);
void sub_02030B30(BattleHallSave *save, u8 field, u8 index, u8 unused, void *value);
u32 sub_02030B88(BattleHallSave *save, u8 field, u8 index, u8 unused, void *unused2);
u8 sub_02030BD0(u8 idx, u8 *typeRanks);
void sub_02030BF4(u8 pokemonType, u8 *typeRanks, u8 value);
void sub_02030C34(u8 *typeRanks);
void sub_02030C50(BattleCastlePersistentSave *save);

BOOL sub_02030B14(BattleCastleSave *save) {
    return save->didSave;
}

void sub_02030B1C(BattleCastleSave *save, BOOL didSave) {
    save->didSave = didSave;
}

void sub_02030B30(BattleHallSave *save, u8 field, u8 index, u8 unused, void *value) {
    switch (field) {
    case 0:
        save->challengeType = *(u8 *)value;
        break;
    case 1:
        save->currentBattleNum = *(u8 *)value;
        break;
    case 2:
        save->trainerIDs[index] = *(u16 *)value;
        break;
    case 3:
        save->partySlots[index] = *(u8 *)value;
        break;
    case 4:
        save->monIndices[index] = *(u16 *)value;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030B88(BattleHallSave *save, u8 field, u8 index, u8 unused, void *unused2) {
    switch (field) {
    case 0:
        return save->challengeType;
    case 1:
        return save->currentBattleNum;
    case 2:
        return save->trainerIDs[index];
    case 3:
        return save->partySlots[index];
    case 4:
        return save->monIndices[index];
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

u8 sub_02030BD0(u8 idx, u8 *typeRanks) {
    u8 byte = idx / 2;
    u8 nybble = idx % 2;
    u8 rank = (typeRanks[byte] >> (4 * nybble)) & 0xf;

    return rank;
}

void sub_02030BF4(u8 pokemonType, u8 *typeRanks, u8 value) {
    u8 byte = pokemonType / 2;
    u8 nybble = pokemonType % 2;

    if (nybble == 0) {
        typeRanks[byte] = typeRanks[byte] & 0xf0;
    } else {
        typeRanks[byte] = typeRanks[byte] & 0xf;
    }

    u8 rankPair = value << (4 * nybble);
    typeRanks[byte] |= rankPair;
}

void sub_02030C34(u8 *typeRanks) {
    for (int i = 0; i < NUM_POKEMON_TYPES; i++) {
        sub_02030BF4(i, typeRanks, 0);
    }
}

void sub_02030C50(BattleCastlePersistentSave *save) {
    MI_CpuClear8(save, sizeof(BattleCastlePersistentSave));
}
