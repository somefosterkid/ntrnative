#ifndef POKEBW2_SYSTEM_PMS_DRAW_H
#define POKEBW2_SYSTEM_PMS_DRAW_H

#include "types.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/pms_data.h"
#include "system/printsys.h"

// Draws sentences into windows (pms_draw.c): the text through a print queue, and the words that are icons as cell
// actors over the window. Each of a number of slots draws one sentence. Our names

// Where in its window a sentence is printed, in pixels
typedef struct {
    int x;
    int y;
} PMSDrawPos;

// The actors are created on the unit, with the resources loaded for vramType (CLACT_VRAM_MAIN or _SUB), the icons'
// palettes at OBJ palette palette
PMSDraw *PMSDraw_Create(ClActUnit *unit, u32 vramType, PrintQueue *queue, Font *font, u8 palette, u8 count,
                        HeapID heapId);
// Every frame: shows the icons once the text is printed, and moves them with the BG when PMSDraw_SetFollowScroll
// is on
void PMSDraw_Main(PMSDraw *draw);
void PMSDraw_Delete(PMSDraw *draw);
// Draws the sentence in slot into the window, at the top left of the window
void PMSDraw_Print(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot);
void PMSDraw_PrintEx(PMSDraw *draw, BmpWin *window, const PMSData *sentence, u8 slot, const PMSDrawPos *pos);
// Whether every slot's text is printed
BOOL PMSDraw_IsPrintEnd(PMSDraw *draw);
// Clears the slot's window and hides its icons, and its screen too when clearScreen
void PMSDraw_Clear(PMSDraw *draw, u8 slot, BOOL clearScreen);
// Shows or hides the slot's window and icons
void PMSDraw_SetVisible(PMSDraw *draw, u8 slot, BOOL visible);
// Whether the slot has a sentence drawn
BOOL PMSDraw_IsDrawn(PMSDraw *draw, u8 slot);
// Whether the slot's icons show once its text is printed
void PMSDraw_SetIconVisible(PMSDraw *draw, u8 slot, BOOL visible);
// The OBJ mode of the slot's icons, GX_OAM_MODE_*
void PMSDraw_SetObjMode(PMSDraw *draw, u8 slot, u32 mode);
// Copies the sentence drawn in slot src to slot dest, whose window has the same size
void PMSDraw_Copy(PMSDraw *draw, u8 src, u8 dest);
// The color index the windows are filled with, 15 at first
void PMSDraw_SetBackColor(PMSDraw *draw, u8 color);
// The text color, a PRINT_COLOR, PRINT_COLOR(1, 2, 0) at first
void PMSDraw_SetColor(PMSDraw *draw, u16 color);
// Whether the icons follow their BG's scroll
void PMSDraw_SetFollowScroll(PMSDraw *draw, BOOL follow);

#endif // POKEBW2_SYSTEM_PMS_DRAW_H
