// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/printsys.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_PRINTSYS_H
#define BWPORT_SYSTEM_PRINTSYS_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/bmpwin.h"
#include "struct_decls.h"

// Printing text (printsys.c): into a bitmap at once, through a queue that spreads it over frames while the game is
// connected, or into a window a few characters a frame. Most of these functions have no name yet

typedef struct PrintQueue PrintQueue;

// A color of text, shadow and background color indices
#define PRINT_COLOR(text, shadow, background)                                                                          \
    ((((text) & 0x1f) << 10) | (((shadow) & 0x1f) << 5) | ((background) & 0x1f))

// Draws a string into a bitmap at once
void GFL_TextRendererDrawToBitmap(GFLBitmap *bitmap, s16 x, s16 y, const StrBuf *strbuf, Font *font);

// Prints a string into a bitmap through the queue (pokebw2's func_02021c54)
void func_0201c6fc(PrintQueue *queue, GFLBitmap *bitmap, s16 x, s16 y, const StrBuf *strbuf, Font *font);
void func_0201c724(PrintQueue *queue, GFLBitmap *bitmap, s16 x, s16 y, const StrBuf *strbuf, Font *font, u16 color);

// A window whose text goes through a print queue, and whose characters are sent to VRAM once it is printed
typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

// func_0201ce30's states
#define PRINT_STREAM_RUNNING 0
#define PRINT_STREAM_PAUSED 1
#define PRINT_STREAM_DONE 2

// pokebw2's func_02021998, func_02021a18, func_02021a3c, func_02021c0c and func_02021c1c
PrintQueue *func_0201c440(HeapID heapId);
void func_0201c4c0(PrintQueue *queue);
BOOL func_0201c4e4(PrintQueue *queue);
BOOL func_0201c6b4(PrintQueue *queue);
BOOL func_0201c6c4(PrintQueue *queue, GFLBitmap *bitmap);
// pokebw2's func_02021c44
void func_0201c6ec(PrintQueue *queue);
// Prints a string into a window a character at a time (pokebw2's func_02022268)
PrintStream *func_0201cce4(BmpWin *window, s16 x, s16 y, const StrBuf *strbuf, Font *font, s32 wait,
                           TCBExManager *tcbManager, u32 a7, HeapID heapId, u16 a9);
// pokebw2's func_020223b4, func_020223bc, func_020223cc and func_020223e0
u32 func_0201ce30(PrintStream *stream);
// Continues printing after PRINT_STREAM_PAUSED
void func_0201ce38(PrintStream *stream);
void func_0201ce48(PrintStream *stream);
// Cuts the wait short, and prints at the fast speed from then on
void func_0201ce5c(PrintStream *stream, s32 waitTimer);

static inline void PrintWindow_Init(PrintWindow *printWindow, BmpWin *window) {
    printWindow->window = window;
    printWindow->flushPending = FALSE;
}

// Prints a string in its own colors through the queue
static inline void PrintWindow_PrintNoColor(PrintWindow *printWindow, PrintQueue *queue, s16 x, s16 y,
                                            const StrBuf *strbuf, Font *font) {
    func_0201c6fc(queue, BmpWin_GetBitmap(printWindow->window), x, y, strbuf, font);
    printWindow->flushPending = TRUE;
}

// Prints a string in a color that PRINT_COLOR makes through the queue
static inline void PrintWindow_PrintColor(PrintWindow *printWindow, PrintQueue *queue, s16 x, s16 y,
                                          const StrBuf *strbuf, Font *font, u16 color) {
    func_0201c724(queue, BmpWin_GetBitmap(printWindow->window), x, y, strbuf, font, color);
    printWindow->flushPending = TRUE;
}

// Whether the window's text has been sent to VRAM
static inline BOOL PrintWindow_IsPrinted(PrintWindow *printWindow) {
    return printWindow->flushPending ? FALSE : TRUE;
}

