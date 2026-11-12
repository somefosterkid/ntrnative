// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov139/frame_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// frame_list.c: a list of items that scrolls on one BG or two, by keys, by touching its rows, scroll bar and arrows, or
// by flicking it. The names are ours

#include "app/ui/frame_list.h"
#include "types.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/mi.h"
#include "system/bgwinfrm.h"
#include "system/blink_palanm.h"
#include "system/printsys.h"
#include "system/scroll_bar.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define LIST_CURSOR_NONE 0xff
#define FRAMELIST_BG_NONE 0xff


// frame_list.c: A list of items that scrolls, with a scroll bar and arrows. The name is the file's string; the rest
// of the names are ours

// What the list's states do: wait for input, scroll the rows, follow the scroll bar, follow a drag
#define LIST_STATE_INPUT 0
#define LIST_STATE_SCROLL 1
#define LIST_STATE_BAR 2
#define LIST_STATE_DRAG 3
#define LIST_STATE_CURSOR_WAIT 4

// What the keys and the touch screen ask for
enum {
    LIST_EVENT_CURSOR_SHOWN,
    LIST_EVENT_CURSOR_MOVE,
    LIST_EVENT_SCROLL_UP,
    LIST_EVENT_SCROLL_DOWN,
    LIST_EVENT_PAGE_UP,
    LIST_EVENT_PAGE_DOWN,
    LIST_EVENT_CURSOR_TOP,
    LIST_EVENT_CURSOR_BOTTOM,
    LIST_EVENT_JUMP_TOP,
    LIST_EVENT_JUMP_BOTTOM,
    LIST_EVENT_BAR,
    LIST_EVENT_TOUCH_ITEM,
    LIST_EVENT_DECIDE,
    LIST_EVENT_BLANK,
    LIST_EVENT_NONE,
};

// The screen height in tiles, and the BG's screen buffer height
#define LIST_SCREEN_ROWS 24
#define LIST_BG_ROWS 32

static void func_ov067_021b7b7c(FrameList *list);
static void func_ov067_021b7be0(FrameList *list, BOOL moved);
int func_ov067_021b7cf8(FrameList *list);
int func_ov067_021b7e70(FrameList *list);
int func_ov067_021b806c(FrameList *list);
void func_ov067_021b83a0(FrameList *list);
u8 func_ov067_021b8404(FrameList *list, s32 delta);
u32 func_ov067_021b841c(FrameList *list, s32 dir);
void func_ov067_021b8434(FrameList *list, s8 step, u8 count, u16 nextState, BOOL playSE);
BOOL func_ov067_021b8460(FrameList *list);
void func_ov067_021b8990(FrameList *list, u32 index);
u32 func_ov067_021b8a44(FrameList *list);
static void func_ov067_021b8a88(FrameList *list, u8 *top, u8 *bottom);

BOOL func_ov067_021b8b74(FrameList *list);
void func_ov067_021b8c14(FrameList *list, s16 item);
BOOL func_ov067_021b8c4c(FrameList *list);
BOOL func_ov067_021b8df0(FrameList *list, int item);
BOOL func_ov067_021b8ea0(FrameList *list);
static u8 func_ov067_021b8fa0(u8 bg);
void func_ov067_021b8fe4(FrameList *list, int item, int row);
static void func_ov067_021b90b8(FrameList *list, int row, s8 y);
static void func_ov067_021b9138(FrameList *list, u8 bg, s8 y);
s8 func_ov067_021b9190(FrameList *list, int dir);
s8 func_ov067_021b91bc(s8 pos, s8 count, s8 delta);
static void func_ov067_021b91d0(FrameList *list, int item, s8 y);
static void func_ov067_021b9228(FrameList *list, int item, s8 y);
static void func_ov067_021b92f0(FrameList *list, u16 newCursor, u16 oldCursor);
static void func_ov067_021b9344(FrameList *list);


// How many rows to scroll by at once, from the speed that a drag or flick started at
// pokebw2's sFlickRows
static const u8 data_ov067_021b94f8[6] = { 32, 24, 16, 10, 6, 4 };

// How far a drag has to go, by how long it has been held, for each speed
// pokebw2's sFlickDistances
static const u8 data_ov067_021b94fe[6][6] = {
#include "asset/frame_list_data_ov067_021b94fe.inc"
};

// pokebw2's FrameList_AddItem


FrameList *func_ov067_021b7754(const FrameListSetup *setup, HeapID heapId) {
    FrameList *list = GFL_HeapAllocate(heapId, sizeof(FrameList), TRUE, "frame_list.c", 245);
    u32 i;
    u32 n;

    list->setup = *setup;
    list->heapId = heapId;
    list->touchY = -1;
    list->prevTouchY = -1;
    list->printQueue = func_0201c450(0x800, heapId);
    list->blinkAnm = func_0202e940(list->setup.layout.palette << 4, 16, list->setup.layout.bg, list->heapId);
    list->items = GFL_HeapAllocate(list->heapId, list->setup.count * sizeof(FrameListItem), TRUE, "frame_list.c", 260);
    if (list->setup.screenCount != 0) {
        list->screens = GFL_HeapAllocate(list->heapId, list->setup.screenCount * sizeof(u16 *), FALSE, "frame_list.c", 265);
        for (i = 0; i < list->setup.screenCount; i++) {
            list->screens[i] = GFL_HeapAllocate(list->heapId, list->setup.layout.width * list->setup.layout.rowHeight * 2,
                                                FALSE, "frame_list.c", 267);
        }
    }
    list->rowCount = LIST_SCREEN_ROWS / list->setup.layout.rowHeight + 2;
    list->rowsPerBG = list->rowCount;
    if (list->setup.layout.bg2 != 0xff) {
        list->rowCount = list->rowCount * 2;
    }
    list->frames = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, list->rowCount, list->heapId);
    list->printWindows = GFL_HeapAllocate(list->heapId, list->rowCount * sizeof(PrintWindow), FALSE, "frame_list.c", 279);
    for (i = 0; i < list->rowCount; i++) {
        BmpWin *window;

        if (list->setup.layout.bg2 != 0xff && list->rowsPerBG <= i) {
            func_0202ca3c(list->frames, i, list->setup.layout.bg2, list->setup.layout.width,
                          list->setup.layout.rowHeight);
            window = BmpWin_CreateDynamic(list->setup.layout.bg2, list->setup.layout.windowX,
                                          list->setup.layout.windowY, list->setup.layout.windowWidth,
                                          list->setup.layout.windowHeight, list->setup.layout.windowPalette, TRUE);
        } else {
            func_0202ca3c(list->frames, i, list->setup.layout.bg, list->setup.layout.width,
                          list->setup.layout.rowHeight);
            window = BmpWin_CreateDynamic(list->setup.layout.bg, list->setup.layout.windowX,
                                          list->setup.layout.windowY, list->setup.layout.windowWidth,
                                          list->setup.layout.windowHeight, list->setup.layout.windowPalette, TRUE);
        }
        list->printWindows[i].window = window;
    }
    list->rowOffset = LIST_SCREEN_ROWS;
    do {
        list->rowOffset -= list->setup.layout.rowHeight;
    } while (list->rowOffset > 0);
    if (list->setup.touch != NULL) {
        n = 0;
        while (TRUE) {
            if (list->setup.touch[n].rect.top == TOUCH_RECT_END) {
                break;
            }
            n++;
        }
        list->touchRects = GFL_HeapAllocate(list->heapId, (n + 1) * sizeof(TouchRect), FALSE, "frame_list.c", 318);
        for (i = 0; i < n + 1; i++) {
            MIi_CpuCopy32(&list->setup.touch[i], &list->touchRects[i], sizeof(TouchRect));
        }
    }
    return list;
}

