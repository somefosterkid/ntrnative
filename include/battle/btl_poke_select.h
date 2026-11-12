// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_poke_select.h: the lines in use
// so far.
#ifndef BWPORT_BATTLE_BTL_POKE_SELECT_H
#define BWPORT_BATTLE_BTL_POKE_SELECT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define POKE_LIST_MAX_SLOTS 6
#define POKE_SELECT_MAX 3

// Black 2's, not checked against Black
// What the party list screen shows: the party, how many Pokemon it lists, a state for each of them and the mode
struct BtlvPokeListCmd {
    BattleParty *party;
    u8 count;
    u8 slotState[POKE_LIST_MAX_SLOTS];
    u8 mode;
};

// Black 2's, not checked against Black
// The Pokemon chosen on the party list, in order, each with the battle position it replaces
struct BtlvPokeSelectParam {
    u8 partyIndex[POKE_SELECT_MAX];
    u8 position[POKE_SELECT_MAX];
    u8 count;
    u8 max;
    u8 selecting;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PokeSelect_IsSelecting
void data_ov095_0689cb09(BtlvPokeSelectParam *select);
BOOL data_ov095_0689cb15(BtlvPokeSelectParam *select);
// pokebw2's PokeSelect_GetLast
u8 data_ov095_0689cb1d(BtlvPokeSelectParam *select);
// pokebw2's PokeListCmd_Init
void data_ov095_0689ca99(BtlvPokeListCmd *cmd, BattleParty *party, u8 count, u8 mode);
// pokebw2's PokeListCmd_SetCoverSlots
void data_ov095_0689cab1(BtlvPokeListCmd *cmd, u8 numCoverPos);
// pokebw2's PokeSelect_Init
void data_ov095_0689cae1(BtlvPokeSelectParam *select, const BtlvPokeListCmd *cmd);
// pokebw2's PokeSelect_GetCount
u32 data_ov095_0689cb19(BtlvPokeSelectParam *select);
// pokebw2's PokeSelect_Push
void data_ov095_0689caed(BtlvPokeSelectParam *select, u8 index, u8 slot);

// Black's Thumb stubs (bx pc) that switch to the ARM code calling the functions above
void func_ov093_021ce7c4(BtlvPokeSelectParam *select);
BOOL func_ov093_021cee98(BtlvPokeSelectParam *select);
u8 func_ov093_021ceea8(BtlvPokeSelectParam *select);
void func_ov093_021d0b3c(BtlvPokeListCmd *cmd, BattleParty *party, u8 count, u8 mode);
void func_ov093_021d0b4c(BtlvPokeListCmd *cmd, u8 numCoverPos);
void func_ov093_021d0b5c(BtlvPokeSelectParam *select, const BtlvPokeListCmd *cmd);
BOOL func_ov093_021d121c(BtlvPokeSelectParam *select);
u8 func_ov093_021d122c(BtlvPokeSelectParam *select);

#endif // BWPORT_BATTLE_BTL_POKE_SELECT_H
