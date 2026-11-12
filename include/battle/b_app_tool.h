// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_app_tool.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_APP_TOOL_H
#define BWPORT_BATTLE_B_APP_TOOL_H

#include "types.h"
#include "system/cursor_move.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "system/printsys.h"

// pokebw2's BAppCursor_SetVisible
void func_ov196_0220afdc(BAppCursor *cursor, BOOL visible);
// pokebw2's BAppCursor_SetPos
// Puts the corners around the position, x and y being its center
void func_ov196_0220b000(BAppCursor *cursor, const CursorMoveData *pos);

struct BAppCursor {
    // Top left, bottom left, top right and bottom right
    ClActor *corners[4];
    u16 heapId;
    BOOL visible;
};

// pokebw2's BAppCursor_Create
BAppCursor *func_ov196_0220af40(HeapID heapId);
// pokebw2's BAppCursor_CreateActors
// The corners use the animation sequences 0 to 3 of the resources
void func_ov196_0220af6c(BAppCursor *cursor, ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims);
// pokebw2's BAppCursor_DeleteActors
void func_ov196_0220afc4(BAppCursor *cursor);
// pokebw2's BAppTool_QueueWindowScreens
// BAppTool_QueueWindowScreen on the windows of a list of indices that ends with 0xff
void func_ov196_0220b094(PrintWindow *printWindows, const u8 *list);
// pokebw2's BAppTool_FlushPrintWindows
// Runs the print queue and flushes the count windows that it has printed
void func_ov196_0220b0b0(PrintWindow *printWindows, PrintQueue *queue, u32 count);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BAppCursor_Delete
void func_ov196_0220af64(BAppCursor *cursor);
// pokebw2's BAppTool_QueueWindowScreen
// Copies a window's screen at the next VBlank
void func_ov196_0220b07c(PrintWindow *printWindow);

#endif // BWPORT_BATTLE_B_APP_TOOL_H
