#include "global.h"

#include "overlay_80_022372D8.h"
#include "save.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

extern const u8 ov80_0223D4C0[9];

struct BattleHallSave;
struct FieldBattleDTO;
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
Party *SaveArray_Party_Get(SaveData *saveData);
u32 GetMonData(Pokemon *mon, int param, void *dest);
typedef struct BattleHallSave BattleHallSave;
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
u8 ov80_0223793C(BattleHall *battleHall);

u8 ov80_02237920(u8 spot) {
    return ov80_0223D4C0[spot];
}

BOOL ov80_0223792C(u8 challengeType) {
    return challengeType == 2 || challengeType == 3;
}

u8 ov80_0223793C(BattleHall *battleHall) {
    Party *party = SaveArray_Party_Get(battleHall->saveData);
    Pokemon *mon = Party_GetMonByIndex(party, battleHall->partySlots[0]);
    int level = GetMonData(mon, MON_DATA_LEVEL, 0L);

    return level / 10;
}
