// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_winframe.h, the lines in use so
// far. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#ifndef BWPORT_SYSTEM_BMP_WINFRAME_H
#define BWPORT_SYSTEM_BMP_WINFRAME_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The frames of the windows

// When a frame's screen is copied
enum {
    WINFRAME_TRANSFER_NOW,
    WINFRAME_TRANSFER_VBLANK,
    WINFRAME_TRANSFER_NONE,
};

// The window frame archive's palette file for a frame type
u32 GetSysMsgBoxPaletteDatID(u8 type);
void LoadSysMsgBoxBGChar(u8 bg, u16 frameChar, u8 type, HeapID heapId);
void LoadSysMsgBox(u8 bg, u16 frameChar, u8 framePalette, u8 type, HeapID heapId);
// Loads a frame's characters at the end of a BG's characters, and its palette. The result is their position in the low
// 16 bits and their size in the high 16, as GFL_BGSysFreeCharMemory takes them
u32 LoadCursorImageEndOfHeap(u8 bg, u8 framePalette, u8 type, HeapID heapId);
#define CHAR_POS(chars) ((chars) & 0xffff)
#define CHAR_SIZE(chars) ((u16)((chars) >> 16))
void BmpWin_DrawFrame(BmpWin *window, u8 transfer, u16 frameChar, u8 framePalette);
// pokebw2's BmpWin_ClearFrame
void func_0201f7a8(BmpWin *window, u8 transfer);

void LoadSysMsgBoxPalette(u8 bg, u8 framePalette, u8 type, HeapID heapId);

// pokebw2's BmpWin_DrawFrameEndOfHeap
// BmpWin_DrawFrame for a frame LoadCursorImageEndOfHeap loaded
void func_0201f79c(BmpWin *window, u8 transfer, u32 chars, u8 framePalette);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's FreeCursorImageEndOfHeap
// Frees the characters LoadCursorImageEndOfHeap loaded
void func_0201f614(u32 bg, u32 chars);

#endif // BWPORT_SYSTEM_BMP_WINFRAME_H
