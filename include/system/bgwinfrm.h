// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bgwinfrm.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_BGWINFRM_H
#define BWPORT_SYSTEM_BGWINFRM_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's BGWinFrame_SetArea
// Sets the area of the BG that a frame shows in, in tiles; the default is the whole screen
void func_0202cfec(BGWinFrame *frames, u32 index, s8 left, s8 right, s8 top, s8 bottom);
// pokebw2's BGWinFrame_Put
// Puts a frame on its BG at a position, in tiles, cut to its area; or again where it is
void func_0202cb4c(BGWinFrame *frames, u32 index, s8 x, s8 y);
// pokebw2's BGWinFrame_Delete
void func_0202ca0c(BGWinFrame *frames);
// pokebw2's BGWinFrame_Hide
// Clears the part of the BG that a frame covers
void func_0202cc70(BGWinFrame *frames, u32 index);
// pokebw2's BGWinFrame_SetPalette
// Sets the palette of an area of a frame's screen data
void func_0202ce50(BGWinFrame *frames, u32 index, u8 x, u8 y, u8 width, u8 height, u8 palette);
// pokebw2's BGWinFrame_Show
void func_0202cc58(BGWinFrame *frames, u32 index);

#define BGWINFRAME_TRANSFER_VBLANK 2

// pokebw2's BGWinFrame_GetBG
// The BG a frame is on
u8 func_0202cf8c(BGWinFrame *frames, u32 index);
// pokebw2's BGWinFrame_SetScreen
// Sets a frame's screen data from a buffer, or from an NSCR file
void func_0202cac4(BGWinFrame *frames, u32 index, u16 *screen);
// pokebw2's BGWinFrame_WriteBmpWin
// Writes a window's tiles into a frame's screen data, at the window's position
void func_0202cebc(BGWinFrame *frames, u32 index, BmpWin *window);

// pokebw2's BGWinFrame_UpdateMoves
// Moves the frames, every frame
void func_0202cd94(BGWinFrame *frames);
// pokebw2's BGWinFrame_StartMove
// Moves a frame by a step count times, one step each time BGWinFrame_UpdateMoves runs
void func_0202cd64(BGWinFrame *frames, u32 index, s8 moveX, s8 moveY, u8 count);
// pokebw2's BGWinFrame_IsMoving
// Whether a frame is still moving
BOOL func_0202ce40(BGWinFrame *frames, u32 index);

// pokebw2's BGWinFrame_GetPos
// A frame's position and size, in tiles; any pointer may be NULL
void func_0202cf98(BGWinFrame *frames, u32 index, s8 *x, s8 *y);
// pokebw2's BGWinFrame_MoveStep
// Moves a frame a step; FALSE once it has arrived
BOOL func_0202cdd4(BGWinFrame *frames, u32 index);

// pokebw2's BGWinFrame_GetSize
void func_0202cfc0(BGWinFrame *frames, u32 index, u16 *width, u16 *height);

// Black 2's, not checked against Black
// How the frames' changes reach the BG's screen: not at all, at once, or at the next VBlank
#define BGWINFRAME_TRANSFER_NONE 0

// Black 2's, not checked against Black
#define BGWINFRAME_TRANSFER_NOW 1

// pokebw2's BGWinFrame_GetScreen
u16 *func_0202cf84(BGWinFrame *frames, u32 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BGWinFrame_Create
BGWinFrame *func_0202c9b4(u32 transferMode, u32 count, HeapID heapId);
// pokebw2's BGWinFrame_InitFrame
// Sets a frame up for a BG, with a screen of width by height tiles
void func_0202ca3c(BGWinFrame *frames, u32 index, u32 bg, u32 width, u32 height);
// pokebw2's BGWinFrame_LoadScreenArc
void func_0202cb18(BGWinFrame *frames, u32 index, ArcTool *arc, u32 fileId, BOOL compressed);
// pokebw2's BGWinFrame_LoadScreen
void func_0202cae4(BGWinFrame *frames, u32 index, u32 arcId, u32 fileId, BOOL compressed);

#endif // BWPORT_SYSTEM_BGWINFRM_H
