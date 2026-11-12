#include "types.h"
#include "app/box2_bgwfrm.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "app/box2_ui.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/cursor_move.h"

// The PC box's cursor: the positions it moves between in each of the box's modes and what moving it and touching a
// position do, and the touch screen's areas. The ROM names this file in its asserts. Names are ours

// A cursor's positions and callbacks
typedef struct {
    const CursorMoveData *data;
    const CursorMoveCallbacks *callbacks;
} Box2CursorMoveTable;

static void MoveCursorTo(Box2SysWork *syswk, u32 pos);
static void CursorOn(void *work, int pos, int prevPos);
static void CursorOff(void *work, int pos, int prevPos);
static void CursorMoveAnim(void *work, int pos, int prevPos);
static void Cursor9_Touch(void *work, int pos, int prevPos);
static void CursorNone(void *work, int pos, int prevPos);
static void Cursor0_Touch(void *work, int pos, int prevPos);
static void Cursor2_Touch(void *work, int pos, int prevPos);
static void Cursor3_Move(void *work, int pos, int prevPos);
static void Cursor3_Touch(void *work, int pos, int prevPos);
static void Cursor4_Move(void *work, int pos, int prevPos);
static void Cursor4_Touch(void *work, int pos, int prevPos);
static void Cursor5_Move(void *work, int pos, int prevPos);
static void Cursor5_Touch(void *work, int pos, int prevPos);
static void Cursor6_Move(void *work, int pos, int prevPos);
static void Cursor6_Touch(void *work, int pos, int prevPos);
static void Cursor7_Move(void *work, int pos, int prevPos);
static void Cursor7_Touch(void *work, int pos, int prevPos);
static void Cursor8_Move(void *work, int pos, int prevPos);
static void Cursor8_Touch(void *work, int pos, int prevPos);
static void Cursor10_Move(void *work, int pos, int prevPos);
static void Cursor10_Touch(void *work, int pos, int prevPos);
static void Cursor11_Move(void *work, int pos, int prevPos);
static void Cursor11_Touch(void *work, int pos, int prevPos);
static void Cursor12_Move(void *work, int pos, int prevPos);
static void Cursor12_Touch(void *work, int pos, int prevPos);
static void Cursor13_Move(void *work, int pos, int prevPos);
static void Cursor13_Touch(void *work, int pos, int prevPos);
static void Cursor14_Move(void *work, int pos, int prevPos);
static void Cursor14_Touch(void *work, int pos, int prevPos);
static void Cursor15_Move(void *work, int pos, int prevPos);
static void Cursor15_Touch(void *work, int pos, int prevPos);
static void StartCursorMoveWork(Box2AppWork *app, u8 fromX, u8 fromY, u8 toX, u8 toY);
static u32 FindTriggeredRect(const CursorMoveData *data, u32 count);
static u32 FindHeldRect(const CursorMoveData *data, u32 count);
static u32 FindRectAt(const CursorMoveData *data, u32 count, u32 x, u32 y);

// MWCC sorts a file's data by size, in an order that depends on where each object is declared; the objects of the same
// size are declared in the order that lays them out as in the ROM (tools/decomp/rodata_order.py)

// The end of a table of touch rectangles
static const TouchRect sTouchRectEnd = { TOUCH_RECT_END, 0, 0, 0 };