void func_ov067_021b796c(FrameList *list) {
    u32 i;

    GFL_HeapFree(list->touchRects);
    for (i = 0; i < list->rowCount; i++) {
        BmpWin_Free(list->printWindows[i].window);
    }
    GFL_HeapFree(list->printWindows);
    func_0202ca0c(list->frames);
    if (list->setup.screenCount != 0) {
        for (i = 0; i < list->setup.screenCount; i++) {
            GFL_HeapFree(list->screens[i]);
        }
        GFL_HeapFree(list->screens);
    }
    GFL_HeapFree(list->items);
    func_0202ea20(list->blinkAnm);
    func_0201c4c0(list->printQueue);
    GFL_HeapFree(list);
}

void func_ov067_021b79e4(FrameList *list, u32 type, u32 value) {
    list->items[list->count].screen = type;
    list->items[list->count].value = value;
    list->count++;
}

// pokebw2's FrameList_LoadScreen
void func_ov067_021b7a10(FrameList *list, ArcTool *arc, u32 fileId, BOOL compressed, u32 index) {
    NNSG2dScreenData *screen;
    void *data = GFL_G2DIOReadNSCRArc(arc, fileId, compressed, &screen, list->heapId);

    MIi_CpuCopy16(screen->rawData, list->screens[index], list->setup.layout.width * list->setup.layout.rowHeight * 2);
    GFL_HeapFree(data);
}

// pokebw2's FrameList_LoadCursorPalette
void func_ov067_021b7a4c(FrameList *list, ArcTool *arc, u32 fileId, u32 palette, u32 count) {
    func_0202e9ac(list->blinkAnm, arc, fileId, palette << 4, count << 4);
}

// pokebw2's FrameList_Draw
BOOL func_ov067_021b7a64(FrameList *list) {
    switch (list->state) {
    case 0:
        func_ov067_021b7b7c(list);
        func_ov067_021b7be0(list, FALSE);
        list->state++;
        break;
    case 1:
        if (func_0201c6b4(list->printQueue) == TRUE) {
            list->state = 0;
            return FALSE;
        }
        break;
    }
    func_ov067_021b9344(list);
    return TRUE;
}

u32 func_ov067_021b7ab0(FrameList *list) {
    u32 x;
    s32 result = FRAMELIST_NONE;

    if (!func_02035cb8(&x, (u32 *)&list->touchY)) {
        list->touchY = -1;
        list->flags.scrolledByDrag = 0;
    }
    switch (list->state) {
    case LIST_STATE_INPUT:
        result = func_ov067_021b806c(list);
        break;
    case LIST_STATE_SCROLL:
        if (func_ov067_021b8460(list) == TRUE) {
            result = -10;
        }
        break;
    case LIST_STATE_BAR:
        if (func_ov067_021b8b74(list) == TRUE) {
            result = -9;
        }
        break;
    case LIST_STATE_DRAG:
        if (func_ov067_021b8c4c(list) == TRUE) {
            result = -8;
        }
        break;
    case LIST_STATE_CURSOR_WAIT:
        if (!func_020355c4() && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A)) {
            result = list->cursor;
        }
        if (list->cursorDelay == 0) {
            list->state = LIST_STATE_INPUT;
        } else {
            list->cursorDelay--;
        }
        break;
    }
    list->prevTouchY = list->touchY;
    func_0202ea40(list->blinkAnm);
    func_ov067_021b9344(list);
    return result;
}


// pokebw2's InitCounts
static void func_ov067_021b7b7c(FrameList *list) {
    if (list->setup.visibleRows > list->count) {
        list->rows = list->count;
    } else {
        list->rows = list->setup.visibleRows;
    }
    if (list->count < list->setup.visibleRows) {
        list->maxScroll = 0;
    } else {
        list->maxScroll = list->count - list->setup.visibleRows;
    }
    if (list->setup.cursorPos > list->rows || list->setup.scroll > list->maxScroll) {
        list->cursor = 0;
        list->scroll = 0;
    } else {
        list->cursor = list->setup.cursorPos;
        list->scroll = list->setup.scroll;
    }
}

