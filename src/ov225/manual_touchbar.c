// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_touchbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual_common.h"
#include "app/manual_graphic.h"
#include "app/manual_touchbar.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/app_menu_common.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/title_7.h"

// The manual's touch bar, named after the ROM's "manual_touchbar.c": the common touch bar's close and return icons,
// a window of text beside them, and the two arrows that turn the explanation's pages, which this file draws and
// reads itself

#define WINDOW_COUNT 2
#define ARROW_COUNT 2
#define ARROW_NONE 0xff
#define TEXT_COLOR PRINT_COLOR(15, 2, 0)

enum {
    WINDOW_UNUSED,
    WINDOW_TEXT,
};

enum {
    STATE_IDLE,
    STATE_DECIDED,
    STATE_ICON_TOUCHED,
    STATE_ARROW_TOUCHED,
};

enum {
    ARROW_STATE_IDLE,
    ARROW_STATE_TOUCHED,
    ARROW_STATE_ANIM,
    ARROW_STATE_DECIDED,
};

// Black 2's; twinned code confirms the offsets of common, bar, mode, pageCount, page, windows, printing, state,
// touched, decided, chars, palette, cellAnims, arrows, arrow and arrowState in Black
struct ManualTouchBar {
    ManualCommon *common;
    TouchBar *bar;
    u32 mode;
    u8 pageCount;
    u8 page;
    BmpWin *windows[WINDOW_COUNT];
    BOOL printing[WINDOW_COUNT];
    u32 state;
    s32 touched;
    s32 decided;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *arrows[ARROW_COUNT];
    u8 arrow;
    u32 arrowState;
    TCB *vblankTask;
};

// An arrow: its actor, the area and the keys that press it, the animation and the sound of a press, and the icon
// it counts as
// Black 2's; twinned code confirms the offsets of left, right, top, bottom and key in Black
typedef struct {
    ClActorSetup setup;
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
    u32 pressedAnim;
    u32 key;
    u32 se;
    u32 icon;
} ManualTouchBarArrow;

static void func_ov225_021ba15c(TCB *tcb, void *data);
static void func_ov225_021ba160(ManualTouchBar *bar);
static void func_ov225_021ba1c0(ManualTouchBar *bar);
static void func_ov225_021ba1c4(ManualTouchBar *bar);
static void func_ov225_021ba218(ManualTouchBar *bar);
static void func_ov225_021ba240(ManualTouchBar *bar);
static void func_ov225_021ba28c(ManualTouchBar *bar);
static void func_ov225_021ba2b0(ManualTouchBar *bar);
static void func_ov225_021ba30c(ManualTouchBar *bar);
static void func_ov225_021ba368(ManualTouchBar *bar);
static void func_ov225_021ba41c(ManualTouchBar *bar);
static void func_ov225_021ba4d8(ManualTouchBar *bar);
static void func_ov225_021ba508(ManualTouchBar *bar);
static void func_ov225_021ba644(ManualTouchBar *bar);

// Where the text starts in each mode but the last, which has none
static const u8 sManualTouchBarTextX[] = { 0, 0, 0, 32 };

// pokebw2's sManualTouchBarWindows
static const ManualWindowSetup data_ov225_021bbf00[WINDOW_COUNT] = {
    { 7, 0, 0, 1, 1, 2, FALSE, 0, 0 },
    { 7, 2, 21, 26, 3, 2, FALSE, 0, 4 },
};

// pokebw2's sManualTouchBarArrows
static const ManualTouchBarArrow data_ov225_021bbf14[ARROW_COUNT] = {
    { { 16, 168, 4, 0, 1 },
      16,
      40,
      168,
      192,
      12,
      PAD_BUTTON_L | PAD_KEY_LEFT,
      SEQ_SE_SELECT1,
      MANUAL_TOUCHBAR_ICON_PREV },
    { { 80, 168, 5, 0, 1 },
      80,
      104,
      168,
      192,
      13,
      PAD_BUTTON_R | PAD_KEY_RIGHT,
      SEQ_SE_SELECT1,
      MANUAL_TOUCHBAR_ICON_NEXT },
};

// pokebw2's ManualTouchBar_Create
ManualTouchBar *func_ov225_021b9f24(ManualCommon *common) {
    ManualTouchBar *bar;

    GFL_OvlLoad(OVERLAY_APP_UI);
    bar = GFL_HeapAllocate(common->heapId, sizeof(ManualTouchBar), TRUE, "manual_touchbar.c", 235);
    bar->common = common;
    func_ov225_021ba160(bar);
    func_ov225_021ba1c0(bar);
    func_ov225_021ba1c4(bar);
    func_ov225_021ba41c(bar);
    bar->mode = MANUAL_TOUCHBAR_MODE_NONE;
    bar->pageCount = 0;
    bar->page = 0;
    func_ov225_021ba068(bar, MANUAL_TOUCHBAR_MODE_TOP);
    func_ov225_021ba644(bar);
    bar->vblankTask = GFL_VBlankTCBAdd(func_ov225_021ba15c, bar, 1);
    return bar;
}

