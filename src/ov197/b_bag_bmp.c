// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/b_app_tool.h"
#include "battle/b_bag_bmp.h"
#include "battle/b_bag_main.h"
#include "constants/items.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "pml/item.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/str_tool.h"
#include "system/text_speed.h"
#include "system/wordset.h"

// The battle bag's windows and text (overlay 286). The ROM doesn't name this file: b_bag_bmp.c is a guess after the
// ROM's b_bag_main.c and the b_bag_bmp.c of Diamond and Pearl. None of these functions has a name yet.

#define BBAG_COLOR PRINT_COLOR(15, 2, 0)

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} BBagWinData;

static void func_ov197_0220b150(BBagWork *work, u8 page);
static void func_ov197_0220b188(BBagWork *work);
static void func_ov197_0220b1d4(BBagWork *work, u32 winIdx, u32 msgId, u32 y, u16 color);
static void func_ov197_0220b248(BBagWork *work);
static void func_ov197_0220b31c(BBagWork *work, u32 index, u32 slot, u32 winIdx, u32 x, u32 y);
static void func_ov197_0220b3ec(BBagWork *work, u32 index, u32 slot, u32 winIdx, u32 x, u32 y, u32 unused);
static void func_ov197_0220b4b8(BBagWork *work, u32 slot);
static void func_ov197_0220b6c4(BBagWork *work);
static void func_ov197_0220b774(BBagWork *work);
static void func_ov197_0220b78c(BBagWork *work, u32 index);
static void func_ov197_0220b814(BBagWork *work, u32 index);
static void func_ov197_0220b880(BBagWork *work);
static void func_ov197_0220b928(BBagWork *work);

static const u8 data_ov197_0220da08[] = { 31, 32, 33, 34, 0xff };
static const u8 data_ov197_0220da0d[] = { 0, 1, 2, 3, 4, 0xff };
static const u8 data_ov197_0220da19[] = {
#include "asset/b_bag_bmp_data_ov197_0220da19.inc"
};
static const u8 data_ov197_0220da28[] = {
#include "asset/b_bag_bmp_data_ov197_0220da28.inc"
};

// By slot, the messages of the item's name and of its count
static const u32 data_ov197_0220da38[6][2] = {
#include "asset/b_bag_bmp_data_ov197_0220da38.inc"
};

static const BBagWinData data_ov197_0220da68[BBAG_WINDOW_MAX] = {
#include "asset/b_bag_bmp_data_ov197_0220da68.inc"
};

// The message window. Declared after the window table for MWCC's sort to lay the data out in the ROM's order
static const BBagWinData data_ov286_021f6d5f = { 4, 1, 19, 30, 4, 12 };

// pokebw2's BBagBmp_Init
void func_ov197_0220b120(BBagWork *work) {
    work->msgWin =
        BmpWin_CreateDynamic(data_ov286_021f6d5f.bg, data_ov286_021f6d5f.x, data_ov286_021f6d5f.y,
                             data_ov286_021f6d5f.width, data_ov286_021f6d5f.height, data_ov286_021f6d5f.palette, TRUE);
    func_ov197_0220b150(work, work->page);
}

// page isn't used: every window is created at once
// pokebw2's BBagBmp_CreateWindows
static void func_ov197_0220b150(BBagWork *work, u8 page) {
    const BBagWinData *data = data_ov197_0220da68;
    u32 i;

    for (i = 0; i < BBAG_WINDOW_MAX; i++) {
        work->windows[i].window =
            BmpWin_CreateDynamic(data->bg, data->x, data->y, data->width, data->height, data->palette, TRUE);
        data++;
    }
}

// pokebw2's BBagBmp_FreeWindows
static void func_ov197_0220b188(BBagWork *work) {
    u32 i;

    for (i = 0; i < BBAG_WINDOW_MAX; i++) {
        BmpWin_Free(work->windows[i].window);
    }
}

// pokebw2's BBagBmp_Exit
void func_ov197_0220b1a0(BBagWork *work) {
    func_ov197_0220b188(work);
    BmpWin_Free(work->msgWin);
}

// pokebw2's BBagBmp_DrawPage
void func_ov197_0220b1b0(BBagWork *work, u8 page) {
    switch (page) {
    case 0:
        func_ov197_0220b248(work);
        break;
    case 1:
        func_ov197_0220b774(work);
        break;
    case 2:
        func_ov197_0220b880(work);
        break;
    }
}

// pokebw2's BBagBmp_PrintCentered
static void func_ov197_0220b1d4(BBagWork *work, u32 winIdx, u32 msgId, u32 y, u16 color) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);
    u32 width = GFL_FontGetBlockWidth(str, work->param->font, 0);
    u32 x = (func_02045738(win) * 8 - width) / 2;

    PrintWindow_Print(&work->windows[winIdx], work->printQueue, x, y, str, work->param->font, color);
    GFL_StrBufFree(str);
}