// pokebw2's DrawAll
static void func_ov067_021b7be0(FrameList *list, BOOL moved) {
    s16 item;
    s8 y;

    GFL_BGSysMoveBGReq(list->setup.layout.bg, 3, 0);
    list->bgOffset = 0;
    item = list->scroll;
    y = list->setup.layout.y;
    while (TRUE) {
        if (item == 0 || y <= 0) {
            break;
        }
        item--;
        y = y - (s8)list->setup.layout.rowHeight;
    }
    list->firstRow = 0;
    while (TRUE) {
        if (y >= LIST_SCREEN_ROWS) {
            break;
        }
        func_ov067_021b91d0(list, item, y);
        list->firstRow++;
        item++;
        y = y + (s8)list->setup.layout.rowHeight;
    }
    list->shownRows = list->firstRow;
    if (list->setup.layout.bg2 != 0xff) {
        GFL_BGSysMoveBGReq(list->setup.layout.bg2, 3, 0);
        y = list->setup.layout.y + LIST_SCREEN_ROWS - list->setup.layout.rowHeight;
        item = list->scroll - 1;
        while (TRUE) {
            if (y <= 0) {
                break;
            }
            item--;
            y = y - (s8)list->setup.layout.rowHeight;
        }
        list->firstRow2 = 0;
        while (TRUE) {
            if (y >= LIST_SCREEN_ROWS) {
                break;
            }
            func_ov067_021b9228(list, item, y);
            list->firstRow2++;
            item++;
            y = y + (s8)list->setup.layout.rowHeight;
        }
        list->shownRows2 = list->firstRow2;
        func_ov067_021b92f0(list, list->cursor, 0xff);
        list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, moved);
    }
}

// What the touch screen asks for
// pokebw2's TouchEvent
int func_ov067_021b7cf8(FrameList *list) {
    s32 touched;
    BOOL touchMode;

    if (list->touchRects == NULL) {
        return LIST_EVENT_NONE;
    }
    touched = func_02035c08(list->touchRects);
    if (touched == TOUCH_RECT_NONE) {
        return LIST_EVENT_NONE;
    }
    touchMode = func_020355c4();
    func_020355d8(TRUE);
    switch (list->setup.touch[touched].action) {
    case FRAMELIST_TOUCH_ITEM_DECIDE:
        if (touched >= list->rows) {
            return LIST_EVENT_BLANK;
        }
        list->prevCursor = list->cursor;
        list->cursor = touched;
        return LIST_EVENT_DECIDE;
    case FRAMELIST_TOUCH_ITEM:
        if (touched >= list->rows) {
            return LIST_EVENT_BLANK;
        }
        list->prevCursor = list->cursor;
        list->cursor = touched;
        return LIST_EVENT_TOUCH_ITEM;
    case FRAMELIST_TOUCH_BAR:
        if (list->count > list->rows) {
            list->barIndex = touched;
            list->prevCursor = list->cursor;
            return LIST_EVENT_BAR;
        }
        func_020355d8(touchMode);
        break;
    case FRAMELIST_TOUCH_UP:
        if (list->scroll != 0) {
            list->repeatStage = 0;
            return LIST_EVENT_SCROLL_UP;
        }
        break;
    case FRAMELIST_TOUCH_DOWN:
        if (list->scroll < list->maxScroll) {
            list->repeatStage = 0;
            return LIST_EVENT_SCROLL_DOWN;
        }
        break;
    case FRAMELIST_TOUCH_PAGE_UP:
        if (list->scroll != 0) {
            return LIST_EVENT_PAGE_UP;
        }
        if (list->cursor != 0) {
            list->prevCursor = list->cursor;
            list->cursor = 0;
            return LIST_EVENT_CURSOR_TOP;
        }
        break;
    case FRAMELIST_TOUCH_PAGE_DOWN:
        if (list->scroll < list->maxScroll) {
            return LIST_EVENT_PAGE_DOWN;
        }
        if (list->cursor < list->rows - 1) {
            list->prevCursor = list->cursor;
            list->cursor = list->rows - 1;
            return LIST_EVENT_CURSOR_BOTTOM;
        }
        break;
    case FRAMELIST_TOUCH_TOP:
        if (list->cursor != 0 || list->scroll != 0) {
            return LIST_EVENT_JUMP_TOP;
        }
        break;
    case FRAMELIST_TOUCH_BOTTOM:
        if (list->cursor != list->rows - 1 || list->scroll != list->maxScroll) {
            return LIST_EVENT_JUMP_BOTTOM;
        }
        break;
    }
    return LIST_EVENT_NONE;
}

// What the keys ask for
// pokebw2's KeyEvent
int func_ov067_021b7e70(FrameList *list) {
    if ((GCTX_HIDGetPressedKeys() & 0x3f1) && func_020355c4() == TRUE) {
        func_020355d8(FALSE);
        list->prevCursor = 0xff;
        list->cursorDelay = 8;
        return LIST_EVENT_CURSOR_SHOWN;
    }
    if (GCTX_HIDGetHeldKeys() == 0) {
        list->repeatTimer = 0;
        list->repeatStage = 6;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_UP) {
        func_020355d8(FALSE);
        if (list->cursor != 0) {
            list->prevCursor = list->cursor;
            list->cursor--;
            list->cursorDelay = 8;
            return LIST_EVENT_CURSOR_MOVE;
        }
        if (list->scroll != 0) {
            list->cursorDelay = 0;
            func_ov067_021b83a0(list);
            return LIST_EVENT_SCROLL_UP;
        }
        return LIST_EVENT_NONE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_DOWN) {
        func_020355d8(FALSE);
        if (list->cursor < list->rows - 1) {
            list->prevCursor = list->cursor;
            list->cursor++;
            list->cursorDelay = 8;
            return LIST_EVENT_CURSOR_MOVE;
        }
        if (list->scroll < list->maxScroll) {
            list->cursorDelay = 0;
            func_ov067_021b83a0(list);
            return LIST_EVENT_SCROLL_DOWN;
        }
        return LIST_EVENT_NONE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_LEFT) {
        func_020355d8(FALSE);
        if (list->scroll != 0) {
            list->cursorDelay = 4;
            return LIST_EVENT_PAGE_UP;
        }
        if (list->cursor != 0) {
            list->prevCursor = list->cursor;
            list->cursor = 0;
            list->cursorDelay = 4;
            return LIST_EVENT_CURSOR_TOP;
        }
        return LIST_EVENT_NONE;
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_RIGHT) {
        func_020355d8(FALSE);
        if (list->scroll < list->maxScroll) {
            list->cursorDelay = 4;
            return LIST_EVENT_PAGE_DOWN;
        }
        if (list->cursor < list->rows - 1) {
            list->prevCursor = list->cursor;
            list->cursor = list->rows - 1;
            list->cursorDelay = 4;
            return LIST_EVENT_CURSOR_BOTTOM;
        }
        return LIST_EVENT_NONE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        return LIST_EVENT_DECIDE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_L) {
        func_020355d8(FALSE);
        if (list->cursor != 0 || list->scroll != 0) {
            return LIST_EVENT_JUMP_TOP;
        }
        return LIST_EVENT_NONE;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_R) {
        func_020355d8(FALSE);
        if (list->cursor != list->rows - 1 || list->scroll != list->maxScroll) {
            return LIST_EVENT_JUMP_BOTTOM;
        }
        return LIST_EVENT_NONE;
    }
    return LIST_EVENT_NONE;
}

