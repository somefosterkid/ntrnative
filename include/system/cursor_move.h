// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/cursor_move.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_CURSOR_MOVE_H
#define BWPORT_SYSTEM_CURSOR_MOVE_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// A link with this bit goes back to the position the cursor came from, if it came along the opposite link
#define CURSOR_MOVE_LINK_RETURN 0x80

// A position of the cursor. A table of them ends with rect.top TOUCH_RECT_END
typedef struct {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    // The positions up, down, left and right of this one, with CURSOR_MOVE_LINK_RETURN
    u8 up;
    u8 down;
    u8 left;
    u8 right;
    TouchRect rect;
} CursorMoveData;

// Black 2's, not checked against Black
// Called with the caller's work, the cursor's position and its previous position (or -1)
typedef void (*CursorMoveCallback)(void *work, int pos, int prevPos);

typedef struct {
    CursorMoveCallback cursorOn;
    CursorMoveCallback cursorOff;
    CursorMoveCallback move;
    CursorMoveCallback touch;
} CursorMoveCallbacks;

// pokebw2's CursorMove_Create
CursorMove *func_02025cd0(const CursorMoveData *data, const CursorMoveCallbacks *callbacks, void *work,
                              BOOL cursorVisible, u8 pos, HeapID heapId);
// pokebw2's CursorMove_SetHideOnTouch
// The first key press after a touch only shows the cursor again
void func_02025d1c(CursorMove *cursor);
// pokebw2's CursorMove_SetBOnlyShowsCursor
// While the cursor is hidden, B only shows it, rather than also cancelling
void func_02025d24(CursorMove *cursor);
// pokebw2's CursorMove_GetData
const CursorMoveData *func_0202616c(CursorMove *cursor, u32 pos);
// pokebw2's CursorMove_SetPos
void func_020260e4(CursorMove *cursor, u8 pos);

// What the update functions return, besides the position chosen with A or by touch
#define CURSOR_MOVE_NONE (-1)

#define CURSOR_MOVE_CURSOR_ON (-4)

// pokebw2's CursorMove_SetCursorVisible
void func_020260f4(CursorMove *cursor, BOOL visible);
// pokebw2's CursorMove_GetPos
u8 func_020260e0(CursorMove *cursor);

#define CURSOR_MOVE_CANCEL (-2)
#define CURSOR_MOVE_MOVED (-3)

// A key pushed against the edge: right, left, down and up
#define CURSOR_MOVE_EDGE_RIGHT (-5)

#define CURSOR_MOVE_EDGE_LEFT (-6)

// pokebw2's CursorMove_UpdatePressed
// Runs a frame: returns the position chosen, or a CURSOR_MOVE_* code. The first moves on presses, the second on
// repeats too
int func_02025d2c(CursorMove *cursor);

// The directions of the links between positions
#define CURSOR_MOVE_UP 0

#define CURSOR_MOVE_DOWN 1
#define CURSOR_MOVE_LEFT 2
#define CURSOR_MOVE_RIGHT 3
#define CURSOR_MOVE_LINK_POS 0x7f
#define CURSOR_MOVE_NO_LINK 0xff
#define CURSOR_MOVE_EDGE_DOWN (-7)
#define CURSOR_MOVE_EDGE_UP (-8)

// pokebw2's CursorMove_DisablePos
// Disabled positions are skipped and can't be touched
void func_02026124(CursorMove *cursor, u32 pos);
// pokebw2's CursorMove_EnablePos
void func_0202614c(CursorMove *cursor, u32 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CursorMove_Delete
void func_02025d14(CursorMove *cursor);
// pokebw2's CursorMove_Update
u32 func_02025de8(CursorMove *cursor);
// pokebw2's CursorMove_IsCursorVisible
BOOL func_020260f0(CursorMove *cursor);

#endif // BWPORT_SYSTEM_CURSOR_MOVE_H