// pokebw2's ManualTouchBar_Free
void func_ov225_021b9f8c(ManualTouchBar *bar) {
    GFL_TCBRemove(bar->vblankTask);
    func_ov225_021ba4d8(bar);
    func_ov225_021ba218(bar);
    func_ov067_021b639c(bar->bar);
    GFL_HeapFree(bar);
    GFL_OvlUnload(OVERLAY_APP_UI);
}

// pokebw2's ManualTouchBar_Main
void func_ov225_021b9fbc(ManualTouchBar *bar) {
    bar->touched = -1;
    bar->decided = -1;
    func_ov225_021ba240(bar);
    if (bar->state == STATE_DECIDED) {
        bar->state = STATE_IDLE;
        return;
    }
    if (bar->state == STATE_IDLE || bar->state == STATE_ICON_TOUCHED) {
        func_ov067_021b63d0(bar->bar);
        if (bar->state == STATE_IDLE) {
            u32 icon = func_ov067_021b6470(bar->bar);

            if (icon != -1) {
                bar->state = STATE_ICON_TOUCHED;
                bar->touched = icon;
            }
        } else {
            u32 icon = func_ov067_021b6448(bar->bar);

            if (icon != -1) {
                bar->state = STATE_DECIDED;
                bar->decided = icon;
            }
        }
    }
    if (bar->state == STATE_IDLE || bar->state == STATE_ARROW_TOUCHED) {
        func_ov225_021ba508(bar);
        if (bar->state == STATE_IDLE) {
            if (bar->arrowState == ARROW_STATE_TOUCHED) {
                bar->state = STATE_ARROW_TOUCHED;
                bar->touched = data_ov225_021bbf14[bar->arrow].icon;
            }
        } else {
            if (bar->arrowState == ARROW_STATE_DECIDED) {
                bar->state = STATE_DECIDED;
                bar->decided = data_ov225_021bbf14[bar->arrow].icon;
            }
        }
    }
}

// pokebw2's ManualTouchBar_SetMode
void func_ov225_021ba068(ManualTouchBar *bar, u32 mode) {
    if (bar->mode != mode) {
        switch (bar->mode) {
        case MANUAL_TOUCHBAR_MODE_TOP:
            func_ov067_021b6558(bar->bar, MANUAL_TOUCHBAR_ICON_CLOSE, FALSE);
            break;
        case MANUAL_TOUCHBAR_MODE_CATEGORY:
            break;
        case MANUAL_TOUCHBAR_MODE_TITLE:
            break;
        case MANUAL_TOUCHBAR_MODE_EXPLAIN:
            func_0204b3dc(bar->arrows[0], FALSE);
            func_0204b3dc(bar->arrows[1], FALSE);
            break;
        case MANUAL_TOUCHBAR_MODE_NONE:
            break;
        }
        bar->mode = mode;
        func_ov225_021ba644(bar);
        switch (bar->mode) {
        case MANUAL_TOUCHBAR_MODE_TOP:
            func_ov067_021b6558(bar->bar, MANUAL_TOUCHBAR_ICON_CLOSE, TRUE);
            func_ov225_021ba28c(bar);
            break;
        case MANUAL_TOUCHBAR_MODE_CATEGORY:
            func_ov225_021ba2b0(bar);
            break;
        case MANUAL_TOUCHBAR_MODE_TITLE:
            func_ov225_021ba30c(bar);
            break;
        case MANUAL_TOUCHBAR_MODE_EXPLAIN:
            break;
        case MANUAL_TOUCHBAR_MODE_NONE:
            break;
        }
    }
}

// pokebw2's ManualTouchBar_SetPage
void func_ov225_021ba0f0(ManualTouchBar *bar, u8 pageCount, u8 page) {
    bar->pageCount = pageCount;
    bar->page = page;
    if (bar->mode == MANUAL_TOUCHBAR_MODE_EXPLAIN) {
        if (bar->pageCount <= 1) {
            func_0204b3dc(bar->arrows[0], FALSE);
            func_0204b3dc(bar->arrows[1], FALSE);
            func_ov225_021ba28c(bar);
        } else {
            if (bar->page == 0) {
                func_0204b3dc(bar->arrows[0], FALSE);
                func_0204b3dc(bar->arrows[1], TRUE);
            } else if (bar->page == pageCount - 1) {
                func_0204b3dc(bar->arrows[0], TRUE);
                func_0204b3dc(bar->arrows[1], FALSE);
            } else {
                func_0204b3dc(bar->arrows[0], TRUE);
                func_0204b3dc(bar->arrows[1], TRUE);
            }
            func_ov225_021ba368(bar);
        }
    }
}

