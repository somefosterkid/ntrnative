#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "unk_02030A98.h"
#include "overlay_80_battle_arcade.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"
#include "save_frontier.h"

struct BattleArcadeSave;
struct BattleCastleSave;
struct FieldBattleDTO;
struct Party;
BOOL sub_02037030(int cmd, const void *data, int size);
u16 sub_0203769C(void);
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
int SizeOfStructPokemon(void);
u8 ov80_02237B24(u8 challengeType, BOOL includePartnersMons);
BOOL sub_02036FD8(int cmd, const void *data, int size);
BOOL Party_AddMon(Party *party, Pokemon *mon);
BOOL Party_SwapSlots(Party *party, int slotA, int slotB);
Pokemon *AllocMonZeroed(enum HeapID heapID);
u8 BattleArcade_GetMonCount(u8 challengeType, BOOL includePartnerMons);

BOOL ov80_0222B7E4(BattleCastle *battleCastle);
BOOL ov80_0222B8D8(BattleCastle *battleCastle, u16 param1);
BOOL ov80_0222B920(BattleCastle *battleCastle, u16 param1);
BOOL ov80_0222B968(BattleCastle *battleCastle);
BOOL ov80_0222BA7C(BattleArcade *battleArcade);
BOOL ov80_0222BAE0(BattleArcade *battleArcade);
BOOL ov80_0222BB54(BattleArcade *battleArcade);
BOOL ov80_0222BC48(BattleArcade *battleArcade, u16 param1);
BOOL ov80_0222BC94(BattleArcade *battleArcade, u16 param1);
BOOL ov80_0222BCE0(BattleArcade *battleArcade);

