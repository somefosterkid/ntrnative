// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov299/zknlist_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/ui/print_msg.h"
#include "app/zknlist_bmp.h"
#include "app/zknlist_main.h"
#include "app/zukanlist.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nnsys/g2d.h"
#include "save/pokedex.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/game_start_3.h"

// The windows and text of the Pokédex's list. The ROM doesn't name this file; zknlist_bmp.c is a guess after the
// search screen's zknsearch_bmp.c

#define COLOR_ITEM PRINT_COLOR(14, 15, 0)
#define COLOR_BUTTON PRINT_COLOR(15, 2, 1)
#define COLOR_COUNT PRINT_COLOR(12, 13, 0)

// A window to create, as BmpWin_CreateDynamic takes it
typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} ZknListWindowData;

void func_ov209_021bd440(ZknListWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align);
void func_ov209_021bd488(ZknListWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align);
void func_ov209_021bd4dc(PrintWindow *window);
void func_ov209_021bd4f4(ZknListWork *wk, u32 window);
void func_ov209_021bd5d4(ZknListWork *wk);

// pokebw2's sZknListWindows
static const ZknListWindowData data_ov209_021be990[ZKNLIST_WIN_COUNT] = {
    { 5, 1, 0, 17, 3, 4 },  { 5, 2, 3, 13, 3, 4 },  { 5, 16, 3, 15, 3, 4 },
};

// pokebw2's ZknListBmp_Init
void func_ov209_021bd398(ZknListWork *wk) {
    u32 i;

    BmpWin_InitAllocator(HEAPID_ZUKAN_LIST);
    for (i = 0; i < ZKNLIST_WIN_COUNT; i++) {
        wk->windows[i].window =
            BmpWin_CreateDynamic(data_ov209_021be990[i].bg, data_ov209_021be990[i].x, data_ov209_021be990[i].y,
                                 data_ov209_021be990[i].width, data_ov209_021be990[i].height, data_ov209_021be990[i].palette, TRUE);
    }
    func_ov209_021bd5d4(wk);
}

// pokebw2's ZknListBmp_Exit
void func_ov209_021bd3e0(ZknListWork *wk) {
    u32 i;

    for (i = 0; i < ZKNLIST_WIN_COUNT; i++) {
        BmpWin_Free(wk->windows[i].window);
    }
    BmpWin_FreeAllocator();
}

// pokebw2's ZknListBmp_Update
void func_ov209_021bd3fc(ZknListWork *wk) {
    u32 i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < ZKNLIST_WIN_COUNT; i++) {
        PrintWindow_Flush(&wk->windows[i], wk->printQueue);
    }
}

// pokebw2's ZknListBmp_Print
void func_ov209_021bd440(ZknListWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, msg);

    func_ov067_021b6ae4(&wk->windows[window], wk->printQueue, x, y, strbuf, wk->font, color, align);
    GFL_StrBufFree(strbuf);
}

// Prints a message with the word set's words
// pokebw2's ZknListBmp_PrintFormat
void func_ov209_021bd488(ZknListWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, msg);

    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, strbuf);
    func_ov067_021b6ae4(&wk->windows[window], wk->printQueue, x, y, wk->strbuf, wk->font, color, align);
    GFL_StrBufFree(strbuf);
}

// pokebw2's ZknListBmp_PutWindow
void func_ov209_021bd4dc(PrintWindow *window) {
    BmpWin_FlushMap(window->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window->window));
}

// Copies the BG under a window of the top screen into it, for the text to go over
// pokebw2's ZknListBmp_LoadWindowBG
void func_ov209_021bd4f4(ZknListWork *wk, u32 window) {
    NNSG2dCharacterData *charData;
    NNSG2dScreenData *screenData;
    void *charFile;
    u8 *chars;
    void *screenFile;
    u8 *pixels;
    u16 *screen;
    ArcTool *arc;
    u8 width, height;
    u8 posX, posY;
    u16 x, y;

    width = BmpWin_GetWidth1(wk->windows[window].window);
    height = BmpWin_GetHeight2(wk->windows[window].window);
    posX = BmpWin_GetPosX(wk->windows[window].window);
    posY = BmpWin_GetPosY(wk->windows[window].window);
    pixels = GFL_BitmapGetPixelData(BmpWin_GetBitmap(wk->windows[window].window));
    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_LIST);
    screenFile = GFL_G2DIOReadNSCRArc(arc, 69, FALSE, &screenData, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    screen = (u16 *)screenData->rawData;
    charFile = GFL_G2DIOReadBGNCGRArc(arc, 57, FALSE, &charData, HEAPID_ZUKAN_LIST | HEAPID_TAIL_BIT);
    chars = charData->pRawData;
    GFL_ArcToolFree(arc);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            MIi_CpuCopy32(&chars[(screen[(posY + y) * 32 + posX + x] & 0x3ff) * 0x20], &pixels[(y * width + x) * 0x20],
                         0x20);
        }
    }
    GFL_HeapFree(charFile);
    GFL_HeapFree(screenFile);
}

