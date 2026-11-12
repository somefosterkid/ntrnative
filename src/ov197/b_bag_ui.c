// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_ui.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_bag_ui.h"
#include "battle/b_app_tool.h"
#include "battle/b_bag_main.h"
#include "system/cursor_move.h"

// The battle bag's key cursor: a CursorMove on the page's table of positions, whose callbacks move and show overlay
// 285's cursor. The file name is our guess, after the ROM's b_bag_main.c and Diamond and Pearl's b_bag_* files.

static void func_ov197_0220d974(BBagWork *work, BOOL visible);
static void func_ov197_0220d98c(BBagWork *work, int pos);
void func_ov197_0220d9a8(void *work, int pos, int prevPos);
void func_ov197_0220d9bc(void *work, int pos, int prevPos);
void func_ov197_0220d9c8(void *work, int pos, int prevPos);
void func_ov197_0220d9d4(void *work, int pos, int prevPos);
void func_ov197_0220d9dc(void *work, int pos, int prevPos);

// The pockets
static const CursorMoveData data_ov197_0220dffc[7] = {
#include "asset/b_bag_ui_data_ov197_0220dffc.inc"
};

// A pocket's items; 7 and 8, the page arrows, are touch only
static const CursorMoveData data_ov197_0220e050[10] = {
#include "asset/b_bag_ui_data_ov197_0220e050.inc"
};

// One item
static const CursorMoveData data_ov197_0220dfd8[3] = {
#include "asset/b_bag_ui_data_ov197_0220dfd8.inc"
};

static const CursorMoveCallbacks data_ov197_0220dfb8 = {
    func_ov197_0220d9dc,
    func_ov197_0220d9bc,
    func_ov197_0220d9d4,
    func_ov197_0220d9c8,
};

static const CursorMoveCallbacks data_ov197_0220dfc8 = {
    func_ov197_0220d9a8,
    func_ov197_0220d9bc,
    func_ov197_0220d9d4,
    func_ov197_0220d9c8,
};

static const CursorMoveCallbacks data_ov197_0220dfa8 = {
    func_ov197_0220d9a8,
    func_ov197_0220d9bc,
    func_ov197_0220d9d4,
    func_ov197_0220d9c8,
};

static const CursorMoveData *const data_ov197_0220df9c[3] = {
    data_ov197_0220dffc,
    data_ov197_0220e050,
    data_ov197_0220dfd8,
};

static const CursorMoveCallbacks *const data_ov197_0220df90[3] = {
    &data_ov197_0220dfc8,
    &data_ov197_0220dfb8,
    &data_ov197_0220dfa8,
};

// pokebw2's BBagUi_CreateCursor
void func_ov197_0220d8c4(BBagWork *work, u8 page, int pos) {
    work->cursorMove = func_02025cd0(data_ov197_0220df9c[page], data_ov197_0220df90[page], work,
                                         work->cursorVisible, pos, work->param->heapId);
    func_02025d1c(work->cursorMove);
    func_02025d24(work->cursorMove);
    func_ov197_0220d98c(work, pos);
}

// pokebw2's BBagUi_DeleteCursor
void func_ov197_0220d914(BBagWork *work) {
    func_02025d14(work->cursorMove);
}

// pokebw2's BBagUi_ChangeCursorPage
void func_ov197_0220d924(BBagWork *work, u8 page, int pos) {
    if (work->cursorVisible == FALSE) {
        pos = 0;
    } else {
        switch (page) {
        case 0:
            pos = work->pocket;
            break;
        case 1:
            pos = work->param->rows[work->pocket];
            break;
        case 2:
            pos = 0;
            break;
        }
    }
    func_ov197_0220d914(work);
    func_ov197_0220d8c4(work, page, pos);
}

// pokebw2's BBagUi_SetCursorVisible
static void func_ov197_0220d974(BBagWork *work, BOOL visible) {
    work->cursorVisible = visible;
    func_ov196_0220afdc(work->cursor, visible);
}

// pokebw2's BBagUi_PutCursor
static void func_ov197_0220d98c(BBagWork *work, int pos) {
    func_ov196_0220b000(work->cursor, func_0202616c(work->cursorMove, pos));
}

// pokebw2's BBagUi_CallbackCursorOn
void func_ov197_0220d9a8(void *work, int pos, int prevPos) {
    func_ov197_0220d98c(work, pos);
    func_ov197_0220d974(work, TRUE);
}

// pokebw2's BBagUi_CallbackCursorOff
void func_ov197_0220d9bc(void *work, int pos, int prevPos) {
    func_ov197_0220d974(work, FALSE);
}

// pokebw2's BBagUi_CallbackTouch
void func_ov197_0220d9c8(void *work, int pos, int prevPos) {
    func_ov197_0220d974(work, FALSE);
}

// pokebw2's BBagUi_CallbackMove
void func_ov197_0220d9d4(void *work, int pos, int prevPos) {
    func_ov197_0220d98c(work, pos);
}

// On the items page, the cursor shown on a page arrow goes to the first item
// pokebw2's BBagUi_CallbackItemsCursorOn
void func_ov197_0220d9dc(void *wk, int pos, int prevPos) {
    BBagWork *work = wk;

    if (pos == 7 || pos == 8) {
        func_020260e4(work->cursorMove, 0);
        pos = 0;
    }
    func_ov197_0220d98c(work, pos);
    func_ov197_0220d974(work, TRUE);
}