static const TouchRect sTouchRects_78cc[2] = {
    { 0, 191, 168, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect sTouchRects_78d4[2] = {
    { 48, 167, 16, 103 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect sTouchRects_78c4[2] = {
    { 40, 160, 0, 167 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect sTouchRects_78dc[2] = {
    { 48, 167, 168, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect sTouchRects_78f0[3] = {
    { 0, 167, 228, 255 },
    { 0, 167, 168, 195 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect sTouchRects_78e4[3] = {
    { 0, 21, 173, 255 },
    { 148, 167, 173, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const CursorMoveCallbacks sCursorCallbacks10 = {
    CursorOn,
    CursorOff,
    Cursor10_Move,
    Cursor10_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks12 = {
    CursorOn,
    CursorOff,
    Cursor12_Move,
    Cursor12_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks13 = {
    CursorOn,
    CursorOff,
    Cursor13_Move,
    Cursor13_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks5 = {
    CursorOn,
    CursorOff,
    Cursor5_Move,
    Cursor5_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks3 = {
    CursorOn,
    CursorOff,
    Cursor3_Move,
    Cursor3_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks2 = {
    CursorOn,
    CursorOff,
    CursorMoveAnim,
    Cursor2_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks4 = {
    CursorOn,
    CursorOff,
    Cursor4_Move,
    Cursor4_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks6 = {
    CursorOn,
    CursorOff,
    Cursor6_Move,
    Cursor6_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks14 = {
    CursorOn,
    CursorOff,
    Cursor14_Move,
    Cursor14_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks7 = {
    CursorOn,
    CursorOff,
    Cursor7_Move,
    Cursor7_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks11 = {
    CursorOn,
    CursorOff,
    Cursor11_Move,
    Cursor11_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks15 = {
    CursorOn,
    CursorOff,
    Cursor15_Move,
    Cursor15_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks8 = {
    CursorOn,
    CursorOff,
    Cursor8_Move,
    Cursor8_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks1 = {
    CursorOn,
    CursorOff,
    CursorNone,
    CursorNone,
};

static const CursorMoveCallbacks sCursorCallbacks9 = {
    CursorOn,
    CursorOff,
    CursorMoveAnim,
    Cursor9_Touch,
};

static const CursorMoveCallbacks sCursorCallbacks0 = {
    CursorOn,
    CursorOff,
    CursorMoveAnim,
    Cursor0_Touch,
};

static const TouchRect sTouchRects_79fc[5] = {
#include "asset/box2_ui_sTouchRects_79fc.inc"
};

// clang-format off
static const CursorMoveData sCursorData10[5] = {
#include "asset/box2_ui_sCursorData10.inc"
};

// clang-format on

// clang-format off
static const CursorMoveData sCursorData1[6] = {
#include "asset/box2_ui_sCursorData1.inc"
};

// clang-format on

// clang-format off
static const CursorMoveData sCursorData11[7] = {
#include "asset/box2_ui_sCursorData11.inc"
};

// clang-format on

// clang-format off
static const CursorMoveData sCursorData12[8] = {
#include "asset/box2_ui_sCursorData12.inc"
};

// clang-format on

// clang-format off
static const CursorMoveData sCursorData9[9] = {
#include "asset/box2_ui_sCursorData9.inc"
};

// clang-format on

static const CursorMoveData sCursorData0[14] = {
#include "asset/box2_ui_sCursorData0.inc"
};

static const CursorMoveData sCursorData8[15] = {
#include "asset/box2_ui_sCursorData8.inc"
};

static const CursorMoveData sCursorData14[18] = {
#include "asset/box2_ui_sCursorData14.inc"
};

static const CursorMoveData sCursorData5[20] = {
#include "asset/box2_ui_sCursorData5.inc"
};

static const CursorMoveData sCursorData15[37] = {
#include "asset/box2_ui_sCursorData15.inc"
};

static const CursorMoveData sCursorData2[41] = {
#include "asset/box2_ui_sCursorData2.inc"
};

static const CursorMoveData sCursorData7[42] = {
#include "asset/box2_ui_sCursorData7.inc"
};

static const CursorMoveData sCursorData13[45] = {
#include "asset/box2_ui_sCursorData13.inc"
};

// The positions of cursors 3, 4 and 6, which share the tray's layout. Each ends with an empty position, and
// some functions use the positions after the tray's by themselves
static const CursorMoveData sTrayCursorData[3][47] = {
#include "asset/box2_ui_sTrayCursorData.inc"
};

// The positions of each cursor and their callbacks
// clang-format off
static const Box2CursorMoveTable sCursorMoveTables[16] = {
    { sCursorData0, &sCursorCallbacks0 },
    { sCursorData1, &sCursorCallbacks1 },
    { sCursorData2, &sCursorCallbacks2 },
    { sTrayCursorData[0], &sCursorCallbacks3 },
    { sTrayCursorData[1], &sCursorCallbacks4 },
    { sCursorData5, &sCursorCallbacks5 },
    { sTrayCursorData[2], &sCursorCallbacks6 },
    { sCursorData7, &sCursorCallbacks7 },
    { sCursorData8, &sCursorCallbacks8 },
    { sCursorData9, &sCursorCallbacks9 },
    { sCursorData10, &sCursorCallbacks10 },
    { sCursorData11, &sCursorCallbacks11 },
    { sCursorData12, &sCursorCallbacks12 },
    { sCursorData13, &sCursorCallbacks13 },
    { sCursorData14, &sCursorCallbacks14 },
    { sCursorData15, &sCursorCallbacks15 },
};

// clang-format on

void Box2Ui_CreateCursor(Box2SysWork *syswk) {
    BOOL visible;
    u32 id;

    visible = GCTX_HIDIsUsingTouch() == TRUE ? FALSE : TRUE;
    switch (syswk->param->mode) {
    case BOX2_MODE_DEPOSIT:
        id = 0;
        break;
    case BOX2_MODE_WITHDRAW:
        id = 2;
        break;
    case BOX2_MODE_MOVE_POKEMON:
        id = 3;
        break;
    case BOX2_MODE_MOVE_ITEMS:
        id = 7;
        break;
    case BOX2_MODE_BATTLE_BOX:
        id = 13;
        break;
    case BOX2_MODE_DREAM_WORLD:
        id = 15;
        break;
    }
    Box2Ui_DeleteCursor(syswk);
    syswk->app->cursorMove = CursorMove_Create(sCursorMoveTables[id].data, sCursorMoveTables[id].callbacks, syswk,
                                               visible, syswk->curRcvPos, HEAPID_BOX2_APP);
    CursorMove_SetHideOnTouch(syswk->app->cursorMove);
    MoveCursorTo(syswk, syswk->curRcvPos);
    Box2Obj_ShowCursor(syswk, visible);
    syswk->app->oldCurPos = syswk->curRcvPos;
}

void Box2Ui_ChangeCursor(Box2SysWork *syswk, u32 id, u32 pos) {
    BOOL visible = CursorMove_IsCursorVisible(syswk->app->cursorMove);

    Box2Ui_DeleteCursor(syswk);
    syswk->app->cursorMove = CursorMove_Create(sCursorMoveTables[id].data, sCursorMoveTables[id].callbacks, syswk,
                                               visible, pos, HEAPID_BOX2_APP);
    CursorMove_SetHideOnTouch(syswk->app->cursorMove);
    MoveCursorTo(syswk, pos);
    Box2Obj_ShowCursor(syswk, visible);
    syswk->app->oldCurPos = pos;
}

void Box2Ui_DeleteCursor(Box2SysWork *syswk) {
    if (syswk->app->cursorMove != NULL) {
        CursorMove_Delete(syswk->app->cursorMove);
        syswk->app->cursorMove = NULL;
    }
}

void Box2Ui_PlaceCursorActor(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *data = CursorMove_GetData(syswk->app->cursorMove, pos);

    Box2Obj_SetPos(syswk->app, BOX2_ACTOR_CURSOR, data->x, data->y, 0);
}

static void MoveCursorTo(Box2SysWork *syswk, u32 pos) {
    Box2Ui_PlaceCursorActor(syswk, pos);
    Box2Obj_MoveHeldIcons(syswk);
}

static void CursorOn(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    MoveCursorTo(syswk, pos);
    Box2Obj_SetVisible(syswk->app, BOX2_ACTOR_CURSOR, TRUE);
}

static void CursorOff(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Obj_SetVisible(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
}

static void CursorMoveAnim(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
}

static void Cursor9_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    CursorMove_SetPos(syswk->app->cursorMove, pos);
    MoveCursorTo(syswk, pos);
}

static void CursorNone(void *work, int pos, int prevPos) {
}

u32 Box2Ui_UpdateCursor0(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 8 && res <= 12) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 6;
        }
        if (res == 6) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 6;
        }
        if (res == 7) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 7;
        }
    } else if (res == 6 || res == 7) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void Cursor0_Touch(void *work, int pos, int prevPos) {
    if (pos != 6 && pos != 7) {
        MoveCursorTo(work, pos);
    }
}

void Box2Ui_StartMoveToPartyPos(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *data = CursorMove_GetData(syswk->app->cursorMove, pos);

    StartCursorMoveWork(syswk->app, 84, 16, data->x, data->y);
}

void Box2Ui_StartMoveToCursor1(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *to = sCursorData1;
    const CursorMoveData *data;

    if (pos < BOX2_TRAY_POKE_MAX) {
        data = &sTrayCursorData[0][pos];
    } else {
        data = &sCursorData0[pos - BOX2_TRAY_POKE_MAX];
    }
    StartCursorMoveWork(syswk->app, data->x, data->y, to->x, to->y);
}

u32 Box2Ui_UpdateCursor2(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 35 && res <= 39) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 33;
        }
        if (res == 33) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 33;
        }
        if (res == 34) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 34;
        }
    } else if (res == 33 || res == 34) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            return 32;
        }
    }
    return res;
}

static void Cursor2_Touch(void *work, int pos, int prevPos) {
    if (pos != 33 && pos != 34) {
        MoveCursorTo(work, pos);
    }
}

u32 Box2Ui_UpdateCursor3(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 36 && res <= 41) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 34;
        }
        if (res == 34) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 34;
        }
        if (res == 35) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if (res == 33 || res == 34 || res == 35 || res == 45) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            return 32;
        }
    }
    return res;
}

static void Cursor3_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor3_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 34 || pos == 35 || pos == 42 || pos == 43 || pos == 44 || pos == 45) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE && pos >= 36 && pos <= 41) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

void Box2Ui_SetButtonStates(Box2SysWork *syswk, u32 pos) {
    u8 button1 = 0;
    u8 button3 = 0;
    u8 button4 = 0;

    if (syswk->pickState != BOX2_PICK_NONE) {
        button1 = 2;
        if (syswk->param->mode == BOX2_MODE_MOVE_ITEMS) {
            button3 = 1;
            button4 = 1;
        } else if (syswk->moveMode == 2) {
            button3 = button1;
            button4 = button1;
        }
    } else if (syswk->param->mode == BOX2_MODE_MOVE_ITEMS) {
        button3 = 1;
        button4 = 1;
    } else if (pos >= BOX2_PARTY_POS) {
        if (Box2Frame_IsPartyAtRight(syswk->app->bgWinFrame) == FALSE ||
            Box2Main_GetPokeParam(syswk, pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            button3 = 2;
        }
    } else if (Box2Main_GetPokeParam(syswk, pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
        button3 = 2;
    }
    Box2Obj_SetButtons(syswk, button1, 1, button3, button4);
}

static void Cursor4_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_SetButtonStates(syswk, pos);
    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor4_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 31 || pos == 32) {
        CursorMove_SetPos(syswk->app->cursorMove, BOX2_PARTY_POS);
        pos = BOX2_PARTY_POS;
    } else if (pos == 33 || pos == 38 || pos == 40 || pos == 41 || pos == 42 || pos == 43 || pos == 44 || pos == 45) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    Box2Main_UpdateRangeIconsPos(syswk, pos);
    Box2Ui_SetButtonStates(syswk, pos);
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor4(Box2SysWork *syswk) {
    u32 x;
    u32 y;
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    GFL_HIDGetTouchHeldPoint(&x, &y);
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            Box2Main_ShowCursor(syswk);
            return 40;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            Box2Main_UpdateRangeIconsPos(syswk, BOX2_PARTY_POS);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            Box2Main_UpdateRangeIconsPos(syswk, BOX2_PARTY_POS);
            return 32;
        }
    }
    return res;
}

u32 Box2Ui_UpdateCursor5(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 9 && res <= 14) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 7;
        }
        if (res == 7) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 7;
        }
        if (res == 8) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if (res == 6 || res == 7 || res == 8 || res == 18) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void Cursor5_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor5_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 7 || pos == 8 || pos == 15 || pos == 16 || pos == 17 || pos == 18) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE && pos >= 9 && pos <= 14) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor6(Box2SysWork *syswk) {
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            Box2Main_ShowCursor(syswk);
            return 40;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            Box2Main_UpdateRangeIconsPos(syswk, BOX2_BOXLIST_POS);
            return 37;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            Box2Main_UpdateRangeIconsPos(syswk, BOX2_BOXLIST_POS);
            return 38;
        }
    }
    return res;
}