// pokebw2's ManualTouchBar_GetDecided
u32 func_ov225_021ba154(ManualTouchBar *bar) {
    return bar->decided;
}

// pokebw2's ManualTouchBar_GetTouched
u32 func_ov225_021ba158(ManualTouchBar *bar) {
    return bar->touched;
}

// pokebw2's ManualTouchBar_VBlank
static void func_ov225_021ba15c(TCB *tcb, void *data) {
}

// pokebw2's ManualTouchBar_CreateBar
static void func_ov225_021ba160(ManualTouchBar *bar) {
    TouchBarSetup setup;
    TouchBarItem items[] = {
        { MANUAL_TOUCHBAR_ICON_RETURN, { 232, 168 } },
        { MANUAL_TOUCHBAR_ICON_CLOSE, { 200, 168 } },
    };

    MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
    setup.items = items;
    setup.count = NELEMS(items);
    setup.unit = func_ov225_021b9a08(bar->common->graphic);
    setup.bg = 6;
    setup.bgPalette = 3;
    setup.objPalette = 1;
    setup.vramType = CLACT_VRAM_MAIN;
    bar->bar = func_ov067_021b62e0(&setup, bar->common->heapId);
    func_ov067_021b6558(bar->bar, MANUAL_TOUCHBAR_ICON_CLOSE, FALSE);
}

// pokebw2's ManualTouchBar_InitBG
static void func_ov225_021ba1c0(ManualTouchBar *bar) {
}

// pokebw2's ManualTouchBar_CreateWindows
static void func_ov225_021ba1c4(ManualTouchBar *bar) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        const ManualWindowSetup *setup = &data_ov225_021bbf00[i];

        bar->windows[i] = BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height,
                                               setup->palette, setup->fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(bar->windows[i]), 0);
        BmpWin_FlushChar(bar->windows[i]);
        bar->printing[i] = FALSE;
    }
}

// pokebw2's ManualTouchBar_FreeWindows
static void func_ov225_021ba218(ManualTouchBar *bar) {
    u8 i;

    func_0201c6ec(bar->common->touchBarQueue);
    for (i = 0; i < WINDOW_COUNT; i++) {
        bar->printing[i] = FALSE;
        BmpWin_Free(bar->windows[i]);
    }
}

// Shows the windows whose text the queue has printed
// pokebw2's ManualTouchBar_UpdateWindows
static void func_ov225_021ba240(ManualTouchBar *bar) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        if (bar->printing[i]) {
            if (!func_0201c6c4(bar->common->touchBarQueue, BmpWin_GetBitmap(bar->windows[i]))) {
                BmpWin *window = bar->windows[i];

                BmpWin_FlushChar(window);
                BmpWin_FlushMap(window);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
                bar->printing[i] = FALSE;
            }
        }
    }
}

// pokebw2's ManualTouchBar_ClearText
static void func_ov225_021ba28c(ManualTouchBar *bar) {
    func_0201c6ec(bar->common->touchBarQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]), 0);
    BmpWin_FlushChar(bar->windows[WINDOW_TEXT]);
    bar->printing[WINDOW_TEXT] = FALSE;
}

// pokebw2's ManualTouchBar_PrintCategoryPrompt
static void func_ov225_021ba2b0(ManualTouchBar *bar) {
    StrBuf *str;

    func_0201c6ec(bar->common->touchBarQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]), 0);
    str = GFL_MsgDataLoadStrbufNew(bar->common->msgSystem, Title7_Text_002);
    func_0201c724(bar->common->touchBarQueue, BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]),
                  sManualTouchBarTextX[MANUAL_TOUCHBAR_MODE_CATEGORY], 4, str, bar->common->font, TEXT_COLOR);
    GFL_StrBufFree(str);
    bar->printing[WINDOW_TEXT] = TRUE;
    func_ov225_021ba240(bar);
}

// pokebw2's ManualTouchBar_PrintTitlePrompt
static void func_ov225_021ba30c(ManualTouchBar *bar) {
    StrBuf *str;

    func_0201c6ec(bar->common->touchBarQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]), 0);
    str = GFL_MsgDataLoadStrbufNew(bar->common->msgSystem, Title7_Text_003);
    func_0201c724(bar->common->touchBarQueue, BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]),
                  sManualTouchBarTextX[MANUAL_TOUCHBAR_MODE_TITLE], 4, str, bar->common->font, TEXT_COLOR);
    GFL_StrBufFree(str);
    bar->printing[WINDOW_TEXT] = TRUE;
    func_ov225_021ba240(bar);
}

