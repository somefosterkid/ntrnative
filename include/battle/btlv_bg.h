// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_bg.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTLV_BG_H
#define BWPORT_BATTLE_BTLV_BG_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's BtlvBg_StartMove
// Moves BG 3 to x, y in the effect's movement mode: 1 relative to the current offset, 4 by x, y each frame with x
// mirrored for an odd pos
void func_ov094_022097cc(BtlvBg *work, u32 pos, u32 mode, s32 x, s32 y, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvBg_IsMoving
BOOL func_ov094_0220985c(BtlvBg *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvBg_Create
BtlvBg *func_ov094_0220975c(TCBManager *tcbMgr, HeapID heapId);
// pokebw2's BtlvBg_Delete
void func_ov094_0220977c(BtlvBg *work);
// pokebw2's BtlvBg_SetOffsetReq
void func_ov094_02209784(BtlvBg *work, s32 x, s32 y);

#endif // BWPORT_BATTLE_BTLV_BG_H