static void Cursor6_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_SetButtonStates(syswk, pos);
    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor6_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 37 || pos == 38) {
        CursorMove_SetPos(syswk->app->cursorMove, BOX2_BOXLIST_POS);
        pos = BOX2_BOXLIST_POS;
    } else if (pos == 40 || pos == 41 || pos == 42 || pos == 43 || pos == 44 || pos == 45) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    Box2Main_UpdateRangeIconsPos(syswk, pos);
    Box2Ui_SetButtonStates(syswk, pos);
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor7(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res == 36 || res == 37 || res == 38) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 34;
        }
        if (res == 34) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 34;
        }
        if (res == 35) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if (res == 33 || res == 34 || res == 35) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            return 32;
        }
    }
    return res;
}

static void Cursor7_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor7_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 34 || pos == 35 || pos == 39 || pos == 40) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor8(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res == 9 || res == 10 || res == 11) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 7;
        }
        if (res == 7) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 7;
        }
        if (res == 8) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if (res == 6 || res == 7 || res == 8) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void Cursor8_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor8_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 7 || pos == 8 || pos == 12 || pos == 13) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

static void Cursor10_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
}

static void Cursor10_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    CursorMove_SetPos(syswk->app->cursorMove, pos);
    MoveCursorTo(syswk, pos);
}

