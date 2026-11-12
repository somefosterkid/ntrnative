#include "battle/b_bag_ui.h"
#include "battle/b_app_tool.h"
#include "battle/b_bag_main.h"
#include "system/cursor_move.h"

// The battle bag's key cursor: a CursorMove on the page's table of positions, whose callbacks move and show overlay
// 285's cursor. The file name is our guess, after the ROM's b_bag_main.c and Diamond and Pearl's b_bag_* files.

static void BBagUi_SetCursorVisible(BBagWork *work, BOOL visible);
static void BBagUi_PutCursor(BBagWork *work, int pos);
static void BBagUi_CallbackCursorOn(void *work, int pos, int prevPos);
static void BBagUi_CallbackCursorOff(void *work, int pos, int prevPos);
static void BBagUi_CallbackTouch(void *work, int pos, int prevPos);
static void BBagUi_CallbackMove(void *work, int pos, int prevPos);
static void BBagUi_CallbackItemsCursorOn(void *work, int pos, int prevPos);

// The pockets
static const CursorMoveData data_ov286_021f7348[7] = {
#include "asset/b_bag_ui_data_ov286_021f7348.inc"
};

// A pocket's items; 7 and 8, the page arrows, are touch only
static const CursorMoveData data_ov286_021f739c[10] = {
#include "asset/b_bag_ui_data_ov286_021f739c.inc"
};

// One item
static const CursorMoveData data_ov286_021f7324[3] = {
#include "asset/b_bag_ui_data_ov286_021f7324.inc"
};

static const CursorMoveCallbacks data_ov286_021f7304 = {
    BBagUi_CallbackItemsCursorOn,
    BBagUi_CallbackCursorOff,
    BBagUi_CallbackMove,
    BBagUi_CallbackTouch,
};

static const CursorMoveCallbacks data_ov286_021f7314 = {
    BBagUi_CallbackCursorOn,
    BBagUi_CallbackCursorOff,
    BBagUi_CallbackMove,
    BBagUi_CallbackTouch,
};

static const CursorMoveCallbacks data_ov286_021f72f4 = {
    BBagUi_CallbackCursorOn,
    BBagUi_CallbackCursorOff,
    BBagUi_CallbackMove,
    BBagUi_CallbackTouch,
};

static const CursorMoveData *const data_ov286_021f72e8[3] = {
    data_ov286_021f7348,
    data_ov286_021f739c,
    data_ov286_021f7324,
};

static const CursorMoveCallbacks *const data_ov286_021f72dc[3] = {
    &data_ov286_021f7314,
    &data_ov286_021f7304,
    &data_ov286_021f72f4,
};

void BBagUi_CreateCursor(BBagWork *work, u8 page, int pos) {
    work->cursorMove = CursorMove_Create(data_ov286_021f72e8[page], data_ov286_021f72dc[page], work,
                                         work->cursorVisible, pos, work->param->heapId);
    CursorMove_SetHideOnTouch(work->cursorMove);
    CursorMove_SetBOnlyShowsCursor(work->cursorMove);
    BBagUi_PutCursor(work, pos);
}

void BBagUi_DeleteCursor(BBagWork *work) {
    CursorMove_Delete(work->cursorMove);
}

void BBagUi_ChangeCursorPage(BBagWork *work, u8 page, int pos) {
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
    BBagUi_DeleteCursor(work);
    BBagUi_CreateCursor(work, page, pos);
}

static void BBagUi_SetCursorVisible(BBagWork *work, BOOL visible) {
    work->cursorVisible = visible;
    BAppCursor_SetVisible(work->cursor, visible);
}

static void BBagUi_PutCursor(BBagWork *work, int pos) {
    BAppCursor_SetPos(work->cursor, CursorMove_GetData(work->cursorMove, pos));
}

static void BBagUi_CallbackCursorOn(void *work, int pos, int prevPos) {
    BBagUi_PutCursor(work, pos);
    BBagUi_SetCursorVisible(work, TRUE);
}

static void BBagUi_CallbackCursorOff(void *work, int pos, int prevPos) {
    BBagUi_SetCursorVisible(work, FALSE);
}

static void BBagUi_CallbackTouch(void *work, int pos, int prevPos) {
    BBagUi_SetCursorVisible(work, FALSE);
}

static void BBagUi_CallbackMove(void *work, int pos, int prevPos) {
    BBagUi_PutCursor(work, pos);
}

// On the items page, the cursor shown on a page arrow goes to the first item
static void BBagUi_CallbackItemsCursorOn(void *wk, int pos, int prevPos) {
    BBagWork *work = wk;

    if (pos == 7 || pos == 8) {
        CursorMove_SetPos(work->cursorMove, 0);
        pos = 0;
    }
    BBagUi_PutCursor(work, pos);
    BBagUi_SetCursorVisible(work, TRUE);
}
