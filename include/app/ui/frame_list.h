// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/frame_list.h: the lines in use so
// far.
#ifndef BWPORT_APP_UI_FRAME_LIST_H
#define BWPORT_APP_UI_FRAME_LIST_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "system/printsys.h"
#include "struct_decls.h"

// frame_list.c: A list of items that scrolls, with a scroll bar and arrows
typedef struct FrameList FrameList;

// pokebw2's FrameList_ShowCursor
void func_ov067_021b940c(FrameList *list);

// What FrameList_Main returns besides the position of the item chosen
#define FRAMELIST_NONE (-1)

// What an entry of a list's touch table does when it is touched (FrameListTouch's action)
#define FRAMELIST_TOUCH_ITEM 0

#define FRAMELIST_TOUCH_BAR 1
#define FRAMELIST_TOUCH_UP 2
#define FRAMELIST_TOUCH_DOWN 3
#define FRAMELIST_TOUCH_PAGE_UP 4
#define FRAMELIST_TOUCH_PAGE_DOWN 5
#define FRAMELIST_TOUCH_TOP 6
#define FRAMELIST_TOUCH_BOTTOM 7
#define FRAMELIST_TOUCH_ITEM_DECIDE 8

// A rectangle of the list's rows and buttons
struct FrameListTouch {
    TouchRect rect;
    u32 action;
};

