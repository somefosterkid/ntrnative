// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov287/b_plist_cursor.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_plist_cursor.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_main.h"
#include "system/cursor_move.h"

// The battle party list's key cursor: a CursorMove on the page's table of positions, whose callbacks move and show
// overlay 285's cursor. The file name is our guess, after the ROM's b_plist_main.c and b_plist_anm.c and the
// b_plist_* files of the earlier games.

static void func_ov198_02211a10(BPlistWork *work, BOOL visible);
static void func_ov198_02211a24(BPlistWork *work, int pos);
void func_ov198_02211a40(void *work, int pos, int prevPos);
void func_ov198_02211a54(void *work, int pos, int prevPos);
void func_ov198_02211a60(void *work, int pos, int prevPos);
void func_ov198_02211a6c(void *work, int pos, int prevPos);

// The positions of each page; pages 0 and 8 share the party's
static const CursorMoveData data_ov198_02212454[8] = {
#include "asset/b_plist_cursor_data_ov198_02212454.inc"
};

static const CursorMoveData data_ov198_022122f8[5] = {
#include "asset/b_plist_cursor_data_ov198_022122f8.inc"
};

static const CursorMoveData data_ov198_02212334[5] = {
#include "asset/b_plist_cursor_data_ov198_02212334.inc"
};

static const CursorMoveData data_ov198_022124b4[9] = {
#include "asset/b_plist_cursor_data_ov198_022124b4.inc"
};

static const CursorMoveData data_ov198_02212370[6] = {
#include "asset/b_plist_cursor_data_ov198_02212370.inc"
};

static const CursorMoveData data_ov198_022123b8[6] = {
#include "asset/b_plist_cursor_data_ov198_022123b8.inc"
};

static const CursorMoveData data_ov198_02212400[7] = {
#include "asset/b_plist_cursor_data_ov198_02212400.inc"
};

static const CursorMoveData data_ov198_022122b0[3] = {
#include "asset/b_plist_cursor_data_ov198_022122b0.inc"
};

static const CursorMoveData *const data_ov198_022122d4[9] = {
    data_ov198_02212454, data_ov198_022122f8,
    data_ov198_02212334, data_ov198_022124b4,
    data_ov198_02212370, data_ov198_022123b8,
    data_ov198_02212400, data_ov198_022122b0,
    data_ov198_02212454,
};

static const CursorMoveCallbacks data_ov198_022122a0 = {
    func_ov198_02211a40,
    func_ov198_02211a54,
    func_ov198_02211a6c,
    func_ov198_02211a60,
};

// pokebw2's BPlistCursor_Create
void func_ov198_02211998(BPlistWork *work, u8 page, int pos) {
    work->cursorMove = func_02025cd0(data_ov198_022122d4[page], &data_ov198_022122a0, work, work->cursorVisible, pos,
                                     work->param->heapId);
    func_02025d1c(work->cursorMove);
    func_02025d24(work->cursorMove);
    func_ov198_02211a24(work, pos);
}

// pokebw2's BPlistCursor_Delete
void func_ov198_022119e8(BPlistWork *work) {
    func_02025d14(work->cursorMove);
}

// pokebw2's BPlistCursor_ChangePage
void func_ov198_022119f8(BPlistWork *work, u8 page, int pos) {
    func_ov198_022119e8(work);
    func_ov198_02211998(work, page, pos);
}

// pokebw2's BPlistCursor_SetVisible
static void func_ov198_02211a10(BPlistWork *work, BOOL visible) {
    work->cursorVisible = visible;
    func_ov196_0220afdc(work->cursor, visible);
}

// pokebw2's BPlistCursor_Put
static void func_ov198_02211a24(BPlistWork *work, int pos) {
    const CursorMoveData *data = func_0202616c(work->cursorMove, pos);
    func_ov196_0220b000(work->cursor, data);
}

// pokebw2's BPlistCursor_CallbackCursorOn
void func_ov198_02211a40(void *work, int pos, int prevPos) {
    func_ov198_02211a24(work, pos);
    func_ov198_02211a10(work, TRUE);
}

// pokebw2's BPlistCursor_CallbackCursorOff
void func_ov198_02211a54(void *work, int pos, int prevPos) {
    func_ov198_02211a10(work, FALSE);
}

// pokebw2's BPlistCursor_CallbackTouch
void func_ov198_02211a60(void *work, int pos, int prevPos) {
    func_ov198_02211a10(work, FALSE);
}

// pokebw2's BPlistCursor_CallbackMove
void func_ov198_02211a6c(void *work, int pos, int prevPos) {
    func_ov198_02211a24(work, pos);
}
