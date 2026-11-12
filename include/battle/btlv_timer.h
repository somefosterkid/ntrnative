// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_timer.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_TIMER_H
#define BWPORT_BATTLE_BTLV_TIMER_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's BtlvTimer_Delete
void func_ov094_022092e8(BtlvTimer *timer);
// pokebw2's BtlvTimer_Start
// Creates the digits of each countdown whose limit isn't 0, at most 5999 seconds (99:59), starts both and hides them
void func_ov094_02209318(BtlvTimer *timer, s32 battleLimit, s32 commandLimit);
// pokebw2's BtlvTimer_SetVisible
// Shows or hides a countdown, restarting it from its limit when both visible and restart are TRUE
void func_ov094_022094bc(BtlvTimer *timer, int which, BOOL visible, BOOL restart);
// pokebw2's BtlvTimer_IsTimeUp
// Whether a countdown has run out
BOOL func_ov094_02209500(BtlvTimer *timer, int which);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvTimer_Create
BtlvTimer *func_ov094_02209224(HeapID heapId);

#endif // BWPORT_BATTLE_BTLV_TIMER_H
