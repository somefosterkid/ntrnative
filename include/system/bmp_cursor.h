// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_cursor.h.
#ifndef BWPORT_SYSTEM_BMP_CURSOR_H
#define BWPORT_SYSTEM_BMP_CURSOR_H

#include "types.h"
#include "struct_decls.h"
#include "system/printsys.h"

// BmpCursor_Create is swan's name (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the others are Black's dsd
// names, with pokebw2's guesses in the comments

// The cursor menus draw before their selected option (bmp_cursor.c)

BmpCursor *BmpCursor_Create(u32 heapId);
// BmpCursor_Free
void func_02020d3c(BmpCursor *cursor);
// Prints the cursor at x and y into the window (BmpCursor_Print)
void func_02020d5c(BmpCursor *cursor, u16 x, u16 y, PrintWindow *printWindow, PrintQueue *queue, Font *font);
// Makes the cursor the ROM's 8x8 triangle (BmpCursor_LoadBitmap)
void func_02020dc8(BmpCursor *cursor, u32 heapId);

#endif // BWPORT_SYSTEM_BMP_CURSOR_H
