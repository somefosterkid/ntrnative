// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/cursor_move.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/cursor_move.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/heapsys.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"

// A cursor that moves between the positions of a table with the keys or the touch screen. A touch hides the cursor;
// a key shows it again

struct CursorMove {
    const CursorMoveData *data;
    BOOL cursorVisible;
    BOOL bOnlyShowsCursor;
    u8 hideOnTouch;
    u8 pos;
    u8 prevPos;
    // The position to go back to along a CURSOR_MOVE_LINK_RETURN link, or CURSOR_MOVE_NO_LINK
    u8 returnPos;
    // A bit per position, set while it is enabled
    u32 enabled[2];
    const CursorMoveCallbacks *callbacks;
    void *work;
};

static int func_02025ea4(CursorMove *cursor, int *pos);
static u8 func_02025f60(const CursorMoveData *data, u8 pos, u8 dir);
static int func_02025fa4(CursorMove *cursor);
static int func_02025fc8(CursorMove *cursor);
static int func_02026010(CursorMove *cursor, u8 dir, u8 next);
static BOOL func_02026178(const CursorMoveData *data, u8 dir);
static BOOL func_020261c4(const u32 *enabled, u32 pos);

CursorMove *func_02025cd0(const CursorMoveData *data, const CursorMoveCallbacks *callbacks, void *work,
                              BOOL cursorVisible, u8 pos, HeapID heapId) {
    CursorMove *cursor = GFL_HeapAllocate(heapId, sizeof(CursorMove), FALSE, "cursor_move.c", 106);

    cursor->data = data;
    cursor->callbacks = callbacks;
    cursor->work = work;
    cursor->hideOnTouch = FALSE;
    cursor->cursorVisible = cursorVisible;
    cursor->bOnlyShowsCursor = FALSE;
    cursor->pos = pos;
    cursor->prevPos = CURSOR_MOVE_NO_LINK;
    cursor->returnPos = CURSOR_MOVE_NO_LINK;
    cursor->enabled[0] = 0xffffffff;
    cursor->enabled[1] = 0xffffffff;
    return cursor;
}

// pokebw2's CursorMove_Delete
void func_02025d14(CursorMove *cursor) {
    // The only free in the game that skips GFL_HeapFree's debug trace
    GFL_HeapFreeCore(cursor);
}

// pokebw2's CursorMove_SetHideOnTouch
void func_02025d1c(CursorMove *cursor) {
    cursor->hideOnTouch = TRUE;
}

// pokebw2's CursorMove_SetBOnlyShowsCursor
void func_02025d24(CursorMove *cursor) {
    cursor->bOnlyShowsCursor = TRUE;
}

// pokebw2's CursorMove_UpdatePressed
int func_02025d2c(CursorMove *cursor) {
    int pos;
    u8 dir;
    u8 next;

    if (func_02025ea4(cursor, &pos) == TRUE) {
        return pos;
    }
    if (cursor->cursorVisible == FALSE && cursor->hideOnTouch == TRUE) {
        return func_02025fc8(cursor);
    }

    if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_UP);
        dir = CURSOR_MOVE_UP;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_DOWN);
        dir = CURSOR_MOVE_DOWN;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_LEFT);
        dir = CURSOR_MOVE_LEFT;
    } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_RIGHT);
        dir = CURSOR_MOVE_RIGHT;
    } else {
        next = CURSOR_MOVE_NO_LINK;
    }
    if (next != CURSOR_MOVE_NO_LINK) {
        return func_02026010(cursor, dir, next);
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_020355d8(FALSE);
        return cursor->pos;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        return CURSOR_MOVE_CANCEL;
    }
    return CURSOR_MOVE_NONE;
}