static void Cursor11_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
}

static void Cursor11_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    CursorMove_SetPos(syswk->app->cursorMove, pos);
    MoveCursorTo(syswk, pos);
}

static void Cursor12_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos >= 1 && pos <= 4) {
        Box2Obj_ShowBoxNameFrame(syswk, pos - 1, TRUE);
    } else {
        Box2Obj_ShowBoxNameFrame(syswk, 0, FALSE);
    }
    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor12_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 0 || pos == 5 || pos == 6) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor13(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 36 && res <= 39) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 34;
        }
        if (res == 34) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 34;
        }
        if (res == 35) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if (res == 30 || res == 33 || res == 34 || res == 35 || res == 43) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            return 32;
        }
    }
    return res;
}

static void Cursor13_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor13_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 34 || pos == 35 || pos == 40 || pos == 41 || pos == 42 || pos == 43) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE && pos >= 36 && pos <= 39) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor14(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 9 && res <= 12) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            GCTX_HIDSetUsingTouch(FALSE);
            return 7;
        }
        if (res == 7) {
            GCTX_HIDSetUsingTouch(TRUE);
            return 7;
        }
        if (res == 8) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if (res == 6 || res == 7 || res == 8 || res == 16) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void Cursor14_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
    syswk->app->oldCurPos = pos;
}

