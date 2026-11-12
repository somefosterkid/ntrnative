// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_wazarec.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_wazarec.h"
#include "types.h"
#include "constants/moves.h"
#include "gfl/std.h"

// pokebw2's BtlWazaRec_Init
void data_ov095_0689cff1(BtlWazaRec *rec) {
    u32 i;

    MI_CpuFill8(rec, 0, sizeof(BtlWazaRec));
    rec->head = 0;
    for (i = 0; i < BTL_WAZAREC_MAX; i++) {
        rec->records[i].move = MOVE_NONE;
    }
}

// pokebw2's BtlWazaRec_Add
void data_ov095_0689d01d(BtlWazaRec *rec, u16 move, u32 turn, u8 monId) {
    rec->records[rec->head].move = move;
    rec->records[rec->head].turn = turn;
    rec->records[rec->head].monId = monId;
    rec->records[rec->head].succeeded = FALSE;
    rec->head++;
    if (rec->head >= BTL_WAZAREC_MAX) {
        rec->head = 0;
    }
}

// pokebw2's BtlWazaRec_MarkLastSucceeded
void data_ov095_0689d051(BtlWazaRec *rec) {
    int last = rec->head - 1;

    if (last < 0) {
        last = BTL_WAZAREC_MAX - 1;
    }
    rec->records[last].succeeded = TRUE;
}

// pokebw2's BtlWazaRec_IsUsed
BOOL data_ov095_0689d065(BtlWazaRec *rec, u16 move, u32 turn) {
    int i = rec->head;
    int end = rec->head;

    while (TRUE) {
        i--;
        if (i < 0) {
            i = BTL_WAZAREC_MAX - 1;
        }
        if (i == end || rec->records[i].move == MOVE_NONE) {
            break;
        }
        if (rec->records[i].move == move && rec->records[i].turn == turn) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 GetUsedMoveCount(BtlWazaRec *rec, u16 move, u32 turn) {
    int i = rec->head;
    int end = rec->head;
    u32 count = 0;

    while (TRUE) {
        i--;
        if (i < 0) {
            i = BTL_WAZAREC_MAX - 1;
        }
        if (i == end || rec->records[i].move == MOVE_NONE) {
            break;
        }
        if (rec->records[i].move == move && rec->records[i].turn == turn) {
            count++;
        }
    }
    return count;
}

// pokebw2's BtlWazaRec_GetSucceededMove
u16 data_ov095_0689d0c9(BtlWazaRec *rec, u32 turn) {
    int i = rec->head;
    int end = rec->head;

    while (TRUE) {
        i--;
        if (i < 0) {
            i = BTL_WAZAREC_MAX - 1;
        }
        if (i == end || rec->records[i].move == MOVE_NONE) {
            break;
        }
        if (rec->records[i].succeeded && rec->records[i].turn == turn) {
            return rec->records[i].move;
        }
    }
    return MOVE_NONE;
}