// pokebw2's CursorMove_Update
u32 func_02025de8(CursorMove *cursor) {
    int pos;
    u8 dir;
    u8 next;

    if (func_02025ea4(cursor, &pos) == TRUE) {
        return pos;
    }
    if (cursor->cursorVisible == FALSE && cursor->hideOnTouch == TRUE) {
        return func_02025fc8(cursor);
    }

    if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_UP);
        dir = CURSOR_MOVE_UP;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_DOWN);
        dir = CURSOR_MOVE_DOWN;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_LEFT);
        dir = CURSOR_MOVE_LEFT;
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        next = func_02025f60(cursor->data, cursor->pos, CURSOR_MOVE_RIGHT);
        dir = CURSOR_MOVE_RIGHT;
    } else {
        next = CURSOR_MOVE_NO_LINK;
    }
    if (next != CURSOR_MOVE_NO_LINK) {
        return func_02026010(cursor, dir, next);
    }

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        func_020355d8(FALSE);
        return cursor->pos;
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_020355d8(FALSE);
        return CURSOR_MOVE_CANCEL;
    }
    return CURSOR_MOVE_NONE;
}

// Whether the screen was touched this frame; pos is the enabled position touched, or -1
// pokebw2's CursorMove_Touch
static int func_02025ea4(CursorMove *cursor, int *pos) {
    TouchRect rects[2];
    BOOL touched;
    u16 i;

    rects[1] = (TouchRect){ TOUCH_RECT_END, 0, 0, 0 };
    touched = FALSE;
    *pos = -1;
    for (i = 0;; i++) {
        if (cursor->data[i].rect.top == TOUCH_RECT_END) {
            break;
        }
        rects[0] = cursor->data[i].rect;
        if (func_02035c08(rects) != TOUCH_RECT_NONE) {
            touched = TRUE;
            if (func_020261c4(cursor->enabled, i) == TRUE) {
                cursor->prevPos = cursor->pos;
                cursor->pos = i;
                if (cursor->cursorVisible == TRUE && cursor->hideOnTouch == TRUE) {
                    cursor->cursorVisible = FALSE;
                    cursor->callbacks->cursorOff(cursor->work, cursor->pos, cursor->prevPos);
                }
                cursor->callbacks->touch(cursor->work, cursor->pos, cursor->prevPos);
                cursor->prevPos = CURSOR_MOVE_NO_LINK;
                cursor->returnPos = CURSOR_MOVE_NO_LINK;
                *pos = i;
                func_020355d8(TRUE);
                break;
            }
        }
    }
    return touched;
}

// pokebw2's CursorMoveData_GetLink
static u8 func_02025f60(const CursorMoveData *data, u8 pos, u8 dir) {
    u8 next = pos;

    switch (dir) {
    case CURSOR_MOVE_UP:
        next = data[pos].up;
        break;
    case CURSOR_MOVE_DOWN:
        next = data[pos].down;
        break;
    case CURSOR_MOVE_LEFT:
        next = data[pos].left;
        break;
    case CURSOR_MOVE_RIGHT:
        next = data[pos].right;
        break;
    }
    return next;
}

// pokebw2's CursorMove_ShowCursor
static int func_02025fa4(CursorMove *cursor) {
    func_020355d8(FALSE);
    cursor->cursorVisible = TRUE;
    cursor->prevPos = CURSOR_MOVE_NO_LINK;
    cursor->returnPos = CURSOR_MOVE_NO_LINK;
    cursor->callbacks->cursorOn(cursor->work, cursor->pos, cursor->prevPos);
    return CURSOR_MOVE_CURSOR_ON;
}

// pokebw2's CursorMove_ShowCursorByKey
static int func_02025fc8(CursorMove *cursor) {
    if (cursor->bOnlyShowsCursor == TRUE) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_PLUS_KEY_MASK)) {
            return func_02025fa4(cursor);
        }
    } else {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_PLUS_KEY_MASK)) {
            return func_02025fa4(cursor);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            func_02025fa4(cursor);
            return CURSOR_MOVE_CANCEL;
        }
    }
    return CURSOR_MOVE_NONE;
}