static void Cursor14_Touch(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 7 || pos == 8 || pos == 13 || pos == 14 || pos == 15 || pos == 16) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE && pos >= 9 && pos <= 12) {
        CursorMove_SetPos(syswk->app->cursorMove, syswk->app->oldCurPos);
        pos = syswk->app->oldCurPos;
    } else {
        MoveCursorTo(syswk, pos);
    }
    syswk->app->oldCurPos = pos;
}

u32 Box2Ui_UpdateCursor15(Box2SysWork *syswk) {
    u8 prev = CursorMove_GetPos(syswk->app->cursorMove);
    u32 res = CursorMove_Update(syswk->app->cursorMove);

    if (Box2Frame_IsMenuOnScreen(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 34 && res <= 35) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            MoveCursorTo(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (res == 33) {
            CursorMove_SetPos(syswk->app->cursorMove, prev);
            return 33;
        }
    } else if (res == 30 || res == 33) {
        CursorMove_SetPos(syswk->app->cursorMove, prev);
        MoveCursorTo(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            Box2Main_ShowCursor(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            Box2Main_ShowCursor(syswk);
            return 32;
        }
    }
    return res;
}

static void Cursor15_Move(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    Box2Ui_StartCursorMove(syswk->app, pos, prevPos);
    Box2Main_VFuncReq(syswk->app, Box2Main_VFuncCursorMove);
}

static void Cursor15_Touch(void *work, int pos, int prevPos) {
    if (pos != 33) {
        MoveCursorTo(work, pos);
    }
}

void Box2Ui_StartCursorMove(Box2AppWork *app, u32 pos, u32 curPos) {
    const CursorMoveData *from = CursorMove_GetData(app->cursorMove, pos);
    const CursorMoveData *to = CursorMove_GetData(app->cursorMove, curPos);

    StartCursorMoveWork(app, from->x, from->y, to->x, to->y);
}

static void StartCursorMoveWork(Box2AppWork *app, u8 fromX, u8 fromY, u8 toX, u8 toY) {
    Box2CursorMoveWork *mv = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2CursorMoveWork), FALSE, "box2_ui.c", 2600);

    mv->cnt = 4;
    mv->px = fromX;
    mv->py = fromY;
    if (fromX >= toX) {
        mv->vx = fromX - toX;
        mv->mx = 0;
    } else {
        mv->vx = toX - fromX;
        mv->mx = 1;
    }
    if (fromY >= toY) {
        mv->vy = fromY - toY;
        mv->my = 0;
    } else {
        mv->vy = toY - fromY;
        mv->my = 1;
    }
    mv->vx = (mv->vx << 8) / mv->cnt >> 8;
    mv->vy = (mv->vy << 8) / mv->cnt >> 8;
    app->vfunk.work = mv;
}

