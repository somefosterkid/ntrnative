#include "types.h"
#include "app/mailbox_local.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "system/cursor_move.h"

// The mailbox screen's input: the touch screen's buttons, the cursor over the list and the keys. The ROM doesn't name
// this file; the name is a guess, after box2_ui.c

static void MailBox_CursorOn(void *work, int pos, int prevPos);
static void MailBox_CursorOff(void *work, int pos, int prevPos);
static void MailBox_CursorMoved(void *work, int pos, int prevPos);
static void MailBox_CursorTouched(void *work, int pos, int prevPos);

// The touch screen's page and return buttons, and the return button of the mail
// clang-format off
static const TouchRect sButtonRects[] = {
#include "asset/mb_ui_sButtonRects.inc"
};

// clang-format on

static const TouchRect sReturnRects[] = {
#include "asset/mb_ui_sReturnRects.inc"
};

static const CursorMoveCallbacks sCursorCallbacks = {
    MailBox_CursorOn,
    MailBox_CursorOff,
    MailBox_CursorMoved,
    MailBox_CursorTouched,
};

static const CursorMoveData sCursorData[] = {
#include "asset/mb_ui_sCursorData.inc"
};

s32 MailBox_GetTouch(MailBoxWork *work) {
    return GFL_HIDGetTouchPressedRect(sButtonRects);
}

void MailBox_CreateCursor(MailBoxWork *work) {
    u32 cursor;

    if (work->page > work->view->maxPage) {
        work->page = work->view->maxPage;
        cursor = 0;
    } else {
        cursor = work->cursor;
    }
    work->view->cursor = CursorMove_Create(sCursorData, &sCursorCallbacks, work, TRUE, cursor, HEAPID_MAILBOX_VIEW);
    MailBox_UpdateCursor(work, cursor);
}

void MailBox_FreeCursor(MailBoxWork *work) {
    CursorMove_Delete(work->view->cursor);
}

static void MailBox_CursorOn(void *work, int pos, int prevPos) {
}

static void MailBox_CursorOff(void *work, int pos, int prevPos) {
}

static void MailBox_CursorMoved(void *work, int pos, int prevPos) {
    MailBox_UpdateCursor(work, pos);
}

static void MailBox_CursorTouched(void *work, int pos, int prevPos) {
    MailBox_UpdateCursor(work, pos);
}

void MailBox_UpdateCursor(MailBoxWork *work, u32 pos) {
    const CursorMoveData *data = CursorMove_GetData(work->view->cursor, pos);

    MailBox_SetActorPos(work->view, MAILBOX_ACTOR_CURSOR, data->x, data->y);
    if (pos == 10) {
        MailBox_SetActorAnim(work->view, MAILBOX_ACTOR_CURSOR, 4);
    } else {
        MailBox_SetActorAnim(work->view, MAILBOX_ACTOR_CURSOR, 5);
    }
}

BOOL MailBox_IsReturnPressed(void) {
    if (GFL_HIDGetTouchPressedRect(sReturnRects) != -1) {
        return TRUE;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}

BOOL MailBox_IsCloseKeyPressed(void) {
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
        return TRUE;
    }
    return FALSE;
}