// pokebw2's CursorMove_Move
static int func_02026010(CursorMove *cursor, u8 dir, u8 next) {
    BOOL direct;

    func_020355d8(FALSE);
    if (next & CURSOR_MOVE_LINK_RETURN) {
        if (cursor->returnPos != CURSOR_MOVE_NO_LINK) {
            next = cursor->returnPos;
        } else {
            next ^= CURSOR_MOVE_LINK_RETURN;
        }
    }

    // Skip the disabled positions along the direction, or stay put
    direct = TRUE;
    while (TRUE) {
        u8 link;

        if (cursor->enabled[next / 32] & (1 << (next % 32))) {
            break;
        }
        direct = FALSE;
        link = func_02025f60(cursor->data, next, dir) & CURSOR_MOVE_LINK_POS;
        if (link == next || link == cursor->pos) {
            next = cursor->pos;
            break;
        }
        next = link;
    }

    if (cursor->pos != next) {
        if (func_02026178(&cursor->data[next], dir) == TRUE && direct) {
            cursor->returnPos = cursor->pos;
        } else {
            cursor->returnPos = CURSOR_MOVE_NO_LINK;
        }
        cursor->prevPos = cursor->pos;
        cursor->pos = next;
        cursor->callbacks->move(cursor->work, cursor->pos, cursor->prevPos);
        return CURSOR_MOVE_MOVED;
    }

    if (dir == CURSOR_MOVE_UP) {
        return CURSOR_MOVE_EDGE_UP;
    }
    if (dir == CURSOR_MOVE_DOWN) {
        return CURSOR_MOVE_EDGE_DOWN;
    }
    if (dir == CURSOR_MOVE_LEFT) {
        return CURSOR_MOVE_EDGE_LEFT;
    }
    if (dir == CURSOR_MOVE_RIGHT) {
        return CURSOR_MOVE_EDGE_RIGHT;
    }
    return CURSOR_MOVE_NONE;
}

// pokebw2's CursorMove_GetPos
u8 func_020260e0(CursorMove *cursor) {
    return cursor->pos;
}

// pokebw2's CursorMove_SetPos
void func_020260e4(CursorMove *cursor, u8 pos) {
    cursor->pos = pos;
    cursor->prevPos = CURSOR_MOVE_NO_LINK;
    cursor->returnPos = CURSOR_MOVE_NO_LINK;
}

// pokebw2's CursorMove_IsCursorVisible
BOOL func_020260f0(CursorMove *cursor) {
    return cursor->cursorVisible;
}

// pokebw2's CursorMove_SetCursorVisible
void func_020260f4(CursorMove *cursor, BOOL visible) {
    cursor->cursorVisible = visible;
    if (visible == TRUE) {
        cursor->prevPos = CURSOR_MOVE_NO_LINK;
        cursor->returnPos = CURSOR_MOVE_NO_LINK;
        cursor->callbacks->cursorOn(cursor->work, cursor->pos, -1);
    } else {
        cursor->callbacks->cursorOff(cursor->work, cursor->pos, -1);
    }
}

// pokebw2's CursorMove_DisablePos
void func_02026124(CursorMove *cursor, u32 pos) {
    if (cursor->enabled[pos / 32] & (1 << (pos % 32))) {
        cursor->enabled[pos / 32] &= (0xffffffff ^ (1 << (pos % 32)));
    }
}

// pokebw2's CursorMove_EnablePos
void func_0202614c(CursorMove *cursor, u32 pos) {
    if (!(cursor->enabled[pos / 32] & (1 << (pos % 32)))) {
        cursor->enabled[pos / 32] ^= (1 << (pos % 32));
    }
}

// pokebw2's CursorMove_GetData
const CursorMoveData *func_0202616c(CursorMove *cursor, u32 pos) {
    return &cursor->data[pos];
}

// Whether moving dir arrives at data along a link that goes back the way it came
// pokebw2's CursorMoveData_IsReturnLink
static BOOL func_02026178(const CursorMoveData *data, u8 dir) {
    switch (dir) {
    case CURSOR_MOVE_UP:
        if (data->down & CURSOR_MOVE_LINK_RETURN) {
            return TRUE;
        }
        break;
    case CURSOR_MOVE_DOWN:
        if (data->up & CURSOR_MOVE_LINK_RETURN) {
            return TRUE;
        }
        break;
    case CURSOR_MOVE_LEFT:
        if (data->right & CURSOR_MOVE_LINK_RETURN) {
            return TRUE;
        }
        break;
    case CURSOR_MOVE_RIGHT:
        if (data->left & CURSOR_MOVE_LINK_RETURN) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's CursorMove_IsPosEnabled
static BOOL func_020261c4(const u32 *enabled, u32 pos) {
    return (enabled[pos / 32] & (1 << (pos % 32))) ? TRUE : FALSE;
}