static u32 FindTriggeredRect(const CursorMoveData *data, u32 count) {
    TouchRect rects[2];
    u32 i;

    rects[1] = sTouchRectEnd;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (GFL_HIDGetTouchPressedRect(rects) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

static u32 FindHeldRect(const CursorMoveData *data, u32 count) {
    TouchRect rects[2];
    u32 i;

    rects[1] = sTouchRectEnd;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (GFL_HIDGetTouchHeldRect(rects) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

static u32 FindRectAt(const CursorMoveData *data, u32 count, u32 x, u32 y) {
    TouchRect rects[2];
    u32 i;

    rects[1] = sTouchRectEnd;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (GFL_HIDGetTouchRectAt(rects, x, y) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

u32 Box2Ui_GetTrayPokeTrigger(void) {
    return FindTriggeredRect(sTrayCursorData[0], BOX2_TRAY_POKE_MAX);
}

u32 Box2Ui_GetTrayPokeHeld(void) {
    return FindHeldRect(sTrayCursorData[0], BOX2_TRAY_POKE_MAX);
}

u32 Box2Ui_GetTrayPokeAt(u32 x, u32 y) {
    return FindRectAt(sTrayCursorData[0], BOX2_TRAY_POKE_MAX, x, y);
}

u32 Box2Ui_GetMenuButtonTrigger(void) {
    return FindTriggeredRect(sCursorData5, 6);
}

u32 Box2Ui_GetPartyPokeTrigger(void) {
    return FindTriggeredRect(&sTrayCursorData[2][30], 6);
}

u32 Box2Ui_GetPartyPokeHeld(void) {
    return FindHeldRect(&sTrayCursorData[2][30], 6);
}

u32 Box2Ui_GetTrayArrowHeld(void) {
    return FindHeldRect(&sTrayCursorData[0][31], 2);
}

u32 Box2Ui_GetBoxListArrowHeld(void) {
    return GFL_HIDGetTouchHeldRect(sTouchRects_78e4);
}

BOOL Box2Ui_GetBoxListScrollTouch(u32 *x, u32 *y) {
    if (GFL_HIDGetTouchHeldRect(sTouchRects_78f0) != TOUCH_RECT_NONE) {
        GFL_HIDGetTouchHeldPoint(x, y);
        return TRUE;
    }
    return FALSE;
}

BOOL Box2Ui_GetBoxListTouch(u32 *x, u32 *y) {
    if (GFL_HIDGetTouchHeldRect(sTouchRects_78cc) != TOUCH_RECT_NONE) {
        GFL_HIDGetTouchHeldPoint(x, y);
        return TRUE;
    }
    return FALSE;
}

int Box2Ui_GetBoxListRowAt(u32 x, u32 y) {
    int res = GFL_HIDGetTouchRectAt(sTouchRects_79fc, x, y);

    if (res == TOUCH_RECT_NONE) {
        return TOUCH_RECT_NONE;
    }
    return res;
}

u32 Box2Ui_GetBoxListPosAt(u32 x, u32 y) {
    u32 res = FindRectAt(&sTrayCursorData[1][34], 4, x, y);

    if (res != CURSORMOVE_NONE) {
        return res + BOX2_BOXLIST_POS;
    }
    return BOX2_GET_NONE;
}

BOOL Box2Ui_IsPartyLeftHeld(void) {
    if (GFL_HIDGetTouchHeldRect(sTouchRects_78d4) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}

BOOL Box2Ui_IsPartyRightHeld(void) {
    if (GFL_HIDGetTouchHeldRect(sTouchRects_78dc) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}

u32 Box2Ui_GetSwitchButtonHeld(void) {
    return FindHeldRect(&sTrayCursorData[0][33], 1);
}

BOOL Box2Ui_IsTrayHeld(void) {
    if (GFL_HIDGetTouchHeldRect(sTouchRects_78c4) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}
