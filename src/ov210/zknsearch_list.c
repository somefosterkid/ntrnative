// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zknsearch_bmp.h"
#include "app/zknsearch_list.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_obj.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "system/printsys.h"
#include "text/system/0479.h"
#include "app/zukansearch.h"

// The lists of the criteria of the Pokédex's search screen: the order, the initial, the type, the color and the shape.
// The ROM doesn't name this file; zknsearch_list.c is a guess after zknsearch_main.c

#define INITIAL_COUNT 26
#define COLOR_COUNT 10
#define ORDER_COUNT 6

void func_ov210_021c0584(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov210_021c05b4(void *work, u32 index, BOOL moved);
static void func_ov210_021c05b8(void *work, s16 delta);
static void func_ov210_021c05bc(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov210_021c05ec(void *work, u32 index, BOOL moved);
static void func_ov210_021c05f0(void *work, s16 delta);
static void func_ov210_021c0608(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov210_021c065c(void *work, u32 index, BOOL moved);
static void func_ov210_021c0660(void *work, s16 delta);
static void func_ov210_021c0678(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov210_021c06a8(void *work, u32 index, BOOL moved);
static void func_ov210_021c06ac(void *work, s16 delta);
static void func_ov210_021c06c4(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG);
static void func_ov210_021c06f8(void *work, u32 index, BOOL moved);
static void func_ov210_021c06fc(void *work, s16 delta);

// pokebw2's sZknSearchOrderTouch
static const FrameListTouch data_ov210_021c0ce8[] = {
    { { 0, 23, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },   { { 24, 47, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },
    { { 48, 71, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },  { { 72, 95, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },
    { { 96, 119, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE }, { { 120, 143, 152, 231 }, FRAMELIST_TOUCH_ITEM_DECIDE },
    { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// pokebw2's sZknSearchOrderCallbacks
static const FrameListCallbacks data_ov210_021c0c14 = {
    func_ov210_021c0584,
    func_ov210_021c05b4,
    func_ov210_021c05b8,
};

// pokebw2's sZknSearchOrderSetup
static const FrameListSetup data_ov210_021c0c20 = {
    { 1, 5, 16, 0, 16, 3, 3, 0, 11, 3, 1, { 24, 12, 8, 6, 4, 3 }, 3, 8, 0 },
    ORDER_COUNT,
    1,
    0,
    6,
    0,
    data_ov210_021c0ce8,
    &data_ov210_021c0c14,
    NULL,
};

// pokebw2's sZknSearchInitialTouch
static const FrameListTouch data_ov210_021c0d60[] = {
    { { 0, 23, 152, 231 }, FRAMELIST_TOUCH_ITEM },         { { 24, 47, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 48, 71, 152, 231 }, FRAMELIST_TOUCH_ITEM },        { { 72, 95, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 96, 119, 152, 231 }, FRAMELIST_TOUCH_ITEM },       { { 120, 143, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 144, 167, 152, 231 }, FRAMELIST_TOUCH_ITEM },      { { 8, 160, 232, 255 }, FRAMELIST_TOUCH_BAR },
    { { 168, 191, 184, 207 }, FRAMELIST_TOUCH_PAGE_UP },   { { 168, 191, 208, 231 }, FRAMELIST_TOUCH_PAGE_DOWN },
    { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// pokebw2's sZknSearchInitialCallbacks
static const FrameListCallbacks data_ov210_021c0bfc = {
    func_ov210_021c05bc,
    func_ov210_021c05ec,
    func_ov210_021c05f0,
};

// pokebw2's sZknSearchInitialSetup
static const FrameListSetup data_ov210_021c0c70 = {
    { 1, 5, 16, 0, 16, 3, 3, 0, 11, 3, 1, { 24, 12, 8, 6, 4, 3 }, 3, 8, 0 },
    ZKNSEARCH_ITEM_NAME_COUNT,
    1,
    0,
    7,
    0,
    data_ov210_021c0d60,
    &data_ov210_021c0bfc,
    NULL,
};

// pokebw2's sZknSearchTypeTouch
static const FrameListTouch data_ov210_021c0e10[] = {
    { { 0, 23, 152, 231 }, FRAMELIST_TOUCH_ITEM },         { { 24, 47, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 48, 71, 152, 231 }, FRAMELIST_TOUCH_ITEM },        { { 72, 95, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 96, 119, 152, 231 }, FRAMELIST_TOUCH_ITEM },       { { 120, 143, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 144, 167, 152, 231 }, FRAMELIST_TOUCH_ITEM },      { { 8, 160, 232, 255 }, FRAMELIST_TOUCH_BAR },
    { { 168, 191, 184, 207 }, FRAMELIST_TOUCH_PAGE_UP },   { { 168, 191, 208, 231 }, FRAMELIST_TOUCH_PAGE_DOWN },
    { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// pokebw2's sZknSearchTypeCallbacks
static const FrameListCallbacks data_ov210_021c0bf0 = {
    func_ov210_021c0608,
    func_ov210_021c065c,
    func_ov210_021c0660,
};

// pokebw2's sZknSearchTypeSetup
static const FrameListSetup data_ov210_021c0c48 = {
    { 1, 5, 16, 0, 16, 3, 3, 0, 11, 3, 1, { 24, 12, 8, 6, 4, 3 }, 3, 8, 0 },
    ZKNSEARCH_ITEM_NAME_COUNT,
    1,
    0,
    7,
    0,
    data_ov210_021c0e10,
    &data_ov210_021c0bf0,
    NULL,
};

// pokebw2's sZknSearchColorTouch
static const FrameListTouch data_ov210_021c0db8[] = {
    { { 0, 23, 152, 231 }, FRAMELIST_TOUCH_ITEM },         { { 24, 47, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 48, 71, 152, 231 }, FRAMELIST_TOUCH_ITEM },        { { 72, 95, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 96, 119, 152, 231 }, FRAMELIST_TOUCH_ITEM },       { { 120, 143, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 144, 167, 152, 231 }, FRAMELIST_TOUCH_ITEM },      { { 8, 160, 232, 255 }, FRAMELIST_TOUCH_BAR },
    { { 168, 191, 184, 207 }, FRAMELIST_TOUCH_PAGE_UP },   { { 168, 191, 208, 231 }, FRAMELIST_TOUCH_PAGE_DOWN },
    { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// pokebw2's sZknSearchColorCallbacks
static const FrameListCallbacks data_ov210_021c0c08 = {
    func_ov210_021c0678,
    func_ov210_021c06a8,
    func_ov210_021c06ac,
};

// pokebw2's sZknSearchColorSetup
static const FrameListSetup data_ov210_021c0c98 = {
    { 1, 5, 16, 0, 16, 3, 3, 0, 11, 3, 1, { 24, 12, 8, 6, 4, 3 }, 3, 8, 0 },
    ZKNSEARCH_ITEM_NAME_COUNT,
    1,
    0,
    7,
    0,
    data_ov210_021c0db8,
    &data_ov210_021c0c08,
    NULL,
};

// pokebw2's sZknSearchShapeTouch
static const FrameListTouch data_ov210_021c0d20[] = {
    { { 0, 39, 152, 231 }, FRAMELIST_TOUCH_ITEM },         { { 40, 79, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 80, 119, 152, 231 }, FRAMELIST_TOUCH_ITEM },       { { 120, 159, 152, 231 }, FRAMELIST_TOUCH_ITEM },
    { { 8, 160, 232, 255 }, FRAMELIST_TOUCH_BAR },         { { 168, 191, 184, 207 }, FRAMELIST_TOUCH_PAGE_UP },
    { { 168, 191, 208, 231 }, FRAMELIST_TOUCH_PAGE_DOWN }, { { TOUCH_RECT_END, 0, 0, 0 }, FRAMELIST_TOUCH_ITEM },
};

// pokebw2's sZknSearchShapeCallbacks
static const FrameListCallbacks data_ov210_021c0be4 = {
    func_ov210_021c06c4,
    func_ov210_021c06f8,
    func_ov210_021c06fc,
};

// pokebw2's sZknSearchShapeSetup
static const FrameListSetup data_ov210_021c0cc0 = {
    { 1, 5, 16, 0, 16, 5, 4, 1, 1, 1, 1, { 40, 20, 10, 8, 5, 4 }, 3, 8, 0 },
    ZKNSEARCH_SHAPE_COUNT,
    1,
    0,
    4,
    0,
    data_ov210_021c0d20,
    &data_ov210_021c0be4,
    NULL,
};

// pokebw2's ZknSearchList_InitOrder
void func_ov210_021c0234(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    FrameListSetup setup = data_ov210_021c0c20;
    u32 i;

    setup.cursorPos = 0;
    setup.scroll = 0;
    setup.work = wk;
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_SEARCH);
    wk->listRows = setup.visibleRows;
    func_ov067_021b7a10(wk->frameList, arc, 97, FALSE, 0);
    func_ov067_021b7a4c(wk->frameList, arc, 73, 2, 3);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ORDER_COUNT; i++) {
        func_ov067_021b79e4(wk->frameList, 0, i);
        wk->itemNames[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0479_Text_025 + i);
    }
}

// pokebw2's ZknSearchList_InitInitial
void func_ov210_021c02dc(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    FrameListSetup setup = data_ov210_021c0c70;
    u32 i;

    setup.cursorPos = 0;
    setup.scroll = 0;
    setup.work = wk;
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_SEARCH);
    wk->listRows = setup.visibleRows;
    func_ov067_021b7a10(wk->frameList, arc, 97, FALSE, 0);
    func_ov067_021b7a4c(wk->frameList, arc, 73, 2, 3);
    GFL_ArcToolFree(arc);
    for (i = 0; i < INITIAL_COUNT; i++) {
        func_ov067_021b79e4(wk->frameList, 0, i);
        wk->itemNames[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, Bank0479_Text_051 + i);
    }
}

// pokebw2's ZknSearchList_InitType
void func_ov210_021c0384(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    FrameListSetup setup = data_ov210_021c0c48;
    u32 i;

    setup.cursorPos = 0;
    setup.scroll = 0;
    setup.work = wk;
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_SEARCH);
    wk->listRows = setup.visibleRows;
    func_ov067_021b7a10(wk->frameList, arc, 97, FALSE, 0);
    func_ov067_021b7a4c(wk->frameList, arc, 73, 2, 3);
    GFL_ArcToolFree(arc);
    for (i = 0; i <= ZKNSEARCH_TYPE_COUNT - 1; i++) {
        func_ov067_021b79e4(wk->frameList, 0, i);
        wk->itemNames[i] = GFL_MsgDataLoadStrbufNew(wk->msgData, func_ov210_021bcef4(wk, i));
    }
}

// pokebw2's ZknSearchList_InitColor
void func_ov210_021c0434(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    FrameListSetup setup = data_ov210_021c0c98;
    u32 i;

    setup.cursorPos = 0;
    setup.scroll = 0;
    setup.work = wk;
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_SEARCH);
    wk->listRows = setup.visibleRows;
    func_ov067_021b7a10(wk->frameList, arc, 97, FALSE, 0);
    func_ov067_021b7a4c(wk->frameList, arc, 73, 2, 3);
    GFL_ArcToolFree(arc);
    for (i = Bank0479_Text_096; i <= Bank0479_Text_105; i++) {
        func_ov067_021b79e4(wk->frameList, 0, i - Bank0479_Text_096);
        wk->itemNames[i - Bank0479_Text_096] = GFL_MsgDataLoadStrbufNew(wk->msgData, i);
    }
}

// pokebw2's ZknSearchList_InitShape
void func_ov210_021c04cc(ZknSearchWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
    FrameListSetup setup = data_ov210_021c0cc0;
    u32 i;

    setup.cursorPos = 0;
    setup.scroll = 0;
    setup.work = wk;
    wk->frameList = func_ov067_021b7754(&setup, HEAPID_ZUKAN_SEARCH);
    wk->listRows = setup.visibleRows;
    func_ov067_021b7a10(wk->frameList, arc, 98, FALSE, 0);
    func_ov067_021b7a4c(wk->frameList, arc, 73, 2, 3);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ZKNSEARCH_SHAPE_COUNT; i++) {
        func_ov067_021b79e4(wk->frameList, 0, i);
    }
}

// pokebw2's ZknSearchList_Exit
void func_ov210_021c0550(ZknSearchWork *wk) {
    u32 i;

    func_ov067_021b796c(wk->frameList);
    wk->frameList = NULL;
    for (i = 0; i < ZKNSEARCH_ITEM_NAME_COUNT; i++) {
        if (wk->itemNames[i] != NULL) {
            GFL_StrBufFree(wk->itemNames[i]);
            wk->itemNames[i] = NULL;
        }
    }
}

// pokebw2's ZknSearchList_OrderPrint
void func_ov210_021c0584(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknSearchWork *wk = work;

    func_ov210_021bec60(wk, window, wk->itemNames[index]);
    if (wk->param->criteria->order == index) {
        func_ov210_021bf7b0(wk, 0, y, firstBG);
    }
}

// pokebw2's ZknSearchList_OrderSelect
static void func_ov210_021c05b4(void *work, u32 index, BOOL moved) {
}

// pokebw2's ZknSearchList_OrderScroll
static void func_ov210_021c05b8(void *work, s16 delta) {
}

// pokebw2's ZknSearchList_InitialPrint
static void func_ov210_021c05bc(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknSearchWork *wk = work;

    func_ov210_021bec60(wk, window, wk->itemNames[index]);
    if (wk->param->criteria->initial == index) {
        func_ov210_021bf7b0(wk, 0, y, firstBG);
    }
}

// pokebw2's ZknSearchList_InitialSelect
static void func_ov210_021c05ec(void *work, u32 index, BOOL moved) {
}

// pokebw2's ZknSearchList_InitialScroll
static void func_ov210_021c05f0(void *work, s16 delta) {
    ZknSearchWork *wk = work;

    func_ov210_021bfbf8(wk, -delta);
    func_ov210_021bf728(wk, TRUE);
}

// pokebw2's ZknSearchList_TypePrint
static void func_ov210_021c0608(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknSearchWork *wk = work;

    func_ov210_021bec60(wk, window, wk->itemNames[index]);
    if (index == func_ov210_021bcef0(wk, wk->param->criteria->type1)) {
        func_ov210_021bf7b0(wk, 0, y, firstBG);
    }
    if (index == func_ov210_021bcef0(wk, wk->param->criteria->type2)) {
        func_ov210_021bf7b0(wk, 1, y, firstBG);
    }
}

// pokebw2's ZknSearchList_TypeSelect
static void func_ov210_021c065c(void *work, u32 index, BOOL moved) {
}

// pokebw2's ZknSearchList_TypeScroll
static void func_ov210_021c0660(void *work, s16 delta) {
    ZknSearchWork *wk = work;

    func_ov210_021bfbf8(wk, -delta);
    func_ov210_021bf728(wk, TRUE);
}

// pokebw2's ZknSearchList_ColorPrint
static void func_ov210_021c0678(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknSearchWork *wk = work;

    func_ov210_021bec60(wk, window, wk->itemNames[index]);
    if (wk->param->criteria->color == index) {
        func_ov210_021bf7b0(wk, 0, y, firstBG);
    }
}

// pokebw2's ZknSearchList_ColorSelect
static void func_ov210_021c06a8(void *work, u32 index, BOOL moved) {
}

// pokebw2's ZknSearchList_ColorScroll
static void func_ov210_021c06ac(void *work, s16 delta) {
    ZknSearchWork *wk = work;

    func_ov210_021bfbf8(wk, -delta);
    func_ov210_021bf728(wk, TRUE);
}

// pokebw2's ZknSearchList_ShapePrint
static void func_ov210_021c06c4(void *work, u32 index, PrintWindow *window, s16 y, BOOL firstBG) {
    ZknSearchWork *wk = work;

    func_ov210_021becac(wk, window);
    if (wk->param->criteria->shape == index) {
        func_ov210_021bfa14(wk, y, firstBG);
    }
    func_ov210_021bfb8c(wk, index, y, firstBG);
}

// pokebw2's ZknSearchList_ShapeSelect
static void func_ov210_021c06f8(void *work, u32 index, BOOL moved) {
}

// pokebw2's ZknSearchList_ShapeScroll
static void func_ov210_021c06fc(void *work, s16 delta) {
    ZknSearchWork *wk = work;

    func_ov210_021bfce4(wk, -delta);
    func_ov210_021bf728(wk, TRUE);
}
