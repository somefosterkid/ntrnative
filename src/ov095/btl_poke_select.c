// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_poke_select.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_poke_select.h"
#include "types.h"

static void func_ov095_0689cad8(BtlvPokeListCmd *cmd, u8 state, u8 slot);

// pokebw2's PokeListCmd_Init
void data_ov095_0689ca99(BtlvPokeListCmd *cmd, BattleParty *party, u8 count, u8 mode) {
    u32 i;

    cmd->mode = mode;
    cmd->count = count;
    cmd->party = party;
    for (i = 0; i < POKE_LIST_MAX_SLOTS; i++) {
        cmd->slotState[i] = 0;
    }
}

// pokebw2's PokeListCmd_SetCoverSlots
void data_ov095_0689cab1(BtlvPokeListCmd *cmd, u8 numCoverPos) {
    u8 i;

    for (i = 0; i < numCoverPos; i++) {
        func_ov095_0689cad8(cmd, 2, i);
    }
}

// pokebw2's PokeListCmd_SetSlotState
static void func_ov095_0689cad8(BtlvPokeListCmd *cmd, u8 state, u8 slot) {
    cmd->slotState[slot] = state;
}

// pokebw2's PokeSelect_Init
void data_ov095_0689cae1(BtlvPokeSelectParam *select, const BtlvPokeListCmd *cmd) {
    select->count = 0;
    select->max = cmd->count;
}

// pokebw2's PokeSelect_Push
void data_ov095_0689caed(BtlvPokeSelectParam *select, u8 index, u8 slot) {
    if (select->count < select->max) {
        select->partyIndex[select->count] = slot;
        select->position[select->count] = index;
        select->count++;
    }
}

// pokebw2's PokeSelect_Pop
void data_ov095_0689cb09(BtlvPokeSelectParam *select) {
    if (select->count != 0) {
        select->count--;
    }
}

// pokebw2's PokeSelect_IsSelecting
BOOL data_ov095_0689cb15(BtlvPokeSelectParam *select) {
    return select->selecting;
}

// pokebw2's PokeSelect_GetCount
u32 data_ov095_0689cb19(BtlvPokeSelectParam *select) {
    return select->count;
}

// pokebw2's PokeSelect_GetLast
u8 data_ov095_0689cb1d(BtlvPokeSelectParam *select) {
    if (select->count != 0) {
        return select->partyIndex[select->count - 1];
    }
    return POKE_LIST_MAX_SLOTS;
}

// pokebw2's PokeSelect_Get
u8 data_ov095_0689cb2d(BtlvPokeSelectParam *select, u8 index) {
    if (index < select->count) {
        return select->partyIndex[index];
    }
    return POKE_LIST_MAX_SLOTS;
}
