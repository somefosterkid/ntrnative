#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

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

int SizeOfStructPokemon(void);

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

BOOL sub_02036FD8(int cmd, const void *data, int size);

BOOL ov80_0222B070(BattleHall *battleHall);

BOOL ov80_0222B070(BattleHall *battleHall) {
    int dummy, result, dataSize, pokemonSize;
    Pokemon *pokemon;
    Party *party;

    dummy = 0;
    dataSize = ((int)(sizeof(battleHall->commHugeBuffer) / sizeof(*(battleHall->commHugeBuffer))));
    pokemonSize = SizeOfStructPokemon();
    party = SaveArray_Party_Get(battleHall->saveData);
    pokemon = Party_GetMonByIndex(party, battleHall->partySlots[0]);

    MI_CpuCopy8(pokemon, battleHall->commHugeBuffer, pokemonSize);

    if (sub_02036FD8(38, battleHall->commHugeBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

u16 sub_0203769C(void);

void ov80_0222B0B8(int netID, int sLotoId, void *partnersMon, void *battleHall) {
    int dummy, pokemonSize;
    BattleHall *battleHall_dupe = battleHall;
    const u8 *partnersMon_dupe = partnersMon;

    dummy = 0;
    battleHall_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    pokemonSize = SizeOfStructPokemon();
    MI_CpuCopy8(partnersMon_dupe, battleHall_dupe->partnersMon, pokemonSize);
}
