// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/ui/print_msg.h"
#include "app/zknsearch_bmp.h"
#include "app/zknsearch_main.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/printsys.h"
#include "text/system/0479.h"
#include "app/zukan_common.h"
#include "app/zukansearch.h"

// The windows and text of the Pokédex's search screen. The ROM doesn't name this file; zknsearch_bmp.c is a guess
// after zknsearch_main.c

#define COLOR_WHITE PRINT_COLOR(15, 2, 0)
#define COLOR_BLACK PRINT_COLOR(1, 2, 0)
#define COLOR_MESSAGE PRINT_COLOR(1, 2, 6)
#define COLOR_ITEM PRINT_COLOR(14, 15, 0)

// A window to create, as BmpWin_CreateDynamic takes it
typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} ZknSearchWindowData;

static void func_ov210_021be44c(ZknSearchWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align);
static void func_ov210_021be49c(PrintWindow *window);
static void func_ov210_021be4b4(ZknSearchWork *wk);
static void func_ov210_021becb8(ZknSearchWork *wk, u32 msg);

// pokebw2's sZknSearchWindows
static const ZknSearchWindowData data_ov210_021c0900[ZKNSEARCH_WIN_COUNT] = {
#include "asset/zknsearch_bmp_data_ov210_021c0900.inc"
};

// pokebw2's ZknSearchBmp_Init
void func_ov210_021be368(ZknSearchWork *wk) {
    u32 i;

    BmpWin_InitAllocator(HEAPID_ZUKAN_SEARCH);
    for (i = 0; i < ZKNSEARCH_WIN_COUNT; i++) {
        wk->windows[i].window = BmpWin_CreateDynamic(data_ov210_021c0900[i].bg, data_ov210_021c0900[i].x,
                                                     data_ov210_021c0900[i].y, data_ov210_021c0900[i].width,
                                                     data_ov210_021c0900[i].height, data_ov210_021c0900[i].palette, TRUE);
    }
    func_ov210_021be4b4(wk);
}

// pokebw2's ZknSearchBmp_Exit
void func_ov210_021be3b4(ZknSearchWork *wk) {
    u32 i;

    for (i = 0; i < ZKNSEARCH_WIN_COUNT; i++) {
        BmpWin_Free(wk->windows[i].window);
    }
    BmpWin_FreeAllocator();
}

// pokebw2's ZknSearchBmp_Update
void func_ov210_021be3d4(ZknSearchWork *wk) {
    u32 i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < ZKNSEARCH_WIN_COUNT; i++) {
        PrintWindow_Flush(&wk->windows[i], wk->printQueue);
    }
}

// pokebw2's ZknSearchBmp_Print
static void func_ov210_021be44c(ZknSearchWork *wk, u32 window, u32 msg, u32 x, u32 y, u16 color, u32 align) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, msg);

    func_ov067_021b6ae4(&wk->windows[window], wk->printQueue, x, y, strbuf, wk->font, color, align);
    GFL_StrBufFree(strbuf);
}

// pokebw2's ZknSearchBmp_PutWindow
static void func_ov210_021be49c(PrintWindow *window) {
    BmpWin_FlushMap(window->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window->window));
}

// pokebw2's ZknSearchBmp_PutLabels
static void func_ov210_021be4b4(ZknSearchWork *wk) {
    if (wk->param->criteria->regional == 0) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_MODE, Bank0479_Text_018, 0, 4, COLOR_WHITE,
                           PRINT_ALIGN_LEFT);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_MODE, Bank0479_Text_017, 0, 4, COLOR_WHITE,
                           PRINT_ALIGN_LEFT);
    }
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_ORDER_LABEL, Bank0479_Text_012, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INITIAL_LABEL, Bank0479_Text_013, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TYPE_LABEL, Bank0479_Text_014, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_COLOR_LABEL, Bank0479_Text_015, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_SHAPE_LABEL, Bank0479_Text_016, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_START, Bank0479_Text_020, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
}