static inline void PrintWindow_Flush(PrintWindow *printWindow, PrintQueue *queue) {
    if (printWindow->flushPending && !func_0201c6c4(queue, BmpWin_GetBitmap(printWindow->window))) {
        BmpWin_FlushChar(printWindow->window);
        printWindow->flushPending = FALSE;
    }
}

// The width of the line str starts, and where the next line starts
u32 GFL_FontGetLineWidth(const u16 *str, Font *font, u32 spacing, const u16 **end);
// The width in pixels of a string's widest line
u32 GFL_FontGetBlockWidth(const StrBuf *strbuf, Font *font, u32 spacing);
u32 GFL_FontGetBlockHeight(const StrBuf *strbuf, Font *font);
// The commands in strings
u16 GFL_StrCmdGetWordSetCommandCount(const StrBuf *strbuf);
u16 GFL_StrCmdGetIdentChar(void);
BOOL GFL_StrCmdIsWordSet(const u16 *cmd);
u32 GFL_StrCmdGetCommandCategory(const u16 *cmd);
u32 GFL_StrCmdGetCommandIndex(const u16 *cmd);
u16 GFL_WordSetGetCommandParameter(const u16 *cmd, u32 index);
const u16 *GFL_StrCmdSkipCommand(const u16 *cmd);

// Prints a string in a color through the queue
static inline void PrintWindow_Print(PrintWindow *printWindow, PrintQueue *queue, s16 x, s16 y, const StrBuf *strbuf,
                                     Font *font, u16 color) {
    func_0201c724(queue, BmpWin_GetBitmap(printWindow->window), x, y, strbuf, font, color);
    printWindow->flushPending = TRUE;
}

void func_0201ce0c(PrintStream *stream);

// Whether strings go in the queue even while the game is not connected
void func_0201c4c8(PrintQueue *queue, u32 queueAlways);
void func_0201c4dc(PrintQueue *queue, u64 timeLimit);

u32 func_0201d2c8(const StrBuf *strbuf);
void func_0201ce20(PrintStream *stream);

// Prints at the speed of a wait, or at the stream's own speed again
void func_0201ce8c(PrintStream *stream, s32 wait);
void func_0201ceb8(PrintStream *stream);

// Black 2's, not checked against Black
typedef BOOL (*PrintStreamCallback)(u32 event);

PrintStream *func_0201cd10(BmpWin *window, s16 x, s16 y, const StrBuf *strbuf, Font *font, s32 wait,
                           TCBExManager *tcbManager, u32 a7, HeapID heapId, u16 a9, PrintStreamCallback callback);

void GFL_StrCmdBuild(StrBuf *strbuf, u32 category, u16 index, u8 paramCount, const u16 *params);

// Sets the string terminator, and creates the bitmap glyphs are drawn into
void GFL_TextRndInit(HeapID heapId);
BOOL func_0201ced4(PrintStream *stream);
// The width of each line, up to maxLines. Returns the count of lines
u32 func_0201d33c(const StrBuf *strbuf, Font *font, u32 spacing, u32 *widths, u32 maxLines);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Draws in a color that PRINT_COLOR makes
void GFL_TextRendererDrawToBitmapEx(GFLBitmap *bitmap, s16 x, s16 y, const StrBuf *strbuf, Font *font, u16 color);
PrintQueue *func_0201c450(u16 size, HeapID heapId);
u8 GFL_StrCmdCountLinesUntilWordSetIndex(const StrBuf *strbuf, u8 index);
u8 GFL_StrCmdGetStrWidthUntilWordSetIndex(const StrBuf *strbuf, u8 index, Font *font, u32 spacing);
// Whether the page scrolls or clears at the next page break
u32 func_0201ce34(PrintStream *stream);
// Copies a line of the string, with its commands. Returns whether it has the line
BOOL func_0201d37c(const StrBuf *strbuf, StrBuf *dest, u32 line);

#endif // BWPORT_SYSTEM_PRINTSYS_H