// pokebw2's BBagBmp_DrawPocketPage
static void func_ov197_0220b248(BBagWork *work) {
    u32 i;

    for (i = 0; i <= 4; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i].window), 0);
    }
    func_ov197_0220b1d4(work, 0, 0, 0, BBAG_COLOR);
    func_ov197_0220b1d4(work, 0, 1, 16, BBAG_COLOR);
    func_ov197_0220b1d4(work, 1, 2, 0, BBAG_COLOR);
    func_ov197_0220b1d4(work, 1, 3, 16, BBAG_COLOR);
    func_ov197_0220b1d4(work, 2, 7, 0, BBAG_COLOR);
    func_ov197_0220b1d4(work, 3, 6, 0, BBAG_COLOR);

    if (work->lastItem != ITEM_NONE) {
        StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 8);

        PrintWindow_Print(&work->windows[4], work->printQueue, 0, 0, str, work->param->font, BBAG_COLOR);
        GFL_StrBufFree(str);
    } else {
        BmpWin_FlushChar(work->windows[4].window);
    }
    work->flushList = data_ov197_0220da0d;
}

// x and y aren't used: the name is centred at y 7
// pokebw2's BBagBmp_PrintSlotName
static void func_ov197_0220b31c(BBagWork *work, u32 index, u32 slot, u32 winIdx, u32 x, u32 y) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *tmpl;
    u32 width;
    u32 nameX;

    GFL_BitmapFill(BmpWin_GetBitmap(win), 0);
    if (work->items[work->pocket][index].item != ITEM_NONE) {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov197_0220da38[slot][0]);
        loadItemNameToStrbuf(work->wordSet, 0, work->items[work->pocket][index].item);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, tmpl);
        width = GFL_FontGetBlockWidth(work->strBuf, work->param->font, 0);
        nameX = (func_02045738(win) * 8 - width) / 2;
        PrintWindow_Print(&work->windows[winIdx], work->printQueue, nameX, 7, work->strBuf, work->param->font,
                          BBAG_COLOR);
        GFL_StrBufFree(tmpl);
    } else {
        BmpWin_FlushChar(win);
    }
}

// x and the last argument aren't used: the count is printed at x 0
// pokebw2's BBagBmp_PrintSlotCount
static void func_ov197_0220b3ec(BBagWork *work, u32 index, u32 slot, u32 winIdx, u32 x, u32 y, u32 unused) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *tmpl;

    GFL_BitmapFill(BmpWin_GetBitmap(win), 0);
    if (work->items[work->pocket][index].count != 0 && work->param->mode != 1) {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov197_0220da38[slot][1]);
        WordSetNumber(work->wordSet, 0, work->items[work->pocket][index].count, 3, NUM_PAD_NONE, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, tmpl);
        PrintWindow_Print(&work->windows[winIdx], work->printQueue, 0, y, work->strBuf, work->param->font, BBAG_COLOR);
        GFL_StrBufFree(tmpl);
    } else {
        BmpWin_FlushChar(win);
    }
}

// pokebw2's BBagBmp_DrawSlot
static void func_ov197_0220b4b8(BBagWork *work, u32 slot) {
    u32 index = work->param->pages[work->pocket] * 6;
    u32 winIdx = 5;

    if (work->listBuf != 0) {
        winIdx = 17;
    }
    func_ov197_0220b31c(work, slot + index, slot, winIdx + slot * 2, 0, 0);
    func_ov197_0220b3ec(work, slot + index, slot, winIdx + 1 + slot * 2, 0, 4, 0);
}

// pokebw2's BBagBmp_DrawSlots
void func_ov197_0220b520(BBagWork *work) {
    u16 i;

    GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 19, 17);
    for (i = 0; i < 6; i++) {
        func_ov197_0220b4b8(work, i);
    }
    if (work->listBuf == 0) {
        work->flushList = data_ov197_0220da28;
    } else {
        work->flushList = data_ov197_0220da19;
    }
    work->listBuf ^= 1;
}