// pokebw2's ZknSearchBmp_PutMain
void func_ov210_021be564(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_ORDER_LABEL]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INITIAL_LABEL]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TYPE_LABEL]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_COLOR_LABEL]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_SHAPE_LABEL]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_START]);
    func_ov210_021be7e4(wk);

    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_ORDER].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_ORDER, Bank0479_Text_025 + wk->param->criteria->order, 40, 4, COLOR_WHITE,
                       PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_ORDER]);

    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INITIAL].window), 0);
    if (wk->param->criteria->initial == ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_INITIAL, Bank0479_Text_095, 8, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_INITIAL, Bank0479_Text_051 + wk->param->criteria->initial, 8, 4, COLOR_WHITE,
                           PRINT_ALIGN_CENTER);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INITIAL]);

    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TYPE].window), 0);
    if (wk->param->criteria->type1 == ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TYPE, Bank0479_Text_048, 64, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else if (wk->param->criteria->type2 == ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TYPE, func_ov210_021bcf00(wk, wk->param->criteria->type1), 64, 4,
                           COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        // Two types, on either side of a separator
        StrBuf *strbuf;
        u8 width;
        u8 x;

        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0479_Text_049);
        func_ov067_021b6ae4(&wk->windows[ZKNSEARCH_WIN_TYPE], wk->printQueue, 66, 4, strbuf, wk->font, COLOR_WHITE,
                        PRINT_ALIGN_CENTER);
        width = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
        GFL_StrBufFree(strbuf);
        x = 64 - (u8)(width / 2);

        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, func_ov210_021bcf00(wk, wk->param->criteria->type1));
        func_ov067_021b6ae4(&wk->windows[ZKNSEARCH_WIN_TYPE], wk->printQueue, x + 2, 4, strbuf, wk->font, COLOR_WHITE,
                        PRINT_ALIGN_RIGHT);
        GFL_StrBufFree(strbuf);
        x += width;

        strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, func_ov210_021bcf00(wk, wk->param->criteria->type2));
        func_ov067_021b6ae4(&wk->windows[ZKNSEARCH_WIN_TYPE], wk->printQueue, x + 2, 4, strbuf, wk->font, COLOR_WHITE,
                        PRINT_ALIGN_LEFT);
        GFL_StrBufFree(strbuf);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TYPE]);

    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_COLOR].window), 0);
    if (wk->param->criteria->color == ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_COLOR, Bank0479_Text_106, 40, 4, COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_COLOR, Bank0479_Text_096 + wk->param->criteria->color, 40, 4, COLOR_WHITE,
                           PRINT_ALIGN_CENTER);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_COLOR]);
}

// pokebw2's ZknSearchBmp_PutInfo
void func_ov210_021be7e4(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_001, 0, 4, COLOR_BLACK,
                       PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
}

// pokebw2's ZknSearchBmp_ClearInfo
void func_ov210_021be820(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    BmpWin_FlushChar(wk->windows[ZKNSEARCH_WIN_INFO].window);
}

// pokebw2's ZknSearchBmp_PutOrder
void func_ov210_021be83c(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_004, 0, 4, COLOR_BLACK, PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_LABEL].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_LABEL, Bank0479_Text_012, 40, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_LABEL]);
    func_ov210_021be8b8(wk);
}

// pokebw2's ZknSearchBmp_PutOrderValue
void func_ov210_021be8b8(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_VALUE].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_025 + wk->param->criteria->order, 48, 0,
                       COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_VALUE]);
}

// pokebw2's ZknSearchBmp_PutInitial
void func_ov210_021be8f8(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_005, 0, 4, COLOR_BLACK,
                       PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_LABEL].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_LABEL, Bank0479_Text_013, 40, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_LABEL]);
    func_ov210_021be978(wk);
}

// pokebw2's ZknSearchBmp_PutInitialValue
void func_ov210_021be978(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_VALUE].window), 0);
    if (wk->param->criteria->initial != ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_051 + wk->param->criteria->initial, 48, 0,
                           COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_095, 48, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_VALUE]);
}

