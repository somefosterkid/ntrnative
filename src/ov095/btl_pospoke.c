// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_pospoke.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_pospoke.h"
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"

static void func_ov095_0689d258(BtlPosPoke *posPoke, BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 pos);
static void func_ov095_0689d448(BtlPosPoke *posPoke, u8 pos, BtlPokeCon *pokeCon);
static void func_ov095_0689d544(BtlPosPoke *posPoke, u8 pos);

// pokebw2's BtlPosPoke_Init
void data_ov095_0689d1fd(BtlPosPoke *posPoke, BtlMainModule *mainModule, BtlPokeCon *pokeCon, u32 battleStyle) {
    u32 i;
    u32 numPos;
    u32 pos;

    for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
        posPoke->entries[i].exists = FALSE;
        posPoke->entries[i].monId = BTL_POSPOKE_NO_MON;
    }
    for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
        posPoke->posOfMon[i] = BTL_POS_NONE;
    }
    numPos = GetValidPosMax(mainModule);
    for (pos = 0; pos <= numPos; pos++) {
        func_ov095_0689d258(posPoke, mainModule, pokeCon, pos);
    }
}

// pokebw2's BtlPosPoke_SetupPos
static void func_ov095_0689d258(BtlPosPoke *posPoke, BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 pos) {
    u8 clientId;
    u8 index;
    BattleMon *mon;

    func_ov093_021b8e28(mainModule, pos, &clientId, &index);
    mon = GetClientMonData(pokeCon, clientId, index);
    if (mon != NULL && !IsFainted(mon)) {
        u8 monId = GetMonID(mon);

        posPoke->entries[pos].monId = monId;
        posPoke->posOfMon[monId] = pos;
    } else {
        posPoke->entries[pos].monId = BTL_POSPOKE_NO_MON;
    }
    posPoke->entries[pos].clientId = clientId;
    posPoke->entries[pos].exists = TRUE;
}

// pokebw2's BtlPosPoke_RemoveMon
void data_ov095_0689d2f9(BtlPosPoke *posPoke, u8 monId) {
    u32 i;

    for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
        if (posPoke->entries[i].exists && posPoke->entries[i].monId == monId) {
            posPoke->entries[i].monId = BTL_POSPOKE_NO_MON;
            return;
        }
    }
}

// pokebw2's BtlPosPoke_SetMon
void data_ov095_0689d321(BtlPosPoke *posPoke, u8 pos, u8 monId, BtlPokeCon *pokeCon) {
    posPoke->entries[pos].monId = monId;
    posPoke->posOfMon[monId] = pos;
    func_ov095_0689d448(posPoke, pos, pokeCon);
}

// pokebw2's BtlPosPoke_Rotate
void data_ov095_0689d339(BtlPosPoke *posPoke, u8 slot, u8 clientId, BattleMon *mon, BtlPokeCon *pokeCon) {
    u8 positions[5];
    u32 i;
    u32 count;
    u8 from;
    u32 to;
    BtlPosPokeEntry first;

    if (slot > 1) {
        count = 0;
        for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
            if (posPoke->entries[i].clientId == clientId) {
                positions[count++] = i;
            }
        }
        from = func_ov093_021b9ab0(slot);
        to = func_ov093_021b9ac8(slot);
        first = posPoke->entries[positions[0]];
        posPoke->entries[positions[from]] = posPoke->entries[positions[to]];
        posPoke->entries[positions[to]] = first;
        if (!IsFainted(mon)) {
            data_ov095_0689d321(posPoke, positions[0], GetMonID(mon), pokeCon);
        } else {
            // BUG: i is BTL_POSPOKE_NUM_POS here, so this writes past the entries, over posOfMon
#ifdef BUGFIX
            posPoke->entries[positions[0]].monId = BTL_POSPOKE_NO_MON;
#else
            posPoke->entries[i].monId = BTL_POSPOKE_NO_MON;
#endif
        }
        for (i = 0; i < count; i++) {
            func_ov095_0689d544(posPoke, positions[i]);
        }
    }
}

// pokebw2's BtlPosPoke_RecordOpponents
static void func_ov095_0689d448(BtlPosPoke *posPoke, u8 pos, BtlPokeCon *pokeCon) {
    u32 i;
    u8 monId = posPoke->entries[pos].monId;

    if (monId != BTL_POSPOKE_NO_MON) {
        for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
            BtlPosPokeEntry *other = &posPoke->entries[i];

            if (other->monId != BTL_POSPOKE_NO_MON && monId != other->monId && !IsAllyMonID(other->monId, monId)) {
                func_ov093_021d6dcc(GetPokeParam(pokeCon, other->monId), monId);
                func_ov093_021d6dcc(GetPokeParam(pokeCon, monId), other->monId);
            }
        }
    }
}

// pokebw2's BtlPosPoke_Swap
void data_ov095_0689d4f1(BtlPosPoke *posPoke, u8 firstPos, u8 secondPos) {
    BtlPosPokeEntry tmp = posPoke->entries[firstPos];

    posPoke->entries[firstPos] = posPoke->entries[secondPos];
    posPoke->entries[secondPos] = tmp;
    func_ov095_0689d544(posPoke, firstPos);
    func_ov095_0689d544(posPoke, secondPos);
}

// pokebw2's BtlPosPoke_UpdatePosOfMon
static void func_ov095_0689d544(BtlPosPoke *posPoke, u8 pos) {
    u8 monId = posPoke->entries[pos].monId;

    if (monId != BTL_POSPOKE_NO_MON) {
        posPoke->posOfMon[monId] = pos;
    }
}

// pokebw2's BtlPosPoke_GetEmptyPositions
u8 data_ov095_0689d559(BtlPosPoke *posPoke, u8 clientId, u8 *positions) {
    u32 count = 0;
    u32 i;

    for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
        if (posPoke->entries[i].exists && posPoke->entries[i].clientId == clientId
            && posPoke->entries[i].monId == BTL_POSPOKE_NO_MON) {
            positions[count++] = i;
        }
    }
    return count;
}

BOOL DoesBattleMonExist(BtlPosPoke *posPoke, u8 monId) {
    if (GetBattlePos(posPoke, monId) != BTL_POS_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlPosPoke_IsMonAtActivePos
BOOL data_ov095_0689d59d(BtlPosPoke *posPoke, BtlMainModule *mainModule, u8 monId) {
    u8 pos = GetBattlePos(posPoke, monId);

    if (pos != BTL_POS_NONE) {
        return func_ov093_021b86a0(mainModule, pos);
    }
    return FALSE;
}

u8 GetBattlePos(BtlPosPoke *posPoke, u8 monId) {
    u32 i;

    for (i = 0; i < BTL_POSPOKE_NUM_POS; i++) {
        if (posPoke->entries[i].exists && posPoke->entries[i].monId == monId) {
            return i;
        }
    }
    return BTL_POS_NONE;
}

// pokebw2's BtlPosPoke_GetPosOfMon
u8 data_ov095_0689d5f5(BtlPosPoke *posPoke, u8 monId) {
    return posPoke->posOfMon[monId];
}

u8 GetExistPokeID(BtlPosPoke *posPoke, u8 pos) {
    return posPoke->entries[pos].monId;
}