// pokebw2's ManualTouchBar_PrintPage
static void func_ov225_021ba368(ManualTouchBar *bar) {
    WordSet *wordSet;
    StrBuf *format;
    StrBuf *str;

    func_0201c6ec(bar->common->touchBarQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]), 0);
    wordSet = GFL_WordSetSystemCreateDefault(bar->common->heapId);
    format = GFL_MsgDataLoadStrbufNew(bar->common->msgSystem, Title7_Text_004);
    str = GFL_StrBufCreate(16, bar->common->heapId);
    WordSetNumber(wordSet, 0, bar->page + 1, 1, 0, TRUE);
    WordSetNumber(wordSet, 1, bar->pageCount, 1, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, str, format);
    func_0201c724(bar->common->touchBarQueue, BmpWin_GetBitmap(bar->windows[WINDOW_TEXT]),
                  sManualTouchBarTextX[MANUAL_TOUCHBAR_MODE_EXPLAIN], 4, str, bar->common->font, TEXT_COLOR);
    GFL_StrBufFree(str);
    GFL_StrBufFree(format);
    GFL_WordSetSystemFree(wordSet);
    bar->printing[WINDOW_TEXT] = TRUE;
    func_ov225_021ba240(bar);
}

// pokebw2's ManualTouchBar_CreateArrows
static void func_ov225_021ba41c(ManualTouchBar *bar) {
    u8 i;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), bar->common->heapId);

    bar->palette = func_0204ac18(arc, func_02027614(), CLACT_VRAM_SUB, 4 * 0x20, 0, 3, bar->common->heapId);
    bar->chars = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_SUB, bar->common->heapId);
    bar->cellAnims = func_0204af28(arc, func_0202761c(0), func_02027620(0), bar->common->heapId);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ARROW_COUNT; i++) {
        bar->arrows[i] = func_0204b294(func_ov225_021b9a08(bar->common->graphic), bar->chars, bar->palette,
                                       bar->cellAnims, &data_ov225_021bbf14[i].setup, 1, bar->common->heapId);
        func_0204b3dc(bar->arrows[i], FALSE);
        func_0204b92c(bar->arrows[i], TRUE);
    }
}

// pokebw2's ManualTouchBar_FreeArrows
static void func_ov225_021ba4d8(ManualTouchBar *bar) {
    u8 i;

    for (i = 0; i < ARROW_COUNT; i++) {
        func_0204b3b4(bar->arrows[i]);
    }
    func_0204afd8(bar->cellAnims);
    func_0204a8d4(bar->chars);
    func_0204ada4(bar->palette);
}

// Reads the arrows: a press by key or touch plays the arrow's animation, and the arrow is decided once it ends
// pokebw2's ManualTouchBar_UpdateArrows
static void func_ov225_021ba508(ManualTouchBar *bar) {
    switch (bar->arrowState) {
    case ARROW_STATE_IDLE: {
        BOOL pressed = FALSE;
        u8 i;

        for (i = 0; i < ARROW_COUNT; i++) {
            if (func_0204b3f4(bar->arrows[i])) {
                const ManualTouchBarArrow *arrow = &data_ov225_021bbf14[i];
                u32 x;
                u32 y;

                if (GCTX_HIDGetTypedKeys() & arrow->key) {
                    bar->arrow = i;
                    func_020355d8(FALSE);
                    pressed = TRUE;
                    break;
                }
                if (func_02035d20(&x, &y)) {
                    if (arrow->left <= x && x <= arrow->right && arrow->top <= y && y <= arrow->bottom) {
                        bar->arrow = i;
                        pressed = TRUE;
                        func_020355d8(TRUE);
                        break;
                    }
                }
            }
        }
        if (pressed) {
            func_0204b878(bar->arrows[bar->arrow], data_ov225_021bbf14[bar->arrow].pressedAnim);
            if (data_ov225_021bbf14[bar->arrow].se != 0) {
                GFL_SndSEPlay(data_ov225_021bbf14[bar->arrow].se);
            }
            bar->arrowState = ARROW_STATE_TOUCHED;
        }
        break;
    }
    case ARROW_STATE_TOUCHED:
        bar->arrowState = ARROW_STATE_ANIM;
        break;
    case ARROW_STATE_ANIM:
        if (!func_0204b98c(bar->arrows[bar->arrow])) {
            func_0204b878(bar->arrows[bar->arrow], data_ov225_021bbf14[bar->arrow].setup.sequence);
            bar->arrowState = ARROW_STATE_DECIDED;
        }
        break;
    case ARROW_STATE_DECIDED:
        bar->arrowState = ARROW_STATE_IDLE;
        bar->arrow = ARROW_NONE;
        break;
    }
}

// pokebw2's ManualTouchBar_Reset
static void func_ov225_021ba644(ManualTouchBar *bar) {
    bar->state = STATE_IDLE;
    bar->touched = -1;
    bar->decided = -1;
    bar->arrow = ARROW_NONE;
    bar->arrowState = ARROW_STATE_IDLE;
}