// pokebw2's ZknSearchBmp_PutType
void func_ov210_021be9d4(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_006, 0, 4, COLOR_BLACK, PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_LABEL].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_LABEL, Bank0479_Text_014, 40, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_LABEL]);
    func_ov210_021bea54(wk);
}

// pokebw2's ZknSearchBmp_PutTypeValue
void func_ov210_021bea54(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_VALUE].window), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_VALUE2].window), 0);
    if (wk->param->criteria->type1 != ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, func_ov210_021bcf00(wk, wk->param->criteria->type1), 48, 0,
                           COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_048, 48, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    }
    if (wk->param->criteria->type2 != ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE2, func_ov210_021bcf00(wk, wk->param->criteria->type2), 48,
                           0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE2, Bank0479_Text_048, 48, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_VALUE]);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_VALUE2]);
}

// pokebw2's ZknSearchBmp_PutColor
void func_ov210_021beb0c(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_008, 0, 4, COLOR_BLACK, PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_LABEL].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_LABEL, Bank0479_Text_015, 40, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_LABEL]);
    func_ov210_021beb8c(wk);
}

// pokebw2's ZknSearchBmp_PutColorValue
void func_ov210_021beb8c(ZknSearchWork *wk) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_VALUE].window), 0);
    if (wk->param->criteria->color != ZKNSEARCH_NONE) {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_096 + wk->param->criteria->color, 48, 0,
                           COLOR_WHITE, PRINT_ALIGN_CENTER);
    } else {
        func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_VALUE, Bank0479_Text_106, 48, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    }
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_VALUE]);
}

// pokebw2's ZknSearchBmp_PutShape
void func_ov210_021bebe8(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MODE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_INFO].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_INFO, Bank0479_Text_007, 0, 4, COLOR_BLACK, PRINT_ALIGN_LEFT);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_INFO]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_TOP_LABEL].window), 0);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_TOP_LABEL, Bank0479_Text_016, 40, 0, COLOR_WHITE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_TOP_LABEL]);
}

// pokebw2's ZknSearchBmp_PrintItem
void func_ov210_021bec60(ZknSearchWork *wk, PrintWindow *window, StrBuf *strbuf) {
    PrintQueue *queue = func_ov067_021b9394(wk->frameList);

    func_ov067_021b6ae4(window, queue, func_02045738(window->window) * 8 / 2, 4, strbuf, wk->font, COLOR_ITEM,
                    PRINT_ALIGN_CENTER);
}

// pokebw2's ZknSearchBmp_FlushItem
void func_ov210_021becac(ZknSearchWork *wk, PrintWindow *window) {
    BmpWin_FlushChar(window->window);
}

// pokebw2's ZknSearchBmp_PutMessage
static void func_ov210_021becb8(ZknSearchWork *wk, u32 msg) {
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[ZKNSEARCH_WIN_MESSAGE].window), 6);
    func_ov210_021be44c(wk, ZKNSEARCH_WIN_MESSAGE, msg, 120, 0, COLOR_MESSAGE, PRINT_ALIGN_CENTER);
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_MESSAGE]);
}

// pokebw2's ZknSearchBmp_PutSearching
void func_ov210_021becf8(ZknSearchWork *wk) {
    func_ov210_021becb8(wk, Bank0479_Text_011);
}

// pokebw2's ZknSearchBmp_PutFound
void func_ov210_021bed04(ZknSearchWork *wk) {
    func_ov210_021becb8(wk, Bank0479_Text_009);
}

// pokebw2's ZknSearchBmp_PutNotFound
void func_ov210_021bed10(ZknSearchWork *wk) {
    func_ov210_021becb8(wk, Bank0479_Text_010);
}

// pokebw2's ZknSearchBmp_PutStart
void func_ov210_021bed1c(ZknSearchWork *wk) {
    func_ov210_021be49c(&wk->windows[ZKNSEARCH_WIN_START]);
}
