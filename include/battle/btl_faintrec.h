// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_faintrec.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_FAINTREC_H
#define BWPORT_BATTLE_BTL_FAINTREC_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BTL_FAINTREC_TURNS 4
#define BTL_FAINTREC_MAX_MONS 24

// Black 2's, not checked against Black
// The mons that fainted in one turn, in the order they did
struct BtlFaintRecTurn {
    u8 count;
    u8 handled[BTL_FAINTREC_MAX_MONS]; // set once the server flow has dealt with the faint
    u8 monIds[BTL_FAINTREC_MAX_MONS];
};

// Black 2's, not checked against Black
// Turn 0 is the current turn, 1 the one before it, and so on
struct BtlFaintRec {
    BtlFaintRecTurn turns[BTL_FAINTREC_TURNS];
};

// Black 2's, not checked against Black
#define BTL_FAINTREC_NO_MON 0x1f

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlFaintRec_GetCount
// How many mons fainted in a turn
u32 data_ov095_0689d175(BtlFaintRec *rec, u32 turn);
// pokebw2's BtlFaintRec_GetMonId
// The mon that fainted in a turn at an index, or BTL_FAINTREC_NO_MON
u8 data_ov095_0689d185(BtlFaintRec *rec, u32 turn, u8 index);
// pokebw2's BtlFaintRec_Init
// Empties the record of all turns
void data_ov095_0689d119(BtlFaintRec *rec);
// pokebw2's BtlFaintRec_NextTurn
// Moves every turn one back, and starts an empty current turn
void data_ov095_0689d135(BtlFaintRec *rec);
// pokebw2's BtlFaintRec_Add
// Records a mon that fainted in the current turn
void data_ov095_0689d15d(BtlFaintRec *rec, u8 monId);
// pokebw2's BtlFaintRec_IsHandled
BOOL data_ov095_0689d1a1(BtlFaintRec *rec, u32 turn, u8 index);
// pokebw2's BtlFaintRec_SetHandled
void data_ov095_0689d1bd(BtlFaintRec *rec, u32 turn, u8 index);

#endif // BWPORT_BATTLE_BTL_FAINTREC_H
