// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mb_ui.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox_local.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "system/cursor_move.h"

// The mailbox screen's input: the touch screen's buttons, the cursor over the list and the keys. The ROM doesn't name
// this file; the name is a guess, after box2_ui.c

static void func_ov136_021c76b8(void *work, int pos, int prevPos);
static void func_ov136_021c76bc(void *work, int pos, int prevPos);
static void func_ov136_021c76c0(void *work, int pos, int prevPos);
static void func_ov136_021c76c8(void *work, int pos, int prevPos);

// The touch screen's page and return buttons, and the return button of the mail
// pokebw2's sButtonRects
static const TouchRect data_ov136_021c7e2c[] = {
#include "asset/mb_ui_data_ov136_021c7e2c.inc"
};

// pokebw2's sReturnRects
static const TouchRect data_ov136_021c7e14[] = {
    { 160, 191, 230, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sCursorCallbacks
static const CursorMoveCallbacks data_ov136_021c7e1c = {
    func_ov136_021c76b8,
    func_ov136_021c76bc,
    func_ov136_021c76c0,
    func_ov136_021c76c8,
};

// pokebw2's sCursorData
static const CursorMoveData data_ov136_021c7e40[] = {
#include "asset/mb_ui_data_ov136_021c7e40.inc"
};

// pokebw2's MailBox_GetTouch
s32 func_ov136_021c7648(MailBoxWork *work) {
    return func_02035c08(data_ov136_021c7e2c);
}

// pokebw2's MailBox_CreateCursor
void func_ov136_021c7658(MailBoxWork *work) {
    u32 cursor;

    if (work->page > work->view->maxPage) {
        work->page = work->view->maxPage;
        cursor = 0;
    } else {
        cursor = work->cursor;
    }
    work->view->cursor = func_02025cd0(data_ov136_021c7e40, &data_ov136_021c7e1c, work, TRUE, cursor, HEAPID_MAILBOX_VIEW);
    func_ov136_021c76d0(work, cursor);
}

// pokebw2's MailBox_FreeCursor
void func_ov136_021c76a8(MailBoxWork *work) {
    func_02025d14(work->view->cursor);
}

// pokebw2's MailBox_CursorOn
static void func_ov136_021c76b8(void *work, int pos, int prevPos) {
}

// pokebw2's MailBox_CursorOff
static void func_ov136_021c76bc(void *work, int pos, int prevPos) {
}

// pokebw2's MailBox_CursorMoved
static void func_ov136_021c76c0(void *work, int pos, int prevPos) {
    func_ov136_021c76d0(work, pos);
}

// pokebw2's MailBox_CursorTouched
static void func_ov136_021c76c8(void *work, int pos, int prevPos) {
    func_ov136_021c76d0(work, pos);
}

// pokebw2's MailBox_UpdateCursor
void func_ov136_021c76d0(MailBoxWork *work, u32 pos) {
    const CursorMoveData *data = func_0202616c(work->view->cursor, pos);

    func_ov136_021c66cc(work->view, MAILBOX_ACTOR_CURSOR, data->x, data->y);
    if (pos == 10) {
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CURSOR, 4);
    } else {
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CURSOR, 5);
    }
}

// pokebw2's MailBox_IsReturnPressed
BOOL func_ov136_021c770c(void) {
    if (func_02035c08(data_ov136_021c7e14) != -1) {
        return TRUE;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MailBox_IsCloseKeyPressed
BOOL func_ov136_021c7738(void) {
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        return TRUE;
    }
    return FALSE;
}
