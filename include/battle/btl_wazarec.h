// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_wazarec.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_WAZAREC_H
#define BWPORT_BATTLE_BTL_WAZAREC_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BTL_WAZAREC_MAX 120

// Black 2's, not checked against Black
struct BtlWazaRecord {
    u32 turn;
    u16 move;
    u8 monId;
    u8 succeeded; // set when the move took effect (BtlWazaRec_MarkLastSucceeded)
};

// Black 2's, not checked against Black
// The newest record is the one before head; head wraps around over the oldest
struct BtlWazaRec {
    u32 head;
    BtlWazaRecord records[BTL_WAZAREC_MAX];
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlWazaRec_IsUsed
// Whether a move was used in a turn
BOOL data_ov095_0689d065(BtlWazaRec *rec, u16 move, u32 turn);
// How many times a move was used in a turn
u32 GetUsedMoveCount(BtlWazaRec *rec, u16 move, u32 turn);
// pokebw2's BtlWazaRec_GetSucceededMove
// The newest move of a turn that took effect, or MOVE_NONE
u16 data_ov095_0689d0c9(BtlWazaRec *rec, u32 turn);
// pokebw2's BtlWazaRec_Init
// Empties the ring
void data_ov095_0689cff1(BtlWazaRec *rec);
// pokebw2's BtlWazaRec_Add
// Records a use of a move by a mon in a turn
void data_ov095_0689d01d(BtlWazaRec *rec, u16 move, u32 turn, u8 monId);
// pokebw2's BtlWazaRec_MarkLastSucceeded
// Marks the newest record as a move that took effect
void data_ov095_0689d051(BtlWazaRec *rec);

#endif // BWPORT_BATTLE_BTL_WAZAREC_H