BOOL ov80_0222B7E4(BattleCastle *battleCastle) {
    int i, offset, result, dataSize;

    offset = 0;
    dataSize = ((int)(sizeof(battleCastle->commBuffer) / sizeof(*(battleCastle->commBuffer))));

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i] = battleCastle->monSetIDs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i + offset] = battleCastle->opponentMonIVs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i + offset] = (battleCastle->opponentMonPersonalities[i] & 0xFFFF);
        battleCastle->commBuffer[i + offset + 4] = ((battleCastle->opponentMonPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += (4 * 2);

    if (sub_02037030(45, battleCastle->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B860(int netID, int unused, void *monData, void *battleCastle) {
    int i, offset;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u16 *monData_dupe = monData;

    offset = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->monSetIDs[i] = monData_dupe[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->opponentMonIVs[i] = monData_dupe[i + offset];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->opponentMonPersonalities[i] = monData_dupe[i + offset];
        battleCastle_dupe->opponentMonPersonalities[i] |= (monData_dupe[i + offset + 4] << 16);
    }

    offset += (4 * 2);
}

BOOL ov80_0222B8D8(BattleCastle *battleCastle, u16 param1) {
    int result, dataSize = ((int)(sizeof(battleCastle->commBuffer) / sizeof(*(battleCastle->commBuffer))));
    *battleCastle->commBuffer = param1;

    if (sub_02037030(46, battleCastle->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B8F8(int netID, int unused, void *param2, void *battleCastle) {
    int dummy;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleCastle_dupe->unk_A10 = *param2_dupe;
}

BOOL ov80_0222B920(BattleCastle *battleCastle, u16 param1) {
    int result, dataSize = ((int)(sizeof(battleCastle->commBuffer) / sizeof(*(battleCastle->commBuffer))));
    battleCastle->commBuffer[0] = param1;

    if (sub_02037030(47, battleCastle->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B940(int netID, int unused, void *param2, void *battleCastle) {
    int dummy;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleCastle_dupe->unk_A11 = *param2_dupe;
}

BOOL ov80_0222B968(BattleCastle *battleCastle) {
    int i, dummy, result, dataSize, pokemonSize;
    u8 partySize;
    Pokemon *pokemon;

    dummy = 0;
    dataSize = ((int)(sizeof(battleCastle->commHugeBuffer) / sizeof(*(battleCastle->commHugeBuffer))));
    partySize = ov80_02237B24(battleCastle->challengeType, 0);
    pokemonSize = SizeOfStructPokemon();

    for (i = 0; i < partySize; i++) {
        pokemon = Party_GetMonByIndex(battleCastle->playersParty, i);
        MI_CpuCopy8(pokemon, &battleCastle->commHugeBuffer[i * pokemonSize], pokemonSize);
    }

    if (sub_02036FD8(48, battleCastle->commHugeBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B9CC(int netID, int unused, void *param2, void *battleCastle) {
    u8 partySize;
    int i, v2, pokemonSize;
    Pokemon *pokemon;
    BattleCastle *battleCastle_dupe = battleCastle;
    const u8 *v6 = param2;

    v2 = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    partySize = ov80_02237B24(battleCastle_dupe->challengeType, 0);
    pokemonSize = SizeOfStructPokemon();
    pokemon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (i = 0; i < partySize; i++) {
        MI_CpuCopy8(&v6[pokemonSize * i], pokemon, pokemonSize);
        Party_AddMon(battleCastle_dupe->playersParty, pokemon);
    }

    Heap_Free(pokemon);

    if (sub_0203769C() != 0) {
        Party_SwapSlots(battleCastle_dupe->playersParty, 0, 2);
        Party_SwapSlots(battleCastle_dupe->playersParty, 1, 3);
    }
}

u8 *ov80_0222BA5C(int index, void *battleCastle, int size) {
    BattleCastle *battleCastle_dupe = battleCastle;

    ((size <= ((int)(sizeof(battleCastle_dupe->commHugeBuffer) / sizeof(*(battleCastle_dupe->commHugeBuffer))))) ? (void)0 : GF_AssertFail());
    return battleCastle_dupe->unk_610[index];
}

BOOL ov80_0222BA7C(BattleArcade *battleArcade) {
    int dummy, result, dataSize;
    BattleArcadeStreakFlags *v4 = sub_02030FA0(battleArcade->saveData);
    dataSize = ((int)(sizeof(battleArcade->commBuffer) / sizeof(*(battleArcade->commBuffer))));
    dummy = 0;

    battleArcade->commBuffer[1] = battleArcade->currentStreak;
    battleArcade->commBuffer[2] = battleArcade->currentRound;

    dummy += 3;

    if (sub_02037030(65, battleArcade->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BAB0(int netID, int sLotoId, void *param2, void *battleArcade) {
    int dummy;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleArcade_dupe->unk_A78 = param2_dupe[1];
    battleArcade_dupe->unk_A76 = param2_dupe[2];

    dummy += 3;
}

BOOL ov80_0222BAE0(BattleArcade *battleArcade) {
    int i, dummy, result, dataSize;

    dummy = 0;
    dataSize = ((int)(sizeof(battleArcade->commBuffer) / sizeof(*(battleArcade->commBuffer))));

    for (i = 0; i < (7 * 2); i++) {
        battleArcade->commBuffer[i + dummy] = battleArcade->trainerIDs[i];
    }

    dummy += (7 * 2);

    if (sub_02037030(66, battleArcade->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BB18(int netID, int unused, void *trainerIDs, void *battleArcade) {
    int i, dummy;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u16 *trainerIDs_dupe = trainerIDs;

    dummy = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < (7 * 2); i++) {
        battleArcade_dupe->trainerIDs[i] = trainerIDs_dupe[i + dummy];
    }

    dummy += (7 * 2);
}

BOOL ov80_0222BB54(BattleArcade *battleArcade) {
    int i, offset, result, dataSize;

    offset = 0;
    dataSize = ((int)(sizeof(battleArcade->commBuffer) / sizeof(*(battleArcade->commBuffer))));

    for (i = 0; i < 4; i++) {
        battleArcade->commBuffer[i] = battleArcade->monSetIDs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleArcade->commBuffer[i + offset] = battleArcade->opponentMonIVs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleArcade->commBuffer[i + offset] = (battleArcade->opponentMonPersonalities[i] & 0xFFFF);
        battleArcade->commBuffer[i + offset + 4] = ((battleArcade->opponentMonPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += (4 * 2);

    if (sub_02037030(67, battleArcade->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BBD0(int netID, int unused, void *param2, void *battleArcade) {
    int i, offset;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u16 *param2_dupe = param2;

    offset = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < 4; i++) {
        battleArcade_dupe->monSetIDs[i] = param2_dupe[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleArcade_dupe->opponentMonIVs[i] = param2_dupe[i + offset];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleArcade_dupe->opponentMonPersonalities[i] = param2_dupe[i + offset];
        battleArcade_dupe->opponentMonPersonalities[i] |= (param2_dupe[i + offset + 4] << 16);
    }

    offset += (4 * 2);
}

BOOL ov80_0222BC48(BattleArcade *battleArcade, u16 param1) {
    int result, dataSize = ((int)(sizeof(battleArcade->commBuffer) / sizeof(*(battleArcade->commBuffer))));
    battleArcade->commBuffer[0] = param1;

    if (sub_02037030(68, battleArcade->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BC6C(int netID, int unused, void *param2, void *battleArcade) {
    int dummy;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleArcade_dupe->unk_A74 = *param2_dupe;
}

BOOL ov80_0222BC94(BattleArcade *battleArcade, u16 param1) {
    int result, dataSize = ((int)(sizeof(battleArcade->commBuffer) / sizeof(*(battleArcade->commBuffer))));
    battleArcade->commBuffer[0] = param1;

    if (sub_02037030(69, battleArcade->commBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BCB8(int netID, int unused, void *param2, void *battleArcade) {
    int dummy;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleArcade_dupe->unk_A75 = *param2_dupe;
}

BOOL ov80_0222BCE0(BattleArcade *battleArcade) {
    int i, dummy, result, dataSize, pokemonSize;
    u8 partySize;
    Pokemon *pokemon;

    dummy = 0;
    dataSize = ((int)(sizeof(battleArcade->commHugeBuffer) / sizeof(*(battleArcade->commHugeBuffer))));
    partySize = BattleArcade_GetMonCount(battleArcade->challengeType, 0);
    pokemonSize = SizeOfStructPokemon();

    for (i = 0; i < partySize; i++) {
        pokemon = Party_GetMonByIndex(battleArcade->playersParty, i);
        MI_CpuCopy8(pokemon, &battleArcade->commHugeBuffer[i * pokemonSize], pokemonSize);
    }

    if (sub_02036FD8(70, battleArcade->commHugeBuffer, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222BD44(int netID, int unused, void *partnersParty, void *battleArcade) {
    u8 partySize;
    int i, dummy, pokemonSize;
    Pokemon *pokemon;
    BattleArcade *battleArcade_dupe = battleArcade;
    const u8 *partnersParty_dupe = partnersParty;

    dummy = 0;
    battleArcade_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    partySize = BattleArcade_GetMonCount(battleArcade_dupe->challengeType, 0);
    pokemonSize = SizeOfStructPokemon();
    pokemon = AllocMonZeroed(HEAP_ID_FIELD2);

    for (i = 0; i < partySize; i++) {
        MI_CpuCopy8(&partnersParty_dupe[pokemonSize * i], pokemon, pokemonSize);
        Party_AddMon(battleArcade_dupe->playersParty, pokemon);
    }

    Heap_Free(pokemon);

    if (sub_0203769C() != 0) {
        Party_SwapSlots(battleArcade_dupe->playersParty, 0, 2);
        Party_SwapSlots(battleArcade_dupe->playersParty, 1, 3);
    }
}

u8 *ov80_0222BDD4(int netID, void *battleArcade, int size) {
    BattleArcade *battleArcade_dupe = battleArcade;

    ((size <= ((int)(sizeof(battleArcade_dupe->commHugeBuffer) / sizeof(*(battleArcade_dupe->commHugeBuffer))))) ? (void)0 : GF_AssertFail());
    return battleArcade_dupe->unk_674[netID];
}
