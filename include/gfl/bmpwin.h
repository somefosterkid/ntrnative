// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/bmpwin.h, the lines in use so far.
#ifndef BWPORT_GFL_BMPWIN_H
#define BWPORT_GFL_BMPWIN_H

#include "types.h"
#include "gfl/bg_sys.h"
#include "struct_decls.h"

// Windows on a BG that are drawn to as bitmaps (bmp_win.c). A window has a bitmap of its own characters, which
// BmpWin_FlushChar loads to the BG, and an area of the BG's screen buffer that BmpWin_FlushMap points at them. Names
// from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// The heap that windows are allocated from
void BmpWin_InitAllocator(HeapID heapId);
void BmpWin_FreeAllocator(void);
// A window of width by height tiles at (x, y) on a BG, its characters allocated from the end of the BG's characters
// if fromEnd
BmpWin *BmpWin_CreateDynamic(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette, u8 fromEnd);
// The same with the characters at charPos
BmpWin *BmpWin_CreateStatic(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette, u32 charPos);
void BmpWin_Free(BmpWin *window);
void BmpWin_FlushChar(BmpWin *window);
void BmpWin_FlushMap(BmpWin *window);
// Fills the screen around the window with the eight characters of a frame from frameChar: corners and edges from the
// top left, row by row
void func_020454e0(BmpWin *window, u16 frameChar, u8 palette);
// Clears the window's area of the screen
void func_020456f8(BmpWin *window);
u8 BmpWin_GetBGIndex(BmpWin *window);
// The size of the bitmap, and of the area of the screen, in tiles
u8 func_02045738(BmpWin *window);
u8 func_02045740(BmpWin *window);
u8 BmpWin_GetWidth1(BmpWin *window);
u8 BmpWin_GetHeight2(BmpWin *window);
u8 BmpWin_GetPosX(BmpWin *window);
u8 BmpWin_GetPosY(BmpWin *window);
// The first character
u16 func_02045768(BmpWin *window);
GFLBitmap *BmpWin_GetBitmap(BmpWin *window);
u8 func_02045778(BmpWin *window);
void func_02045780(BmpWin *window, u8 x);
void func_02045788(BmpWin *window, u8 y);
void func_02045790(BmpWin *window, u8 height);
void func_02045798(BmpWin *window, u8 palette);

// Copies the window's characters and screen, the screen at the next VBlank
static inline void BmpWin_Transfer(BmpWin *window) {
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's BmpWin_FreeAllocator
void func_ov014_02177704(void);

// BmpWin_Transfer with the screen copied at once
static inline void BmpWin_TransferNow(BmpWin *window) {
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

#endif // BWPORT_GFL_BMPWIN_H
