// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_faintrec.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_faintrec.h"
#include "types.h"

static void func_ov095_0689d0fc(BtlFaintRecTurn *turn);

// pokebw2's BtlFaintRecTurn_Init
static void func_ov095_0689d0fc(BtlFaintRecTurn *turn) {
    u32 i;

    turn->count = 0;
    for (i = 0; i < BTL_FAINTREC_MAX_MONS; i++) {
        turn->monIds[i] = BTL_FAINTREC_NO_MON;
        turn->handled[i] = FALSE;
    }
}

// pokebw2's BtlFaintRec_Init
void data_ov095_0689d119(BtlFaintRec *rec) {
    u32 i;

    for (i = 0; i < BTL_FAINTREC_TURNS; i++) {
        func_ov095_0689d0fc(&rec->turns[i]);
    }
}

// pokebw2's BtlFaintRec_NextTurn
void data_ov095_0689d135(BtlFaintRec *rec) {
    u32 i;

    for (i = BTL_FAINTREC_TURNS - 1; i > 0; i--) {
        rec->turns[i] = rec->turns[i - 1];
    }
    func_ov095_0689d0fc(&rec->turns[0]);
}

// pokebw2's BtlFaintRec_Add
void data_ov095_0689d15d(BtlFaintRec *rec, u8 monId) {
    u8 count = rec->turns[0].count;

    if (count < BTL_FAINTREC_MAX_MONS) {
        rec->turns[0].monIds[count] = monId;
        rec->turns[0].handled[count] = FALSE;
        rec->turns[0].count++;
    }
}

// pokebw2's BtlFaintRec_GetCount
u32 data_ov095_0689d175(BtlFaintRec *rec, u32 turn) {
    if (turn < BTL_FAINTREC_TURNS) {
        return rec->turns[turn].count;
    }
    return 0;
}

// pokebw2's BtlFaintRec_GetMonId
u8 data_ov095_0689d185(BtlFaintRec *rec, u32 turn, u8 index) {
    if (turn < BTL_FAINTREC_TURNS && index < rec->turns[turn].count) {
        return rec->turns[turn].monIds[index];
    }
    return BTL_FAINTREC_NO_MON;
}

// pokebw2's BtlFaintRec_IsHandled
BOOL data_ov095_0689d1a1(BtlFaintRec *rec, u32 turn, u8 index) {
    if (turn < BTL_FAINTREC_TURNS && index < rec->turns[turn].count) {
        return rec->turns[turn].handled[index];
    }
    return FALSE;
}

// pokebw2's BtlFaintRec_SetHandled
void data_ov095_0689d1bd(BtlFaintRec *rec, u32 turn, u8 index) {
    if (turn < BTL_FAINTREC_TURNS && index < rec->turns[turn].count) {
        rec->turns[turn].handled[index] = TRUE;
    }
}

// pokebw2's BtlFaintRec_GetLastFainted
u8 data_ov095_0689d1d5(BtlFaintRec *rec) {
    u32 i;

    for (i = 0; i < BTL_FAINTREC_TURNS; i++) {
        if (rec->turns[i].count != 0) {
            return rec->turns[i].monIds[rec->turns[i].count - 1];
        }
    }
    return BTL_FAINTREC_NO_MON;
}