// Black 2's; twinned code confirms the offsets of print and select in Black
// What the list calls to print an item into its row's window at y (on the first BG, or on the second), when the
// cursor moves to an item (initial is FALSE only for the first draw), and when the items scroll by delta pixels
typedef struct {
    void (*print)(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
    void (*select)(void *work, u32 index, BOOL moved);
    void (*scroll)(void *work, s16 delta);
} FrameListCallbacks;

// Black 2's; twinned code confirms the offsets of bg, width, rowHeight and barSize in Black
// The first 20 bytes of the setup: where the list is, the size of its rows and the windows they are printed in
typedef struct {
    u8 bg;
    u8 bg2; // the second BG the rows scroll onto, or 0xff
    u8 x;
    s8 y;
    u8 width;
    u8 rowHeight;
    u8 windowX;
    u8 windowY;
    u8 windowWidth;
    u8 windowHeight;
    u8 windowPalette;
    u8 speed[6]; // pixels a step scrolls by, from the fastest
    u8 palette; // of the cursor
    u8 barSize;
    u8 unk13;
} FrameListLayout;

// Black 2's; twinned code confirms the offsets of layout, cursorPos, visibleRows, scroll, touch, callbacks and work in
// Black
typedef struct {
    FrameListLayout layout;
    u16 count; // of items it can hold
    u16 screenCount;
    u8 cursorPos;
    u8 visibleRows;
    u16 scroll;
    const FrameListTouch *touch;
    const FrameListCallbacks *callbacks;
    void *work;
} FrameListSetup;

// A list item: the screen it is drawn from, and its value
typedef struct {
    u32 screen;
    u32 value;
} FrameListItem;

// Black 2's; twinned code confirms the offsets of setup, items, touchRects, blinkAnm, printQueue, printWindows,
// screens, rowCount, firstRow, count, state, nextState, touchY, cursor, rows, prevCursor, scroll, maxScroll, bgOffset,
// scrollDir, scrollStep, scrollFrames, playSE, flicking, barTouch, barIndex, barTop, barBottom, barDragging, barTouchY,
// dragState, dragDelta, flags, dragItem, dragItemPrev, flickStartY, flickEndY, repeatTimer, repeatStage and heapId in
// Black
struct FrameList {
    FrameListSetup setup;
    FrameListItem *items;
    TouchRect *touchRects;
    BlinkPalAnm *blinkAnm;
    PrintQueue *printQueue;
    PrintWindow *printWindows;
    BGWinFrame *frames;
    u16 **screens;
    u8 rowCount;
    u8 rowsPerBG;
    s8 firstRow;
    s8 firstRow2;
    u8 shownRows;
    u8 shownRows2;
    s8 rowOffset;
    u16 count;
    u16 state;
    u16 nextState;
    s32 touchY;
    s32 prevTouchY;
    s16 cursor;
    u16 rows;
    u16 prevCursor;
    s16 scroll;
    u16 maxScroll;
    s8 bgOffset;
    s8 scrollDir;
    u8 scrollStep;
    u8 scrollFrames;
    BOOL playSE;
    BOOL flicking;
    TouchRect barTouch[2];
    u8 barIndex;
    u8 barTop;
    u8 barBottom;
    u8 barDragging;
    s32 barTouchY;
    u8 dragState;
    s8 dragDelta;
    struct {
        u8 touchFrames : 5;
        u8 scrolledByDrag : 1;
        u8 touching : 1;
        u8 dragging : 1;
    } flags;
    u8 dragItem;
    u8 dragItemPrev;
    u8 speedIndex;
    u32 flickStartY;
    u32 flickEndY;
    u8 repeatTimer;
    u8 repeatStage;
    u8 cursorDelay;
    HeapID heapId;
};

// pokebw2's FrameList_AddItem
// Adds an item
void func_ov067_021b79e4(FrameList *list, u32 type, u32 value);
// pokebw2's FrameList_LoadScreen
// Loads the screen of the list's frame, and its palette
void func_ov067_021b7a10(FrameList *list, ArcTool *arc, u32 fileId, BOOL compressed, u32 index);
// pokebw2's FrameList_LoadCursorPalette
void func_ov067_021b7a4c(FrameList *list, ArcTool *arc, u32 fileId, u32 palette, u32 count);
// pokebw2's FrameList_Draw
// Whether the list is still drawing its items
BOOL func_ov067_021b7a64(FrameList *list);
// pokebw2's FrameList_GetBarPos
// Unused: where the scroll bar is, and the cursor off its row
u32 func_ov067_021b8ab8(FrameList *list);
// pokebw2's FrameList_ClampBarPos
// Where the scroll bar goes, from its y
int func_ov067_021b8ae4(FrameList *list, int y);
// pokebw2's FrameList_GetValue
// An item's value
u32 func_ov067_021b9398(FrameList *list, u32 index);
// pokebw2's FrameList_GetSelected
// The item under the cursor, the cursor's row, and the first row shown
int func_ov067_021b93a4(FrameList *list);
// pokebw2's FrameList_GetCursor
s16 func_ov067_021b93b0(FrameList *list);
// pokebw2's FrameList_GetScroll
int func_ov067_021b93b8(FrameList *list);
// pokebw2's FrameList_CanScrollDown
// Whether the list can scroll down
BOOL func_ov067_021b93c0(FrameList *list);
// pokebw2's FrameList_SetCursor
void func_ov067_021b93d4(FrameList *list, int pos);
// pokebw2's FrameList_RedrawCursor
void func_ov067_021b941c(FrameList *list);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's FrameList_Create
FrameList *func_ov067_021b7754(const FrameListSetup *setup, HeapID heapId);
// pokebw2's FrameList_Free
void func_ov067_021b796c(FrameList *list);
// pokebw2's FrameList_Main
u32 func_ov067_021b7ab0(FrameList *list);
// pokebw2's FrameList_SetRowPalette
// Sets the palette of an item's row
void func_ov067_021b9280(FrameList *list, u32 row, u32 palette);
// pokebw2's FrameList_GetPrintQueue
PrintQueue *func_ov067_021b9394(FrameList *list);

#endif // BWPORT_APP_UI_FRAME_LIST_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/frame_list.h: the lines in use so
// far.
#ifndef BWPORT_APP_UI_FRAME_LIST_H

#include "types.h"

// frame_list.c: A list of items that scrolls, with a scroll bar and arrows

// What FrameList_Main returns besides the position of the item chosen

// pokebw2's FrameList_Draw
// Whether the list is still drawing its items
// pokebw2's FrameList_SetCursor
// pokebw2's FrameList_GetCursor

// pokebw2's FrameList_ClampBarPos
// Where the scroll bar goes, from its y
// pokebw2's FrameList_CanScrollDown
// Whether the list can scroll down

#endif // BWPORT_APP_UI_FRAME_LIST_H