// Does what the input asked for, and returns what the list's user has to know about it
// pokebw2's HandleEvent
int func_ov067_021b806c(FrameList *list) {
    s16 rows;

    if (func_0201c6b4(list->printQueue) == FALSE) {
        return FRAMELIST_NONE;
    }
    {
        int event = func_ov067_021b7cf8(list);

        if (event == LIST_EVENT_NONE) {
            event = func_ov067_021b7e70(list);
        }
        switch (event) {
        case LIST_EVENT_CURSOR_SHOWN:
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov067_021b92f0(list, list->cursor, list->prevCursor);
            list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            list->state = LIST_STATE_CURSOR_WAIT;
            return -12;
        case LIST_EVENT_CURSOR_MOVE:
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov067_021b92f0(list, list->cursor, list->prevCursor);
            list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            list->state = LIST_STATE_CURSOR_WAIT;
            return -11;
        case LIST_EVENT_SCROLL_UP:
            func_ov067_021b8434(list, -list->setup.layout.speed[list->repeatStage], 1, LIST_STATE_CURSOR_WAIT, TRUE);
            return -10;
        case LIST_EVENT_SCROLL_DOWN:
            func_ov067_021b8434(list, list->setup.layout.speed[list->repeatStage], 1, LIST_STATE_CURSOR_WAIT, TRUE);
            return -10;
        case LIST_EVENT_PAGE_UP:
            rows = list->rows;
            list->prevCursor = list->cursor;
            if (list->scroll - rows < 0) {
                list->cursor = 0;
                rows = list->scroll;
            }
            func_ov067_021b92f0(list, 0xff, list->prevCursor);
            func_ov067_021b8434(list, -list->setup.layout.speed[0], rows, LIST_STATE_CURSOR_WAIT, FALSE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return -7;
        case LIST_EVENT_PAGE_DOWN:
            rows = list->rows;
            list->prevCursor = list->cursor;
            if (list->scroll + rows > list->maxScroll) {
                list->cursor = list->rows - 1;
                rows = list->maxScroll - list->scroll;
            }
            func_ov067_021b92f0(list, 0xff, list->prevCursor);
            func_ov067_021b8434(list, list->setup.layout.speed[0], rows, LIST_STATE_CURSOR_WAIT, FALSE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return -6;
        case LIST_EVENT_CURSOR_TOP:
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov067_021b92f0(list, list->cursor, list->prevCursor);
            list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            list->state = LIST_STATE_CURSOR_WAIT;
            return -5;
        case LIST_EVENT_CURSOR_BOTTOM:
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov067_021b92f0(list, list->cursor, list->prevCursor);
            list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            list->state = LIST_STATE_CURSOR_WAIT;
            return -4;
        case LIST_EVENT_JUMP_TOP:
            list->cursor = 0;
            list->scroll = 0;
            func_ov067_021b7be0(list, TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return -3;
        case LIST_EVENT_JUMP_BOTTOM:
            list->cursor = list->rows - 1;
            list->scroll = list->maxScroll;
            func_ov067_021b7be0(list, TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return -2;
        case LIST_EVENT_BAR:
            func_ov067_021b92f0(list, 0xff, list->prevCursor);
            func_ov067_021b8990(list, list->barIndex);
            GFL_SndSEPlay(SEQ_SE_SYS_06);
            return -9;
        case LIST_EVENT_TOUCH_ITEM:
            func_ov067_021b92f0(list, 0xff, list->prevCursor);
            list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            func_ov067_021b8c14(list, list->cursor);
            GFL_SndSEPlay(SEQ_SE_SYS_06);
            return -8;
        case LIST_EVENT_DECIDE:
            if (func_020355c4() == TRUE) {
                func_ov067_021b92f0(list, list->cursor, list->prevCursor);
                list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
            }
            return list->cursor;
        case LIST_EVENT_BLANK:
        case LIST_EVENT_NONE:
        default:
            return FRAMELIST_NONE;
        }
    }
}

// Speeds the cursor's repeat up the longer a key is held
// pokebw2's UpdateKeyRepeat
void func_ov067_021b83a0(FrameList *list) {
    if (list->repeatTimer < 20) {
        list->repeatTimer++;
    }
    if (list->repeatTimer == 20) {
        list->repeatStage = 0;
    } else if (list->repeatTimer >= 16) {
        list->repeatStage = 1;
    } else if (list->repeatTimer >= 12) {
        list->repeatStage = 2;
    } else if (list->repeatTimer >= 8) {
        list->repeatStage = 3;
    } else if (list->repeatTimer >= 4) {
        list->repeatStage = 4;
    } else {
        list->repeatStage = 5;
    }
}

// How many frames a step of delta pixels waits to move a row
// pokebw2's GetScrollFrames
u8 func_ov067_021b8404(FrameList *list, s32 delta) {
    if (delta < 0) {
        delta = -delta;
    }
    return (list->setup.layout.rowHeight * 8) / delta;
}

// How many rows the list can scroll in a direction
// pokebw2's GetScrollRoom
u32 func_ov067_021b841c(FrameList *list, s32 dir) {
    if (dir < 0) {
        return list->scroll;
    }
    return list->maxScroll - list->scroll;
}

// Starts scrolling count rows, step pixels a frame, and goes on to nextState
// pokebw2's StartScroll
void func_ov067_021b8434(FrameList *list, s8 step, u8 count, u16 nextState, BOOL playSE) {
    list->scrollDir = step;
    list->scrollStep = count;
    list->scrollFrames = 0;
    list->nextState = nextState;
    list->state = LIST_STATE_SCROLL;
    list->playSE = playSE;
}

// pokebw2's UpdateScroll
BOOL func_ov067_021b8460(FrameList *list) {
    s32 touched;

    if (func_0201c6b4(list->printQueue) == FALSE) {
        return TRUE;
    }
    if (list->nextState == LIST_STATE_DRAG && list->flicking == TRUE) {
        touched = func_02035ba4(list->touchRects);
        if (list->flags.touching == TRUE) {
            if (!list->flags.dragging) {
                if (touched == TOUCH_RECT_NONE) {
                    int delta;

                    list->dragDelta = list->flickEndY - list->flickStartY;
                    delta = list->dragDelta;
                    if (delta < 0) {
                        delta = -delta;
                    }
                    if (delta >= 16) {
                        list->flags.touchFrames = 0;
                        list->flags.touching = 0;
                        list->flags.dragging = 1;
                    }
                } else if (list->flags.touchFrames != 3) {
                    list->flickStartY = list->touchY;
                    list->flags.touchFrames++;
                }
            }
        } else if (touched != TOUCH_RECT_NONE && list->setup.touch[touched].action == FRAMELIST_TOUCH_ITEM) {
            list->cursor = touched;
            list->flags.touching = 1;
            list->flags.touchFrames = 0;
            list->flickStartY = list->touchY;
            list->flickEndY = list->touchY;
        }
    }
    if (list->scrollFrames == 0) {
        if (list->scrollStep == 0) {
            touched = func_02035ba4(list->touchRects);
            if (touched != TOUCH_RECT_NONE && list->setup.touch[touched].action == FRAMELIST_TOUCH_ITEM &&
                list->flags.scrolledByDrag == 1 && list->setup.touch[touched].action == FRAMELIST_TOUCH_ITEM &&
                ((list->dragDelta < 0 && list->dragItem < touched) ||
                 (list->dragDelta > 0 && list->dragItem > touched))) {
                u32 available;

                list->flags.scrolledByDrag = 0;
                available = func_ov067_021b841c(list, list->scrollDir);
                if (available != 0) {
                    int diff = list->dragItem - touched;

                    if (diff < 0) {
                        diff = -diff;
                    }
                    list->scrollStep = diff;
                    if (list->scrollStep > available) {
                        list->scrollStep = available;
                    }
                    list->dragItem = touched;
                } else {
                    list->flickStartY = list->touchY;
                    list->flickEndY = list->touchY;
                    list->flags.touching = 0;
                    func_ov067_021b92f0(list, list->cursor, LIST_CURSOR_NONE);
                    list->state = list->nextState;
                    return FALSE;
                }
            } else {
                list->flickStartY = list->touchY;
                list->flickEndY = list->touchY;
                list->flags.scrolledByDrag = 0;
                list->flags.touching = 0;
                func_ov067_021b92f0(list, list->cursor, LIST_CURSOR_NONE);
                list->state = list->nextState;
                return FALSE;
            }
        }
        func_ov067_021b92f0(list, LIST_CURSOR_NONE, list->cursor);
        if (list->nextState == LIST_STATE_DRAG) {
            if (list->flicking == 0) {
                if (list->scrollDir < 0) {
                    list->cursor = list->cursor + 1;
                } else {
                    list->cursor = list->cursor - 1;
                }
            } else {
                if (list->flags.dragging == 1) {
                    if (func_ov067_021b8ea0(list) == TRUE) {
                        list->flags.touchFrames = 0;
                    }
                    list->flags.dragging = 0;
                }
                if (list->flags.touching == 1 && list->flags.touchFrames == 3) {
                    touched = func_02035ba4(list->touchRects);
                    if (touched != TOUCH_RECT_NONE && list->setup.touch[touched].action == FRAMELIST_TOUCH_ITEM) {
                        list->cursor = touched;
                    }
                    list->flicking = 0;
                    list->dragItem = list->cursor;
                    list->flags.touching = 0;
                    list->flags.touchFrames = 0;
                    list->flickStartY = list->touchY;
                    list->flickEndY = list->touchY;
                    list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
                    list->state = list->nextState;
                    return FALSE;
                }
                if (list->scrollStep == 1) {
                    u32 available = func_ov067_021b841c(list, list->scrollDir);

                    list->speedIndex++;
                    if (list->speedIndex != 6) {
                        if (list->scrollDir < 0) {
                            list->scrollDir = -list->setup.layout.speed[list->speedIndex];
                        } else {
                            list->scrollDir = list->setup.layout.speed[list->speedIndex];
                        }
                        list->scrollStep = data_ov067_021b94f8[list->speedIndex];
                    }
                    if (list->scrollStep > available) {
                        list->scrollStep = available;
                    }
                }
            }
        }
        list->scrollFrames = func_ov067_021b8404(list, list->scrollDir);
        list->scrollStep--;
        if (list->scrollDir < 0) {
            list->scroll--;
            func_ov067_021b91d0(list, list->scroll, func_ov067_021b9190(list, -1));
            list->firstRow = func_ov067_021b91bc(list->firstRow, list->shownRows, -1);
            func_ov067_021b9228(list, list->scroll - list->shownRows2, list->rowOffset + func_ov067_021b9190(list, -1));
            list->firstRow2 = func_ov067_021b91bc(list->firstRow2, list->shownRows2, -1);
        } else {
            list->scroll++;
            func_ov067_021b91d0(list, list->scroll + (list->shownRows - 1), func_ov067_021b9190(list, 1));
            list->firstRow = func_ov067_021b91bc(list->firstRow, list->shownRows, 1);
            func_ov067_021b9228(list, list->scroll - 1, list->rowOffset + func_ov067_021b9190(list, 1));
            list->firstRow2 = func_ov067_021b91bc(list->firstRow2, list->shownRows2, 1);
        }
        list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
        if (list->playSE == TRUE) {
            if (list->nextState == LIST_STATE_BAR || list->nextState == LIST_STATE_DRAG) {
                GFL_SndSEPlay(SEQ_SE_SYS_06);
            } else {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    }
    list->bgOffset = list->bgOffset + list->scrollDir;
    GFL_BGSysMoveBGReq(list->setup.layout.bg, BG_MOVE_SET_Y, list->bgOffset);
    if (list->setup.layout.bg2 != FRAMELIST_BG_NONE) {
        GFL_BGSysMoveBGReq(list->setup.layout.bg2, BG_MOVE_SET_Y, list->bgOffset);
    }
    list->setup.callbacks->scroll(list->setup.work, list->scrollDir);
    list->scrollFrames--;
    return TRUE;
}

// Starts following the scroll bar from the rectangle that was touched
// pokebw2's StartBar
void func_ov067_021b8990(FrameList *list, u32 index) {
    u32 x;
    u32 top;
    int bottom;
    u32 left;
    int right;

    list->barTop = list->touchRects[index].top;
    list->barBottom = list->touchRects[index].bottom;
    list->barTouch[0].top = list->touchRects[index].top;
    list->barTouch[0].bottom = list->touchRects[index].bottom;
    list->barTouch[0].left = list->touchRects[index].left;
    list->barTouch[0].right = list->touchRects[index].right;
    top = list->barTouch[0].top;
    if (top >= 32) {
        top = top - 32;
    } else {
        top = 0;
    }
    list->barTouch[0].top = top;
    bottom = list->barTouch[0].bottom + 32;
    if (bottom <= 191) {
        list->barTouch[0].bottom = bottom;
    } else {
        list->barTouch[0].bottom = 191;
    }
    left = list->barTouch[0].left;
    if (left >= 0) {
        list->barTouch[0].left = left;
    } else {
        list->barTouch[0].left = 0;
    }
    right = list->barTouch[0].right;
    if (right <= 255) {
        list->barTouch[0].right = right;
    } else {
        list->barTouch[0].right = 255;
    }
    list->barTouch[1].top = TOUCH_RECT_END;
    if (!func_02035cb8(&x, (u32 *)&list->barTouchY)) {
        list->barTouchY = -1;
    }
    list->barDragging = TRUE;
    list->state = LIST_STATE_BAR;
}

// The item that the scroll bar's position points at
// pokebw2's GetBarTouchValue
u32 func_ov067_021b8a44(FrameList *list) {
    u32 x;
    u32 y;

    if (!func_02035cb8(&x, &y)) {
        y = list->barTouchY;
    }
    list->barTouchY = -1;
    return func_0202ede4(list->maxScroll, y, list->barTop, list->barBottom, list->setup.layout.barSize);
}

// The scroll bar's top and bottom, from the first rectangle of the touch table that is the bar
// pokebw2's GetBarRange
static void func_ov067_021b8a88(FrameList *list, u8 *top, u8 *bottom) {
    u8 i = 0;
    const FrameListTouch *touch = list->setup.touch;

    while (TRUE) {
        const FrameListTouch *entry = &touch[i];

        if (entry->rect.top == TOUCH_RECT_END) {
            break;
        }
        if (entry->action == FRAMELIST_TOUCH_BAR) {
            *top = entry->rect.top;
            *bottom = list->setup.touch[i].rect.bottom;
            return;
        }
        i++;
    }
}

// pokebw2's FrameList_GetBarPos
u32 func_ov067_021b8ab8(FrameList *list) {
    u8 top;
    u8 bottom;

    func_ov067_021b8a88(list, &top, &bottom);
    return func_0202ee0c(list->maxScroll, list->scroll, top, bottom, list->setup.layout.barSize);
}

// pokebw2's FrameList_ClampBarPos
int func_ov067_021b8ae4(FrameList *list, int y) {
    u8 top;
    u8 bottom;
    u32 x;
    u32 touchY;
    u32 start;
    u32 end;

    if (list->barDragging == TRUE) {
        if (func_02035ba4(&list->barTouch[0]) != TOUCH_RECT_NONE) {
            func_02035cb8(&x, &touchY);
            return touchY;
        }
        return y;
    }
    func_ov067_021b8a88(list, &top, &bottom);
    start = func_0202ee0c(list->maxScroll, list->scroll, top, bottom, list->setup.layout.barSize);
    if (list->maxScroll == list->scroll) {
        end = bottom;
    } else {
        end = func_0202ee0c(list->maxScroll, list->scroll + 1, top, bottom, list->setup.layout.barSize);
    }
    if (y >= start && y < end) {
        return y;
    }
    return start;
}

// Follows the scroll bar
// pokebw2's UpdateBar
BOOL func_ov067_021b8b74(FrameList *list) {
    u32 target;
    u32 diff;

    if (func_0201c6b4(list->printQueue) == FALSE) {
        return TRUE;
    }
    if (func_02035ba4(&list->barTouch[0]) == TOUCH_RECT_NONE && list->barTouchY == -1) {
        list->barDragging = FALSE;
        list->state = LIST_STATE_INPUT;
        return FALSE;
    }
    target = func_ov067_021b8a44(list);
    diff = list->scroll - target;
    if ((s32)diff < 0) {
        diff = -diff;
    }
    if (diff > list->shownRows) {
        diff = list->shownRows;
    }
    if (list->scroll > target) {
        list->scroll = target + diff;
        func_ov067_021b8434(list, -list->setup.layout.speed[0], diff, LIST_STATE_BAR, TRUE);
    } else if (list->scroll < target) {
        list->scroll = target - diff;
        func_ov067_021b8434(list, list->setup.layout.speed[0], diff, LIST_STATE_BAR, TRUE);
    }
    return TRUE;
}

// Starts following a touch that began on an item
// pokebw2's StartDrag
void func_ov067_021b8c14(FrameList *list, s16 item) {
    list->dragState = 0;
    list->dragItem = item;
    list->flags.touchFrames = 0;
    list->flickStartY = list->touchY;
    list->flickEndY = list->touchY;
    list->flicking = FALSE;
    list->state = LIST_STATE_DRAG;
}

// Follows a touch that began on an item, and scrolls the list when it is dragged or flicked
// pokebw2's UpdateDrag
BOOL func_ov067_021b8c4c(FrameList *list) {
    s32 touched;

    if (func_0201c6b4(list->printQueue) == FALSE) {
        return TRUE;
    }
    touched = func_02035ba4(list->touchRects);
    switch (list->dragState) {
    case 0:
        if (touched == TOUCH_RECT_NONE) {
            if (list->flickStartY != -1 && list->flickEndY != -1) {
                list->dragDelta = list->flickEndY - list->flickStartY;
                list->flags.touchFrames = 0;
                if (func_ov067_021b8ea0(list) == TRUE) {
                    break;
                }
            }
            list->flicking = FALSE;
            list->state = LIST_STATE_INPUT;
            return FALSE;
        }
        if (list->setup.touch[touched].action != FRAMELIST_TOUCH_ITEM) {
            if (list->flickStartY != -1 && list->flickEndY != -1) {
                list->dragDelta = list->flickEndY - list->flickStartY;
                list->flags.touchFrames = 0;
                if (func_ov067_021b8ea0(list) == TRUE) {
                    break;
                }
            }
            list->flicking = FALSE;
            list->state = LIST_STATE_INPUT;
            return FALSE;
        }
        if (list->dragItem != touched) {
            list->dragItemPrev = touched;
            list->dragDelta = list->dragItem - touched;
            list->flags.touchFrames = 0;
            list->dragState = 1;
        }
        list->flickEndY = list->flickStartY;
        list->flickStartY = list->touchY;
        break;
    case 1:
        if (touched == TOUCH_RECT_NONE) {
            list->dragState = 0;
            return func_ov067_021b8ea0(list);
        }
        if (list->setup.touch[touched].action != FRAMELIST_TOUCH_ITEM) {
            list->dragState = 0;
            return func_ov067_021b8ea0(list);
        }
        if (list->flags.touchFrames == 5) {
            list->flags.touchFrames = 0;
            list->dragState = 0;
            if (func_ov067_021b8df0(list, touched) == TRUE) {
                list->flags.scrolledByDrag = 1;
            }
        } else {
            list->dragItemPrev = touched;
            list->flickStartY = list->touchY;
            list->flags.touchFrames++;
        }
        break;
    }
    return TRUE;
}

// Scrolls toward the item that a drag has moved to
// pokebw2's ScrollToTouched
BOOL func_ov067_021b8df0(FrameList *list, int item) {
    int diff = list->dragItem - item;
    s8 step;
    s32 sign = 0;

    if ((diff < 0 && list->dragDelta > 0) || (diff > 0 && list->dragDelta < 0)) {
        return FALSE;
    }
    if ((list->dragDelta < 0 && list->scroll == 0) || (list->dragDelta > 0 && list->scroll == list->maxScroll)) {
        return FALSE;
    }
    if (diff < 0) {
        diff = -diff;
    }
    if (diff == 0) {
        return FALSE;
    }
    if (diff == 1) {
        step = list->setup.layout.speed[3];
    } else if (diff == 2) {
        step = list->setup.layout.speed[2];
    } else if (diff == 3) {
        step = list->setup.layout.speed[1];
    } else {
        step = list->setup.layout.speed[0];
    }
    if (item > list->dragItem) {
        sign = ~sign;
        step *= sign;
    }
    {
        u32 available = func_ov067_021b841c(list, step);

        if (available != 0) {
            if (available < diff) {
                diff = available;
            }
            func_ov067_021b8434(list, step, diff, LIST_STATE_DRAG, TRUE);
            list->dragItem = item;
            return TRUE;
        }
    }
    return FALSE;
}

// Keeps scrolling after a flick, as far and as fast as it was
// pokebw2's StartFlick
BOOL func_ov067_021b8ea0(FrameList *list) {
    u32 diff;
    u32 i;
    s8 step;
    s32 sign = 0;

    if ((list->dragDelta < 0 && list->flickEndY > list->flickStartY) ||
        (list->dragDelta > 0 && list->flickEndY < list->flickStartY)) {
        return FALSE;
    }
    diff = list->flickEndY - list->flickStartY;
    if ((s32)diff < 0) {
        diff = -diff;
    }
    if (diff == 0) {
        return FALSE;
    }
    list->speedIndex = 0;
    for (i = list->speedIndex; i < 6; i = list->speedIndex) {
        if (diff >= data_ov067_021b94fe[list->flags.touchFrames][i]) {
            step = list->setup.layout.speed[i];
            diff = data_ov067_021b94f8[i];
            break;
        }
        list->speedIndex++;
    }
    if (i == 6) {
        return FALSE;
    }
    if (list->flickStartY > list->flickEndY) {
        sign = ~sign;
        step *= sign;
    }
    {
        u32 available = func_ov067_021b841c(list, step);

        if (available != 0) {
            if (available < diff) {
                diff = available;
            }
            func_ov067_021b8434(list, step, diff, LIST_STATE_DRAG, TRUE);
            list->flicking = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's GetBgNo
static u8 func_ov067_021b8fa0(u8 bg) {
    if (bg == 0) {
        return 0;
    }
    if (bg == 1) {
        return 1;
    }
    if (bg == 2) {
        return 2;
    }
    if (bg == 3) {
        return 3;
    }
    if (bg == 4) {
        return 4;
    }
    if (bg == 5) {
        return 5;
    }
    if (bg == 6) {
        return 6;
    }
    if (bg == 7) {
        return 7;
    }
    return 0;
}

// Draws an item's frame, with its text, into the row's window
// pokebw2's DrawItemFrame
void func_ov067_021b8fe4(FrameList *list, int item, int row) {
    BmpWin *window = list->printWindows[row].window;
    u16 *screen = list->screens[list->items[item].screen];
    u8 *pixels;
    u8 *tiles;
    u8 screenWidth;
    u8 width;
    u8 height;
    u8 x;
    u8 y;
    u8 i;
    u8 j;

    tiles = func_02051b0c(func_ov067_021b8fa0(func_0202cf8c(list->frames, row)));
    func_0202cac4(list->frames, row, screen);
    func_0202cebc(list->frames, row, window);
    screenWidth = list->setup.layout.width;
    width = BmpWin_GetWidth1(window);
    height = BmpWin_GetHeight2(window);
    x = BmpWin_GetPosX(window);
    y = BmpWin_GetPosY(window);
    pixels = GFL_BitmapGetPixelData(BmpWin_GetBitmap(window));
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            u16 entry = screen[x + (j + y) * screenWidth + i];

            MIi_CpuCopy32(tiles + (entry & 0x3ff) * 32, pixels + (i + j * width) * 32, 32);
        }
    }
}

static void func_ov067_021b90b8(FrameList *list, int row, s8 y) {
    u16 *screen = func_0202cf84(list->frames, row);
    u32 bg = func_0202cf8c(list->frames, row);
    u16 i;

    for (i = 0; i < list->setup.layout.rowHeight; i++) {
        if (y < 0) {
            y += LIST_BG_ROWS;
        } else if (y >= LIST_BG_ROWS) {
            y -= LIST_BG_ROWS;
        }
        GFL_BGSysLoadScrArea(bg, list->setup.layout.x, y, list->setup.layout.width, 1, screen, 0, i,
                             list->setup.layout.width, list->setup.layout.rowHeight);
        y++;
    }
    GFL_BGSysQueueScrLoad(bg);
}


// Clears a row of a BG
// pokebw2's ClearRow
static void func_ov067_021b9138(FrameList *list, u8 bg, s8 y) {
    u32 i;

    for (i = 0; i < list->setup.layout.rowHeight; i++) {
        if (y < 0) {
            y += LIST_BG_ROWS;
        } else if (y >= LIST_BG_ROWS) {
            y -= LIST_BG_ROWS;
        }
        GFL_BGSysFillScrArea(bg, 0, list->setup.layout.x, y, list->setup.layout.width, 1, 0);
        y++;
    }
    GFL_BGSysQueueScrLoad(bg);
}

// Where the row that scrolls in goes
// pokebw2's GetOffscreenRowY
s8 func_ov067_021b9190(FrameList *list, int dir) {
    s8 pos = list->setup.layout.y;
    s8 step = list->setup.layout.rowHeight * dir;
    do {
        pos += step;
    } while (pos >= 0 && pos < LIST_SCREEN_ROWS);
    return pos + list->bgOffset / 8;
}

// Moves a ring of rows on by delta
// pokebw2's WrapRow
s8 func_ov067_021b91bc(s8 pos, s8 count, s8 delta) {
    s8 next = pos + delta;

    if (next < 0) {
        return count;
    }
    if (next > count) {
        return 0;
    }
    return next;
}

// pokebw2's PrintRow
static void func_ov067_021b91d0(FrameList *list, int item, s8 y) {
    if (item >= 0 && item < list->count) {
        func_ov067_021b8fe4(list, item, list->firstRow);
        func_ov067_021b90b8(list, list->firstRow, y);
        list->setup.callbacks->print(list->setup.work, item, &list->printWindows[list->firstRow], y * 8 - list->bgOffset, TRUE);
    } else {
        func_ov067_021b9138(list, list->setup.layout.bg, y);
    }
}

// pokebw2's PrintRowOnBg2
static void func_ov067_021b9228(FrameList *list, int item, s8 y) {
    if (list->setup.layout.bg2 != 0xff) {
        if (item >= 0) {
            int row = list->firstRow2 + list->rowsPerBG;

            func_ov067_021b8fe4(list, item, row);
            func_ov067_021b90b8(list, row, y);
            list->setup.callbacks->print(list->setup.work, item, &list->printWindows[row], y * 8 - list->bgOffset, FALSE);
        } else {
            func_ov067_021b9138(list, list->setup.layout.bg2, y);
        }
    }
}

// Sets the palette of an item's row
// pokebw2's FrameList_SetRowPalette
void func_ov067_021b9280(FrameList *list, u32 row, u32 palette) {
    s16 y = list->bgOffset / 8 + (list->setup.layout.y + row * list->setup.layout.rowHeight);
    u16 i;

    for (i = 0; i < list->setup.layout.rowHeight; i++) {
        if (y < 0) {
            y += LIST_BG_ROWS;
        } else if (y >= LIST_BG_ROWS) {
            y -= LIST_BG_ROWS;
        }
        GFL_BGSysSetScrPaletteNo(list->setup.layout.bg, list->setup.layout.x, y, list->setup.layout.width, 1,
                                 palette);
        y++;
    }
    GFL_BGSysQueueScrLoad(list->setup.layout.bg);
}

// Gives the row of the cursor its palette, and the row that it left its own
// pokebw2's SetCursorRow
static void func_ov067_021b92f0(FrameList *list, u16 newCursor, u16 oldCursor) {
    if (oldCursor < list->rows) {
        func_ov067_021b9280(list, oldCursor, (u32)(list->screens[list->items[list->scroll + oldCursor].screen][0] << 4) >> 16);
    }
    if (newCursor < list->rows && !func_020355c4()) {
        func_0202eb04(list->blinkAnm);
        func_ov067_021b9280(list, newCursor, list->setup.layout.palette);
    }
}

// pokebw2's FlushWindows
static void func_ov067_021b9344(FrameList *list) {
    u32 i;

    func_0201c4e4(list->printQueue);
    for (i = 0; i < list->rowCount; i++) {
        PrintWindow_Flush(&list->printWindows[i], list->printQueue);
    }
}

PrintQueue *func_ov067_021b9394(FrameList *list) {
    return list->printQueue;
}


// pokebw2's FrameList_GetValue
u32 func_ov067_021b9398(FrameList *list, u32 index) {
    return list->items[index].value;
}

// pokebw2's FrameList_GetSelected
int func_ov067_021b93a4(FrameList *list) {
    return list->cursor + list->scroll;
}

// pokebw2's FrameList_GetCursor
s16 func_ov067_021b93b0(FrameList *list) {
    return list->cursor;
}

// pokebw2's FrameList_GetScroll
int func_ov067_021b93b8(FrameList *list) {
    return list->scroll;
}

// pokebw2's FrameList_CanScrollDown
BOOL func_ov067_021b93c0(FrameList *list) {
    if (list->scroll < list->maxScroll) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's FrameList_SetCursor
void func_ov067_021b93d4(FrameList *list, int pos) {
    list->prevCursor = list->cursor;
    list->cursor = pos;
    func_ov067_021b92f0(list, list->cursor, list->prevCursor);
    list->setup.callbacks->select(list->setup.work, list->cursor + list->scroll, TRUE);
}

// pokebw2's FrameList_ShowCursor
void func_ov067_021b940c(FrameList *list) {
    func_ov067_021b92f0(list, list->cursor, 0xff);
}

// pokebw2's FrameList_RedrawCursor
void func_ov067_021b941c(FrameList *list) {
    func_ov067_021b92f0(list, 0xff, list->cursor);
}

