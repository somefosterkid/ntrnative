// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/pms_draw.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_PMS_DRAW_H
#define BWPORT_SYSTEM_PMS_DRAW_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Where in its window a sentence is printed, in pixels
typedef struct {
    int x;
    int y;
} PMSDrawPos;

// pokebw2's PMSDraw_Main
// Every frame: shows the icons once the text is printed, and moves them with the BG when PMSDraw_SetFollowScroll
// is on
void func_0202dbc4(PMSDraw *draw);
// pokebw2's PMSDraw_Delete
void func_0202dc08(PMSDraw *draw);
// pokebw2's PMSDraw_PrintEx
void func_0202dc60(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot, const PMSDrawPos *pos);

// pokebw2's PMSDraw_SetIconVisible
// Whether the slot's icons show once its text is printed
void func_0202dcd4(PMSDraw *draw, u8 slot, BOOL visible);

// pokebw2's PMSDraw_Print
// Draws the sentence in slot into the window, at the top left of the window
void func_0202dc48(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot);

// pokebw2's PMSDraw_IsDrawn
// Whether the slot has a sentence drawn
BOOL func_0202dcc4(PMSDraw *draw, u8 slot);
// pokebw2's PMSDraw_Clear
// Clears the slot's window and hides its icons, and its screen too when clearScreen
void func_0202dc9c(PMSDraw *draw, u8 slot, BOOL clearScreen);

// pokebw2's PMSDraw_Copy
// Copies the sentence drawn in slot src to slot dest, whose window has the same size
void func_0202dd24(PMSDraw *draw, u8 src, u8 dest);

// pokebw2's PMSDraw_SetObjMode
// The OBJ mode of the slot's icons, GX_OAM_MODE_*
void func_0202dcfc(PMSDraw *draw, u8 slot, u32 mode);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSDraw_Create
// The actors are created on the unit, with the resources loaded for vramType (CLACT_VRAM_MAIN or _SUB), the icons'
// palettes at OBJ palette palette
PMSDraw *func_0202db0c(ClActUnit *unit, u32 vramType, PrintQueue *queue, Font *font, u8 palette, u8 count,
                        HeapID heapId);
// pokebw2's PMSDraw_SetBackColor
// The color index the windows are filled with, 15 at first
void func_0202de5c(PMSDraw *draw, u8 color);
// pokebw2's PMSDraw_SetColor
// The text color, a PRINT_COLOR, PRINT_COLOR(1, 2, 0) at first
void func_0202de64(PMSDraw *draw, u16 color);
// pokebw2's PMSDraw_SetVisible
// Shows or hides the slot's window and icons
void func_0202dcb0(PMSDraw *draw, u8 slot, BOOL visible);
// pokebw2's PMSDraw_SetFollowScroll
// Whether the icons follow their BG's scroll
void func_0202de68(PMSDraw *draw, BOOL follow);
// pokebw2's PMSDraw_IsPrintEnd
// Whether every slot's text is printed
BOOL func_0202dc98(PMSDraw *draw);

#endif // BWPORT_SYSTEM_PMS_DRAW_H
