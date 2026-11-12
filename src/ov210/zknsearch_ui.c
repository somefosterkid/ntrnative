// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_ui.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_ui.h"
#include "gfl/bg_sys.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/blink_palanm.h"
#include "system/cursor_move.h"

// The keys and touches of the Pokédex's search screen: the main page's cursor and the buttons around a list. The ROM
// doesn't name this file; zknsearch_ui.c is a guess after zknsearch_main.c

// How long the search's result stays without a key or a touch
#define RESULT_FRAMES 90

static void func_ov210_021c003c(ZknSearchWork *wk, u32 pos);
static void func_ov210_021c006c(void *work, int pos, int prevPos);
static void func_ov210_021c0098(void *work, int pos, int prevPos);
static void func_ov210_021c00b4(void *work, int pos, int prevPos);
static void func_ov210_021c00d8(void *work, int pos, int prevPos);

// pokebw2's sZknSearchCursorCallbacks
static const CursorMoveCallbacks data_ov210_021c0b14 = {
    func_ov210_021c006c,
    func_ov210_021c0098,
    func_ov210_021c00b4,
    func_ov210_021c00d8,
};

// The return button and the rows of the shapes' list
// pokebw2's sZknSearchShapeListTouch
static const TouchRect data_ov210_021c0b24[] = {
    { 168, 191, 232, 255 }, { 0, 39, 128, 143 },    { 40, 79, 128, 143 },
    { 80, 119, 128, 143 },  { 120, 159, 128, 143 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// The return button and the rows of the other lists
// pokebw2's sZknSearchListTouch
static const TouchRect data_ov210_021c0b3c[] = {
    { 168, 191, 232, 255 }, { 0, 23, 128, 143 },    { 24, 47, 128, 143 },
    { 48, 71, 128, 143 },   { 72, 95, 128, 143 },   { 96, 119, 128, 143 },
    { 120, 143, 128, 143 }, { 144, 167, 128, 143 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sZknSearchCursorData
static const CursorMoveData data_ov210_021c0b60[] = {
    { 16,
      0,
      16,
      3,
      ZKNSEARCH_UI_START,
      ZKNSEARCH_UI_INITIAL,
      ZKNSEARCH_UI_ORDER,
      ZKNSEARCH_UI_ORDER,
      { 0, 23, 128, 255 } },
    { 16,
      3,
      16,
      3,
      ZKNSEARCH_UI_ORDER,
      ZKNSEARCH_UI_TYPE,
      ZKNSEARCH_UI_INITIAL,
      ZKNSEARCH_UI_INITIAL,
      { 24, 47, 128, 255 } },
    { 16,
      6,
      16,
      3,
      ZKNSEARCH_UI_INITIAL,
      ZKNSEARCH_UI_COLOR,
      ZKNSEARCH_UI_TYPE,
      ZKNSEARCH_UI_TYPE,
      { 48, 71, 128, 255 } },
    { 16,
      9,
      16,
      3,
      ZKNSEARCH_UI_TYPE,
      ZKNSEARCH_UI_SHAPE,
      ZKNSEARCH_UI_COLOR,
      ZKNSEARCH_UI_COLOR,
      { 72, 95, 128, 255 } },
    { 16,
      12,
      16,
      5,
      ZKNSEARCH_UI_COLOR,
      ZKNSEARCH_UI_START,
      ZKNSEARCH_UI_SHAPE,
      ZKNSEARCH_UI_SHAPE,
      { 96, 135, 128, 255 } },
    { 14,
      18,
      18,
      3,
      ZKNSEARCH_UI_SHAPE,
      ZKNSEARCH_UI_ORDER,
      ZKNSEARCH_UI_RESET,
      ZKNSEARCH_UI_RESET,
      { 144, 167, 113, 255 } },
    { 0,
      18,
      7,
      3,
      ZKNSEARCH_UI_SHAPE,
      ZKNSEARCH_UI_ORDER,
      ZKNSEARCH_UI_START,
      ZKNSEARCH_UI_START,
      { 144, 167, 0, 50 } },
    { 0,
      0,
      0,
      0,
      ZKNSEARCH_UI_SHORTCUT,
      ZKNSEARCH_UI_SHORTCUT,
      ZKNSEARCH_UI_SHORTCUT,
      ZKNSEARCH_UI_SHORTCUT,
      { 168, 191, 184, 207 } },
    { 0, 0, 0, 0, ZKNSEARCH_UI_EXIT, ZKNSEARCH_UI_EXIT, ZKNSEARCH_UI_EXIT, ZKNSEARCH_UI_EXIT, { 168, 191, 208, 231 } },
    { 0,
      0,
      0,
      0,
      ZKNSEARCH_UI_RETURN,
      ZKNSEARCH_UI_RETURN,
      ZKNSEARCH_UI_RETURN,
      ZKNSEARCH_UI_RETURN,
      { 168, 191, 232, 255 } },
    { 0, 0, 0, 0, 0, 0, 0, 0, { TOUCH_RECT_END, 0, 0, 0 } },
};

// pokebw2's ZknSearchUI_InitCursor
void func_ov210_021bfeac(ZknSearchWork *wk, u32 pos) {
    if (func_020355c4() == TRUE) {
        wk->cursorMove =
            func_02025cd0(data_ov210_021c0b60, &data_ov210_021c0b14, wk, FALSE, pos, HEAPID_ZUKAN_SEARCH);
    } else {
        wk->cursorMove =
            func_02025cd0(data_ov210_021c0b60, &data_ov210_021c0b14, wk, TRUE, pos, HEAPID_ZUKAN_SEARCH);
        func_ov210_021bff30(wk, pos, ZKNSEARCH_UI_PALETTE_ON);
    }
    func_02025d1c(wk->cursorMove);
}

// pokebw2's ZknSearchUI_ExitCursor
void func_ov210_021bff18(ZknSearchWork *wk) {
    if (wk->cursorMove != NULL) {
        func_02025d14(wk->cursorMove);
        wk->cursorMove = NULL;
    }
}

// Sets the palette of a position's frame: the criteria and the two buttons under them
// pokebw2's ZknSearchUI_SetCursorPalette
void func_ov210_021bff30(ZknSearchWork *wk, u32 pos, u32 palette) {
    if (pos <= ZKNSEARCH_UI_RESET) {
        const CursorMoveData *data = func_0202616c(wk->cursorMove, pos);

        GFL_BGSysSetScrPaletteNo(0, data->x, data->y, data->width, data->height, palette);
        GFL_BGSysQueueScrLoad(0);
    }
}

// pokebw2's ZknSearchUI_Main
u32 func_ov210_021bff68(ZknSearchWork *wk) {
    u32 ret = func_02025de8(wk->cursorMove);

    if (ret != CURSOR_MOVE_NONE) {
        return ret;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_EXIT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
        if (func_020355c4() == TRUE) {
            func_020260f4(wk->cursorMove, TRUE);
        }
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_SHORTCUT;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        if (func_020355c4() == TRUE) {
            func_020260f4(wk->cursorMove, TRUE);
        }
        func_020355d8(FALSE);
        func_ov210_021c003c(wk, ZKNSEARCH_UI_START);
        return ZKNSEARCH_UI_START;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        if (func_020355c4() == TRUE) {
            func_020260f4(wk->cursorMove, TRUE);
        }
        func_020355d8(FALSE);
        func_ov210_021c003c(wk, ZKNSEARCH_UI_RESET);
        return ZKNSEARCH_UI_RESET;
    }
    // Any other key only shows the cursor again
    if (GCTX_HIDGetPressedKeys() != 0 && func_020355c4() == TRUE) {
        func_020355d8(FALSE);
        func_020260f4(wk->cursorMove, TRUE);
        return CURSOR_MOVE_CURSOR_ON;
    }
    return ret;
}

// pokebw2's ZknSearchUI_MoveCursor
static void func_ov210_021c003c(ZknSearchWork *wk, u32 pos) {
    func_ov210_021bff30(wk, func_020260e0(wk->cursorMove), ZKNSEARCH_UI_PALETTE_OFF);
    func_ov210_021bff30(wk, pos, ZKNSEARCH_UI_PALETTE_ON);
    func_020260e4(wk->cursorMove, pos);
}

// pokebw2's ZknSearchUI_CursorOn
static void func_ov210_021c006c(void *work, int pos, int prevPos) {
    ZknSearchWork *wk = work;
    u32 keys = GCTX_HIDGetPressedKeys();

    // B alone only shows the cursor
    if (!(keys & PAD_BUTTON_B) || (keys & (PAD_BUTTON_A | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN))) {
        func_ov210_021bff30(wk, pos, ZKNSEARCH_UI_PALETTE_ON);
        func_0202eb04(wk->blinkAnm);
    }
}

// pokebw2's ZknSearchUI_CursorOff
static void func_ov210_021c0098(void *work, int pos, int prevPos) {
    ZknSearchWork *wk = work;

    func_ov210_021bff30(wk, prevPos, ZKNSEARCH_UI_PALETTE_OFF);
    func_ov210_021bff30(wk, pos, ZKNSEARCH_UI_PALETTE_OFF);
}

// pokebw2's ZknSearchUI_CursorMove
static void func_ov210_021c00b4(void *work, int pos, int prevPos) {
    ZknSearchWork *wk = work;

    func_ov210_021bff30(wk, prevPos, ZKNSEARCH_UI_PALETTE_OFF);
    func_ov210_021bff30(wk, pos, ZKNSEARCH_UI_PALETTE_ON);
    func_0202eb04(wk->blinkAnm);
}

// The shortcut button doesn't take the cursor
// pokebw2's ZknSearchUI_CursorTouch
static void func_ov210_021c00d8(void *work, int pos, int prevPos) {
    ZknSearchWork *wk = work;

    func_ov210_021bff30(wk, prevPos, ZKNSEARCH_UI_PALETTE_OFF);
    if (pos == ZKNSEARCH_UI_SHORTCUT) {
        func_020260e4(wk->cursorMove, prevPos);
    }
}

// pokebw2's ZknSearchUI_WaitResult
BOOL func_ov210_021c00fc(ZknSearchWork *wk) {
    if (wk->timer == RESULT_FRAMES) {
        wk->timer = 0;
        return TRUE;
    }
    wk->timer++;
    if (func_02035c60()) {
        func_020355d8(TRUE);
        wk->timer = 0;
        return TRUE;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_020355d8(FALSE);
        wk->timer = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZknSearchUI_ListMain
int func_ov210_021c014c(ZknSearchWork *wk) {
    int ret = func_02035c08(data_ov210_021c0b3c);

    if (ret != -1) {
        func_020355d8(TRUE);
        return ret;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_LIST_CANCEL;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_LIST_CANCEL;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        if (func_020355c4() == TRUE) {
            func_020355d8(FALSE);
            func_ov067_021b940c(wk->frameList);
        }
        return ZKNSEARCH_UI_LIST_RESET;
    }
    return ret;
}

// pokebw2's ZknSearchUI_ShapeListMain
int func_ov210_021c01c0(ZknSearchWork *wk) {
    int ret = func_02035c08(data_ov210_021c0b24);

    if (ret != -1) {
        func_020355d8(TRUE);
        return ret;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_LIST_CANCEL;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_START) {
        func_020355d8(FALSE);
        return ZKNSEARCH_UI_LIST_CANCEL;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_SELECT) {
        if (func_020355c4() == TRUE) {
            func_020355d8(FALSE);
            func_ov067_021b940c(wk->frameList);
        }
        return ZKNSEARCH_UI_LIST_RESET;
    }
    return ret;
}