// pokebw2's BBagBmp_PrintPageNumber
void func_ov197_0220b584(BBagWork *work) {
    BmpWin *win = work->windows[30].window;
    GFLBitmap *bitmap = BmpWin_GetBitmap(win);
    StrBuf *str;
    u32 slashWidth;
    u32 x;
    u32 pageWidth;

    GFL_BitmapFill(bitmap, 0);
    str = GFL_MsgDataLoadStrbufNew(work->msgData, 28);
    slashWidth = GFL_FontGetBlockWidth(str, work->param->font, 0);
    x = (func_02045738(win) * 8 - slashWidth) / 2;
    func_0201c724(work->printQueue, bitmap, x, 4, str, work->param->font, BBAG_COLOR);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(work->msgData, 29);
    WordSetNumber(work->wordSet, 0, work->lastPage[work->pocket] + 1, 2, NUM_PAD_NONE, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    func_0201c724(work->printQueue, bitmap, x + slashWidth, 4, work->strBuf, work->param->font, BBAG_COLOR);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(work->msgData, 30);
    WordSetNumber(work->wordSet, 0, work->param->pages[work->pocket] + 1, 2, NUM_PAD_NONE, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    pageWidth = GFL_FontGetBlockWidth(work->strBuf, work->param->font, 0);
    PrintWindow_Print(&work->windows[30], work->printQueue, x - pageWidth, 4, work->strBuf, work->param->font,
                      BBAG_COLOR);
    GFL_StrBufFree(str);
}

// pokebw2's BBagBmp_PrintPocketName
static void func_ov197_0220b6c4(BBagWork *work) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->windows[29].window), 0);
    if (work->param->mode != 1) {
        switch (work->pocket) {
        case 0:
            func_ov197_0220b1d4(work, 29, 22, 4, BBAG_COLOR);
            func_ov197_0220b1d4(work, 29, 23, 20, BBAG_COLOR);
            break;
        case 1:
            func_ov197_0220b1d4(work, 29, 24, 4, BBAG_COLOR);
            func_ov197_0220b1d4(work, 29, 25, 20, BBAG_COLOR);
            break;
        case 2:
            func_ov197_0220b1d4(work, 29, 26, 12, BBAG_COLOR);
            break;
        case 3:
            func_ov197_0220b1d4(work, 29, 27, 12, BBAG_COLOR);
            break;
        }
    } else {
        BmpWin_FlushChar(work->windows[29].window);
    }
}

// pokebw2's BBagBmp_DrawItemPage
static void func_ov197_0220b774(BBagWork *work) {
    func_ov197_0220b520(work);
    func_ov197_0220b6c4(work);
    func_ov197_0220b584(work);
}

// pokebw2's BBagBmp_PrintChosenName
static void func_ov197_0220b78c(BBagWork *work, u32 index) {
    BmpWin *win = work->windows[31].window;
    StrBuf *tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 9);

    loadItemNameToStrbuf(work->wordSet, 0, work->items[work->pocket][index].item);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, tmpl);
    GFL_BitmapFill(BmpWin_GetBitmap(win), 0);
    PrintWindow_Print(&work->windows[31], work->printQueue, 0, 0, work->strBuf, work->param->font, BBAG_COLOR);
    GFL_StrBufFree(tmpl);
}

// pokebw2's BBagBmp_PrintDescription
static void func_ov197_0220b814(BBagWork *work, u32 index) {
    StrBuf *str = GFL_StrBufCreate(130, work->param->heapId);

    setItemDescriptionTextToStrbuf(str, work->items[work->pocket][index].item, work->param->heapId);
    PrintWindow_Print(&work->windows[33], work->printQueue, 4, 0, str, work->param->font, BBAG_COLOR);
    GFL_StrBufFree(str);
}

// pokebw2's BBagBmp_DrawChosenItemPage
static void func_ov197_0220b880(BBagWork *work) {
    u32 i;
    s16 row;
    s16 page;

    for (i = 31; i <= 34; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i].window), 0);
    }
    row = work->param->rows[work->pocket];
    page = work->param->pages[work->pocket];
    func_ov197_0220b78c(work, row + page * 6);
    func_ov197_0220b3ec(work, row + page * 6, 0, 32, 0, 0, 0);
    func_ov197_0220b814(work, row + page * 6);
    func_ov197_0220b1d4(work, 34, 31, 0, BBAG_COLOR);
    work->flushList = data_ov197_0220da08;
}

// pokebw2's BBagBmp_OpenMessage
void func_ov197_0220b904(BBagWork *work) {
    BmpWin_DrawFrame(work->msgWin, WINFRAME_TRANSFER_NONE, 1, 11);
    GFL_BitmapFill(BmpWin_GetBitmap(work->msgWin), 15);
    func_ov197_0220b928(work);
}

// pokebw2's BBagBmp_StartMessageStream
static void func_ov197_0220b928(BBagWork *work) {
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    work->printStream = func_0201cce4(work->msgWin, 0, 0, work->strBuf, work->param->font, func_02012ffc(),
                                      work->tcbExMgr, 10, work->param->heapId, 15);
    BmpWin_Transfer(work->msgWin);
}

// pokebw2's BBagBmp_FlushWindows
void func_ov197_0220b980(BBagWork *work) {
    func_ov196_0220b0b0(work->windows, work->printQueue, BBAG_WINDOW_MAX);
}

// pokebw2's BBagBmp_TransferPage
void func_ov197_0220b990(BBagWork *work) {
    func_ov196_0220b094(work->windows, work->flushList);
}
