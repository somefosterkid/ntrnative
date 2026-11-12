// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_finger_cursor.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTLV_FINGER_CURSOR_H
#define BWPORT_BATTLE_BTLV_FINGER_CURSOR_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's BtlvFingerCursor_Delete
void func_ov094_02209b30(BtlvFingerCursor *cursor);
// pokebw2's BtlvFingerCursor_Start
// Shows the finger at x, y; after wait frames it taps, reaching the touch screen on frame tapFrame of the tap, and
// hides hideWait frames later. Returns FALSE while a tap is still showing
BOOL func_ov094_02209b60(BtlvFingerCursor *cursor, s32 x, s32 y, u32 wait, s32 tapFrame, u32 hideWait);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Loads the finger's graphics, its palette into row paletteRow of the sub screen's OBJ palettes
BtlvFingerCursor *BtlvFingerCursor_Create(PaletteFade *fade, u32 paletteRow, HeapID heapId);
// pokebw2's BtlvFingerCursor_RemoveActor
void func_ov094_02209c18(BtlvFingerCursor *cursor);
// pokebw2's BtlvFingerCursor_IsTouched
// Whether the finger has touched the screen and is still showing
BOOL func_ov094_02209c2c(BtlvFingerCursor *cursor);

#endif // BWPORT_BATTLE_BTLV_FINGER_CURSOR_H
