#include "battle/b_plist_cursor.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_main.h"
#include "system/cursor_move.h"

// The battle party list's key cursor: a CursorMove on the page's table of positions, whose callbacks move and show
// overlay 285's cursor. The file name is our guess, after the ROM's b_plist_main.c and b_plist_anm.c and the
// b_plist_* files of the earlier games.

static void BPlistCursor_SetVisible(BPlistWork *work, BOOL visible);
static void BPlistCursor_Put(BPlistWork *work, int pos);
static void BPlistCursor_CallbackCursorOn(void *work, int pos, int prevPos);
static void BPlistCursor_CallbackCursorOff(void *work, int pos, int prevPos);
static void BPlistCursor_CallbackTouch(void *work, int pos, int prevPos);
static void BPlistCursor_CallbackMove(void *work, int pos, int prevPos);

// The positions of each page; pages 0 and 8 share the party's
static const CursorMoveData data_ov287_021fb78c[8] = {
#include "asset/b_plist_cursor_data_ov287_021fb78c.inc"
};

static const CursorMoveData data_ov287_021fb630[5] = {
#include "asset/b_plist_cursor_data_ov287_021fb630.inc"
};

static const CursorMoveData data_ov287_021fb66c[5] = {
#include "asset/b_plist_cursor_data_ov287_021fb66c.inc"
};

static const CursorMoveData data_ov287_021fb7ec[9] = {
#include "asset/b_plist_cursor_data_ov287_021fb7ec.inc"
};

static const CursorMoveData data_ov287_021fb6a8[6] = {
#include "asset/b_plist_cursor_data_ov287_021fb6a8.inc"
};

static const CursorMoveData data_ov287_021fb6f0[6] = {
#include "asset/b_plist_cursor_data_ov287_021fb6f0.inc"
};

static const CursorMoveData data_ov287_021fb738[7] = {
#include "asset/b_plist_cursor_data_ov287_021fb738.inc"
};

static const CursorMoveData data_ov287_021fb5e8[3] = {
#include "asset/b_plist_cursor_data_ov287_021fb5e8.inc"
};

static const CursorMoveData *const data_ov287_021fb60c[9] = {
    data_ov287_021fb78c, data_ov287_021fb630, data_ov287_021fb66c, data_ov287_021fb7ec, data_ov287_021fb6a8,
    data_ov287_021fb6f0, data_ov287_021fb738, data_ov287_021fb5e8, data_ov287_021fb78c,
};

static const CursorMoveCallbacks data_ov287_021fb5d8 = {
    BPlistCursor_CallbackCursorOn,
    BPlistCursor_CallbackCursorOff,
    BPlistCursor_CallbackMove,
    BPlistCursor_CallbackTouch,
};

void BPlistCursor_Create(BPlistWork *work, u8 page, int pos) {
    work->cursorMove = CursorMove_Create(data_ov287_021fb60c[page], &data_ov287_021fb5d8, work, work->cursorVisible,
                                         pos, work->param->heapId);
    CursorMove_SetHideOnTouch(work->cursorMove);
    CursorMove_SetBOnlyShowsCursor(work->cursorMove);
    BPlistCursor_Put(work, pos);
}

void BPlistCursor_Delete(BPlistWork *work) {
    CursorMove_Delete(work->cursorMove);
}

void BPlistCursor_ChangePage(BPlistWork *work, u8 page, int pos) {
    BPlistCursor_Delete(work);
    BPlistCursor_Create(work, page, pos);
}

static void BPlistCursor_SetVisible(BPlistWork *work, BOOL visible) {
    work->cursorVisible = visible;
    BAppCursor_SetVisible(work->cursor, visible);
}

static void BPlistCursor_Put(BPlistWork *work, int pos) {
    BAppCursor_SetPos(work->cursor, CursorMove_GetData(work->cursorMove, pos));
}

static void BPlistCursor_CallbackCursorOn(void *work, int pos, int prevPos) {
    BPlistCursor_Put(work, pos);
    BPlistCursor_SetVisible(work, TRUE);
}

static void BPlistCursor_CallbackCursorOff(void *work, int pos, int prevPos) {
    BPlistCursor_SetVisible(work, FALSE);
}

static void BPlistCursor_CallbackTouch(void *work, int pos, int prevPos) {
    BPlistCursor_SetVisible(work, FALSE);
}

static void BPlistCursor_CallbackMove(void *work, int pos, int prevPos) {
    BPlistCursor_Put(work, pos);
}
