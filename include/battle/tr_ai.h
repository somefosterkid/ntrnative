// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/tr_ai.h: the lines in use so far.
#ifndef BWPORT_BATTLE_TR_AI_H
#define BWPORT_BATTLE_TR_AI_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

// pokebw2's TrAI_DeleteVM
void func_ov096_02188080(VM *vm);
u8 func_ov096_02188134(VM *vm);
VM *TrAI_CreateVM(BtlMainModule *mainModule, BtlServerFlow *serverFlow, BtlPokeCon *pokeCon, u32 aiFlags, HeapID heapId);
u8 func_ov096_02188140(VM *vm);
// pokebw2's TrAI_Setup
// usableMoves has one entry per move slot, 0 if the move can't be chosen
void func_ov096_021880bc(VM *vm, const u8 *usableMoves, u8 attackerPos);
// pokebw2's TrAI_Think
// Returns TRUE if the AI ran out of time, and has to continue in the next call
BOOL func_ov096_02187fb0(VM *vm);

// Black 2's, not checked against Black
// The trainer AI, which scores each of the Pokemon's moves by running an AI script for every flag in its AI flags,
// and chooses the move with the highest score
#define OVERLAY_TR_AI OVERLAY_ID(170)

// The move that TrAI_GetChosenMove returns when the AI decides to flee
#define TRAI_MOVE_FLEE 4

#endif // BWPORT_BATTLE_TR_AI_H