void func_ov209_021bd5d4(ZknListWork *wk) {
    func_ov209_021bd4f4(wk, ZKNLIST_WIN_TITLE);
    if (func_0200ca70(wk->param->pokedex) == 1) {
        func_ov209_021bd440(wk, ZKNLIST_WIN_TITLE, GameStart3_Text_001, 0, 4, COLOR_ITEM, PRINT_ALIGN_LEFT);
    } else {
        func_ov209_021bd440(wk, ZKNLIST_WIN_TITLE, GameStart3_Text_000, 0, 4, COLOR_ITEM, PRINT_ALIGN_LEFT);
    }
    func_ov209_021bd4dc(&wk->windows[ZKNLIST_WIN_TITLE]);
}

void func_ov209_021bd628(ZknListWork *wk) {
    u32 seen;
    u32 caught;

    if (func_0200ca70(wk->param->pokedex) == 1) {
        seen = PokeDex_GetSeenNoNational(wk->param->pokedex);
        caught = PokeDex_GetCaughtNoNational(wk->param->pokedex);
    } else {
        seen = func_0200d0f8(wk->param->pokedex, 0x806b);
        caught = func_0200cf54(wk->param->pokedex, 0x806b);
    }
    func_ov209_021bd4f4(wk, ZKNLIST_WIN_SEEN);
    func_ov209_021bd4f4(wk, ZKNLIST_WIN_CAUGHT);
    func_ov209_021bd440(wk, ZKNLIST_WIN_SEEN, 2, 0x48, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    WordSetNumber(wk->wordSet, 0, seen, 3, 0, TRUE);
    func_ov209_021bd488(wk, ZKNLIST_WIN_SEEN, 5, 0x68, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    func_ov209_021bd440(wk, ZKNLIST_WIN_CAUGHT, 3, 0x58, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    WordSetNumber(wk->wordSet, 0, caught, 3, 0, TRUE);
    func_ov209_021bd488(wk, ZKNLIST_WIN_CAUGHT, 5, 0x78, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    func_ov209_021bd4dc(&wk->windows[ZKNLIST_WIN_SEEN]);
    func_ov209_021bd4dc(&wk->windows[ZKNLIST_WIN_CAUGHT]);
}


void func_ov209_021bd704(ZknListWork *wk) {
    func_ov209_021bd4f4(wk, ZKNLIST_WIN_CAUGHT);
    func_ov209_021bd440(wk, ZKNLIST_WIN_CAUGHT, 4, 0x58, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    WordSetNumber(wk->wordSet, 0, wk->param->count, 3, 0, TRUE);
    func_ov209_021bd488(wk, ZKNLIST_WIN_CAUGHT, 5, 0x78, 4, COLOR_COUNT, PRINT_ALIGN_RIGHT);
    func_ov209_021bd4dc(&wk->windows[ZKNLIST_WIN_CAUGHT]);
}

void func_ov209_021bd75c(ZknListWork *wk, PrintWindow *window, StrBuf *strbuf, u32 index) {
    PrintQueue *queue;
    StrBuf *src;

    queue = func_ov067_021b9394(wk->frameList);
    if (func_0200ca70(wk->param->pokedex) == 0) {
        index = wk->regionalNumbers[index];
    }
    src = GFL_MsgDataLoadStrbufNew(wk->msgData, 8);
    WordSetNumber(wk->wordSet, 0, index, 3, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, src);
    GFL_StrBufFree(src);
    func_ov067_021b6ae4(window, queue, 0, 4, wk->strbuf, wk->font, COLOR_ITEM, PRINT_ALIGN_LEFT);
    func_ov067_021b6ae4(window, queue, 0x1c, 4, strbuf, wk->font, COLOR_ITEM, PRINT_ALIGN_LEFT);
}




