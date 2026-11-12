// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_ui.c, ported by tools/port_file.py.
#include "types.h"
#include "app/box2_ui.h"
#include "app/box2_bgwfrm.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/cursor_move.h"



// The PC box's cursor: the positions it moves between in each of the box's modes and what moving it and touching a
// position do, and the touch screen's areas. The ROM names this file in its asserts. Names are ours

// A cursor's positions and callbacks
// Black 2's, not checked against Black
typedef struct {
    const CursorMoveData *data;
    const CursorMoveCallbacks *callbacks;
} Box2CursorMoveTable;

static void func_ov170_021ee408(Box2SysWork *syswk, u32 pos);
static void func_ov170_021ee418(void *work, int pos, int prevPos);
static void func_ov170_021ee42c(void *work, int pos, int prevPos);
static void func_ov170_021ee43c(void *work, int pos, int prevPos);
static void func_ov170_021ee454(void *work, int pos, int prevPos);
static void func_ov170_021ee470(void *work, int pos, int prevPos);
static void func_ov170_021ee510(void *work, int pos, int prevPos);
static void func_ov170_021ee64c(void *work, int pos, int prevPos);
static void func_ov170_021ee72c(void *work, int pos, int prevPos);
static void func_ov170_021ee744(void *work, int pos, int prevPos);
void func_ov170_021ee7c4(void *work, int pos, int prevPos);
void func_ov170_021ee7f4(void *work, int pos, int prevPos);
static void func_ov170_021ee944(void *work, int pos, int prevPos);
static void func_ov170_021ee95c(void *work, int pos, int prevPos);
static void func_ov170_021ee9c4(void *work, int pos, int prevPos);
void func_ov170_021ee9f4(void *work, int pos, int prevPos);
static void func_ov170_021eeb18(void *work, int pos, int prevPos);
static void func_ov170_021eeb30(void *work, int pos, int prevPos);
static void func_ov170_021eebdc(void *work, int pos, int prevPos);
static void func_ov170_021eebf4(void *work, int pos, int prevPos);
static void func_ov170_021eec04(void *work, int pos, int prevPos);
static void func_ov170_021eec1c(void *work, int pos, int prevPos);
static void func_ov170_021eec38(void *work, int pos, int prevPos);
static void func_ov170_021eec50(void *work, int pos, int prevPos);
static void func_ov170_021eec6c(void *work, int pos, int prevPos);
static void func_ov170_021eecb0(void *work, int pos, int prevPos);
static void func_ov170_021eedb8(void *work, int pos, int prevPos);
static void func_ov170_021eedd0(void *work, int pos, int prevPos);
static void func_ov170_021eee7c(void *work, int pos, int prevPos);
static void func_ov170_021eee94(void *work, int pos, int prevPos);
static void func_ov170_021eef50(void *work, int pos, int prevPos);
static void func_ov170_021eef68(void *work, int pos, int prevPos);
static void func_ov170_021eefa4(Box2AppWork *app, u8 fromX, u8 fromY, u8 toX, u8 toY);
static u32 func_ov170_021ef048(const CursorMoveData *data, u32 count);
static u32 func_ov170_021ef0a8(const CursorMoveData *data, u32 count);
static u32 func_ov170_021ef108(const CursorMoveData *data, u32 count, u32 x, u32 y);

// MWCC sorts a file's data by size, in an order that depends on where each object is declared; the objects of the same
// size are declared in the order that lays them out as in the ROM (tools/decomp/rodata_order.py)

// The end of a table of touch rectangles
static const TouchRect data_ov170_021f0040[3] = {
    { 0, 21, 173, 255 }, { 148, 167, 173, 255 }, { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov170_021f0024[2] = {
    { 0, 191, 168, 255 }, { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov170_021f0020 = { TOUCH_RECT_END, 0, 0, 0 };

static const TouchRect data_ov170_021f0034[3] = {
    { 0, 167, 228, 255 },
    { 0, 167, 168, 195 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const TouchRect data_ov170_021f002c[2] = {
    { 48, 167, 16, 103 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's sCursorCallbacks12
static const CursorMoveCallbacks data_ov170_021f00ac = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eec6c, func_ov170_021eecb0,
};

// pokebw2's sCursorCallbacks10
static const CursorMoveCallbacks data_ov170_021f00ec = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eec04, func_ov170_021eec1c,
};

// pokebw2's sCursorCallbacks13
static const CursorMoveCallbacks data_ov170_021f007c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eedb8, func_ov170_021eedd0,
};

// pokebw2's sCursorCallbacks0
static const CursorMoveCallbacks data_ov170_021f009c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee43c, func_ov170_021ee510,
};

// pokebw2's sCursorCallbacks7
static const CursorMoveCallbacks data_ov170_021f00fc = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eeb18, func_ov170_021eeb30,
};

// pokebw2's sCursorCallbacks15
static const CursorMoveCallbacks data_ov170_021f00bc = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eef50, func_ov170_021eef68,
};

// pokebw2's sCursorCallbacks9
static const CursorMoveCallbacks data_ov170_021f00cc = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee43c, func_ov170_021ee454,
};

// pokebw2's sCursorCallbacks3
static const CursorMoveCallbacks data_ov170_021f004c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee72c, func_ov170_021ee744,
};

// pokebw2's sCursorCallbacks14
static const CursorMoveCallbacks data_ov170_021f005c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eee7c, func_ov170_021eee94,
};

// pokebw2's sCursorCallbacks4
static const CursorMoveCallbacks data_ov170_021f006c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee7c4, func_ov170_021ee7f4,
};

// pokebw2's sCursorCallbacks1
static const CursorMoveCallbacks data_ov170_021f011c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee470, func_ov170_021ee470,
};

// pokebw2's sCursorCallbacks11
static const CursorMoveCallbacks data_ov170_021f013c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eec38, func_ov170_021eec50,
};

// pokebw2's sCursorCallbacks8
static const CursorMoveCallbacks data_ov170_021f012c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021eebdc, func_ov170_021eebf4,
};

// pokebw2's sCursorCallbacks5
static const CursorMoveCallbacks data_ov170_021f008c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee944, func_ov170_021ee95c,
};

// pokebw2's sCursorCallbacks6
static const CursorMoveCallbacks data_ov170_021f010c = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee9c4, func_ov170_021ee9f4,
};

// pokebw2's sCursorCallbacks2
static const CursorMoveCallbacks data_ov170_021f00dc = {
    func_ov170_021ee418, func_ov170_021ee42c, func_ov170_021ee43c, func_ov170_021ee64c,
};

// pokebw2's sCursorData10
static const CursorMoveData data_ov170_021f014c[5] = {
#include "asset/box2_ui_data_ov170_021f014c.inc"
};

// pokebw2's sCursorData1
static const CursorMoveData data_ov170_021f0188[6] = {
#include "asset/box2_ui_data_ov170_021f0188.inc"
};

// pokebw2's sCursorData11
static const CursorMoveData data_ov170_021f01d0[7] = {
#include "asset/box2_ui_data_ov170_021f01d0.inc"
};

// pokebw2's sCursorData12
static const CursorMoveData data_ov170_021f0224[8] = {
#include "asset/box2_ui_data_ov170_021f0224.inc"
};

// pokebw2's sCursorData9
static const CursorMoveData data_ov170_021f0284[9] = {
#include "asset/box2_ui_data_ov170_021f0284.inc"
};

// pokebw2's sCursorData0
static const CursorMoveData data_ov170_021f0370[14];
// pokebw2's sCursorData8
static const CursorMoveData data_ov170_021f0418[14];
// pokebw2's sCursorData14
static const CursorMoveData data_ov170_021f04c0[15];
// pokebw2's sCursorData5
static const CursorMoveData data_ov170_021f0574[17];
// pokebw2's sCursorData15
static const CursorMoveData data_ov170_021f0640[37];
// pokebw2's sCursorData7
static const CursorMoveData data_ov170_021f07fc[41];
// pokebw2's sCursorData2
static const CursorMoveData data_ov170_021f09e8[41];
// pokebw2's sCursorData13
static const CursorMoveData data_ov170_021f0bd4[42];
// pokebw2's sCursorData3
static const CursorMoveData data_ov170_021f0dcc[44];
// pokebw2's sCursorData4
static const CursorMoveData data_ov170_021f0fdc[44];
// pokebw2's sCursorData6
static const CursorMoveData data_ov170_021f11ec[44];

// pokebw2's sCursorMoveTables
static const Box2CursorMoveTable data_ov170_021f02f0[16] = {
    { data_ov170_021f0370, &data_ov170_021f009c },
    { data_ov170_021f0188, &data_ov170_021f011c },
    { data_ov170_021f09e8, &data_ov170_021f00dc },
    { data_ov170_021f0dcc, &data_ov170_021f004c },
    { data_ov170_021f0fdc, &data_ov170_021f006c },
    { data_ov170_021f0574, &data_ov170_021f008c },
    { data_ov170_021f11ec, &data_ov170_021f010c },
    { data_ov170_021f07fc, &data_ov170_021f00fc },
    { data_ov170_021f0418, &data_ov170_021f012c },
    { data_ov170_021f0284, &data_ov170_021f00cc },
    { data_ov170_021f014c, &data_ov170_021f00ec },
    { data_ov170_021f01d0, &data_ov170_021f013c },
    { data_ov170_021f0224, &data_ov170_021f00ac },
    { data_ov170_021f0bd4, &data_ov170_021f007c },
    { data_ov170_021f04c0, &data_ov170_021f005c },
    { data_ov170_021f0640, &data_ov170_021f00bc },
};

static const CursorMoveData data_ov170_021f0370[14] = {
#include "asset/box2_ui_data_ov170_021f0370.inc"
};

static const CursorMoveData data_ov170_021f0418[14] = {
#include "asset/box2_ui_data_ov170_021f0418.inc"
};

static const CursorMoveData data_ov170_021f04c0[15] = {
#include "asset/box2_ui_data_ov170_021f04c0.inc"
};

static const CursorMoveData data_ov170_021f0574[17] = {
#include "asset/box2_ui_data_ov170_021f0574.inc"
};

static const CursorMoveData data_ov170_021f0640[37] = {
#include "asset/box2_ui_data_ov170_021f0640.inc"
};

static const CursorMoveData data_ov170_021f07fc[41] = {
#include "asset/box2_ui_data_ov170_021f07fc.inc"
};

static const CursorMoveData data_ov170_021f09e8[41] = {
#include "asset/box2_ui_data_ov170_021f09e8.inc"
};

static const CursorMoveData data_ov170_021f0bd4[42] = {
#include "asset/box2_ui_data_ov170_021f0bd4.inc"
};

static const CursorMoveData data_ov170_021f0dcc[44] = {
#include "asset/box2_ui_data_ov170_021f0dcc.inc"
};

static const CursorMoveData data_ov170_021f0fdc[44] = {
#include "asset/box2_ui_data_ov170_021f0fdc.inc"
};

static const CursorMoveData data_ov170_021f11ec[44] = {
#include "asset/box2_ui_data_ov170_021f11ec.inc"
};

void func_ov170_021ee2c4(Box2SysWork *syswk) {
    BOOL visible;
    u32 id;

    visible = func_020355c4() == TRUE ? FALSE : TRUE;
    switch (syswk->param->mode) {
    case 0:
        id = 0;
        break;
    case 1:
        id = 2;
        break;
    case 2:
        id = 3;
        break;
    case 3:
        id = 7;
        break;
    case 4:
        id = 13;
        break;
    case 5:
        id = 15;
        break;
    }
    func_ov170_021ee3cc(syswk);
    syswk->app->cursorMove = func_02025cd0(data_ov170_021f02f0[id].data, data_ov170_021f02f0[id].callbacks, syswk,
                                           visible, syswk->curRcvPos, HEAPID_BOX2_APP);
    func_02025d1c(syswk->app->cursorMove);
    func_ov170_021ee408(syswk, syswk->curRcvPos);
    func_ov170_021ed55c(syswk, visible);
    syswk->app->curPos = syswk->curRcvPos;
}

void func_ov170_021ee364(Box2SysWork *syswk, u32 id, u32 pos) {
    BOOL visible = func_020260f0(syswk->app->cursorMove);

    func_ov170_021ee3cc(syswk);
    syswk->app->cursorMove = func_02025cd0(data_ov170_021f02f0[id].data, data_ov170_021f02f0[id].callbacks, syswk,
                                           visible, pos, HEAPID_BOX2_APP);
    func_02025d1c(syswk->app->cursorMove);
    func_ov170_021ee408(syswk, pos);
    func_ov170_021ed55c(syswk, visible);
    syswk->app->curPos = pos;
}

void func_ov170_021ee3cc(Box2SysWork *syswk) {
    if (syswk->app->cursorMove != NULL) {
        func_02025d14(syswk->app->cursorMove);
        syswk->app->cursorMove = NULL;
    }
}

void func_ov170_021ee3e4(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *data = func_0202616c(syswk->app->cursorMove, pos);

    func_ov170_021ec08c(syswk->app, BOX2_ACTOR_CURSOR, data->x, data->y, 0);
}

static void func_ov170_021ee408(Box2SysWork *syswk, u32 pos) {
    func_ov170_021ee3e4(syswk, pos);
    func_ov170_021ecc1c(syswk);
}

static void func_ov170_021ee418(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021ee408(syswk, pos);
    func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, TRUE);
}

static void func_ov170_021ee42c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021ec000(syswk->app, BOX2_ACTOR_CURSOR, FALSE);
}

static void func_ov170_021ee43c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021ee454(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_020260e4(syswk->app->cursorMove, pos);
    func_ov170_021ee408(syswk, pos);
}

static void func_ov170_021ee470(void *work, int pos, int prevPos) {
}

u32 func_ov170_021ee474(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 8 && res <= 12) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 6;
        }
        if (res == 6) {
            func_020355d8(TRUE);
            return 6;
        }
        if (res == 7) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 7;
        }
    } else if (res == 6 || res == 7) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void func_ov170_021ee510(void *work, int pos, int prevPos) {
    if (pos != 6 && pos != 7) {
        func_ov170_021ee408(work, pos);
    }
}

void func_ov170_021ee520(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *data = func_0202616c(syswk->app->cursorMove, pos);

    func_ov170_021eefa4(syswk->app, 84, 16, data->x, data->y);
}

void func_ov170_021ee544(Box2SysWork *syswk, u32 pos) {
    const CursorMoveData *to = data_ov170_021f0188;
    const CursorMoveData *data;

    if (pos < BOX2_TRAY_POKE_MAX) {
        data = &data_ov170_021f0dcc[pos];
    } else {
        data = &data_ov170_021f0370[pos - BOX2_TRAY_POKE_MAX];
    }
    func_ov170_021eefa4(syswk->app, data->x, data->y, to->x, to->y);
}

u32 func_ov170_021ee57c(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 35 && res <= 39) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 33;
        }
        if (res == 33) {
            func_020355d8(TRUE);
            return 33;
        }
        if (res == 34) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 34;
        }
    } else if (res == 33 || res == 34) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}

static void func_ov170_021ee64c(void *work, int pos, int prevPos) {
    if (pos != 33 && pos != 34) {
        func_ov170_021ee408(work, pos);
    }
}

u32 func_ov170_021ee65c(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 36 && res <= 41) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 34;
        }
        if (res == 34) {
            func_020355d8(TRUE);
            return 34;
        }
        if (res == 35) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if ((res - 33) <= 2) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}

static void func_ov170_021ee72c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021ee744(void *work, int pos, int prevPos) {
    if (pos != 0x22 && pos != 0x23) {
        func_ov170_021ee408(work, pos);
    }
}

void func_ov170_021ee754(Box2SysWork *syswk, u32 pos) {
    u8 button1;

    button1 = 0;
    u8 button3 = 0;
    if (syswk->unk18 == 1) {
        button1 = 2;
        if (syswk->param->mode == 3) {
            button3 = 1;
        }
    } else if (syswk->param->mode == 3) {
        button3 = 1;
    } else {
        if (pos >= BOX2_PARTY_POS) {
            if (func_ov170_021ef45c(syswk->app->bgWinFrame) == FALSE || func_ov170_021dd25c(syswk, pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
                button3 = 2;
            }
        } else if (func_ov170_021dd25c(syswk, pos, syswk->tray, PKM_PARAM_SPECIES_VALID, NULL) == 0) {
            button3 = 2;
        }
    }
    func_ov170_021ee038(syswk, button1, 1, button3);
}


void func_ov170_021ee7c4(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021ee754(syswk, pos);
    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
    syswk->app->curPos = pos;
}

void func_ov170_021ee7f4(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 0x1f || pos == 0x20) {
        func_020260e4(syswk->app->cursorMove, 0x1e);
        pos = 0x1e;
    } else if (pos == 0x21 || pos == 0x26 || pos == 0x28 || pos == 0x29 || pos == 0x2a) {
        func_020260e4(syswk->app->cursorMove, syswk->app->curPos);
        pos = syswk->app->curPos;
    } else {
        func_ov170_021ee408(syswk, pos);
    }
    func_ov170_021ee754(syswk, pos);
    syswk->app->curPos = pos;
}

u32 func_ov170_021ee850(Box2SysWork *syswk) {
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            func_ov170_021dd238(syswk);
            return 40;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}



u32 func_ov170_021ee8a8(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 9 && res <= 14) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 7;
        }
        if (res == 7) {
            func_020355d8(TRUE);
            return 7;
        }
        if (res == 8) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if ((res - 6) <= 2) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void func_ov170_021ee944(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021ee95c(void *work, int pos, int prevPos) {
    if (pos != 7 && pos != 8)
        func_ov170_021ee408(work, pos);
}

u32 func_ov170_021ee96c(Box2SysWork *syswk) {
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) {
            func_ov170_021dd238(syswk);
            return 40;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 37;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 38;
        }
    }
    return res;
}


void func_ov170_021ee9c4(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021ee754(syswk, pos);
    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
    syswk->app->curPos = pos;
}

void func_ov170_021ee9f4(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 0x25 || pos == 0x26) {
        func_020260e4(syswk->app->cursorMove, 0x24);
        pos = 0x24;
    } else if (pos == 0x28 || pos == 0x29 || pos == 0x2a) {
        func_020260e4(syswk->app->cursorMove, syswk->app->curPos);
        pos = syswk->app->curPos;
    } else {
        func_ov170_021ee408(syswk, pos);
    }
    func_ov170_021ee754(syswk, pos);
    syswk->app->curPos = pos;
}


u32 func_ov170_021eea48(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res == 36 || res == 37 || res == 38) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 34;
        }
        if (res == 34) {
            func_020355d8(TRUE);
            return 34;
        }
        if (res == 35) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if (res == 33 || res == 34 || res == 35) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}

static void func_ov170_021eeb18(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eeb30(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos != 34 && pos != 35) {
        func_ov170_021ee408(syswk, pos);
    }
}

u32 func_ov170_021eeb40(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res == 9 || res == 10 || res == 11) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 7;
        }
        if (res == 7) {
            func_020355d8(TRUE);
            return 7;
        }
        if (res == 8) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if (res == 6 || res == 7 || res == 8) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void func_ov170_021eebdc(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eebf4(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos != 7 && pos != 8) {
        func_ov170_021ee408(syswk, pos);
    }
}

static void func_ov170_021eec04(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eec1c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_020260e4(syswk->app->cursorMove, pos);
    func_ov170_021ee408(syswk, pos);
}

static void func_ov170_021eec38(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eec50(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_020260e4(syswk->app->cursorMove, pos);
    func_ov170_021ee408(syswk, pos);
}

static void func_ov170_021eec6c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos >= 1 && pos <= 4) {
        func_ov170_021ee008(syswk, pos - 1, TRUE);
    } else {
        func_ov170_021ee008(syswk, 0, FALSE);
    }
    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
    syswk->app->curPos = pos;
}

void func_ov170_021eecb0(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    if (pos == 0 || pos == 5 || pos == 6) {
        func_020260e4(syswk->app->cursorMove, syswk->app->curPos);
        pos = syswk->app->curPos;
    } else {
        func_ov170_021ee408(syswk, pos);
    }
    syswk->app->curPos = pos;
}

u32 func_ov170_021eece4(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 36 && res <= 39) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 34;
        }
        if (res == 34) {
            func_020355d8(TRUE);
            return 34;
        }
        if (res == 35) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 35;
        }
    } else if (res == 30 || res - 33 <= 2) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}

static void func_ov170_021eedb8(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eedd0(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;
    if (pos != 0x22 && pos != 0x23) {
        func_ov170_021ee408(syswk, pos);
    }
}

u32 func_ov170_021eede0(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 9 && res <= 12) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
            func_020355d8(FALSE);
            return 7;
        }
        if (res == 7) {
            func_020355d8(TRUE);
            return 7;
        }
        if (res == 8) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 8;
        }
    } else if ((res - 6) <= 2) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    return res;
}

static void func_ov170_021eee7c(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eee94(void *work, int pos, int prevPos) {
    if (pos != 7 && pos != 8) {
        func_ov170_021ee408(work, pos);
    }
}

u32 func_ov170_021eeea4(Box2SysWork *syswk) {
    u8 prev = func_020260e0(syswk->app->cursorMove);
    u32 res = func_02025de8(syswk->app->cursorMove);

    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == FALSE) {
        if (res >= 34 && res <= 35) {
            func_020260e4(syswk->app->cursorMove, prev);
            func_ov170_021ee408(syswk, prev);
            return CURSORMOVE_NONE;
        }
        if (res == 33) {
            func_020260e4(syswk->app->cursorMove, prev);
            return 33;
        }
    } else if (res == 30 || res == 33) {
        func_020260e4(syswk->app->cursorMove, prev);
        func_ov170_021ee408(syswk, prev);
        return CURSORMOVE_NONE;
    }
    if (res == CURSORMOVE_NONE) {
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
            func_ov170_021dd238(syswk);
            return 31;
        }
        if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
            func_ov170_021dd238(syswk);
            return 32;
        }
    }
    return res;
}

static void func_ov170_021eef50(void *work, int pos, int prevPos) {
    Box2SysWork *syswk = work;

    func_ov170_021eef74(syswk->app, pos, prevPos);
    func_ov170_021dc9b4(syswk->app, func_ov170_021e066c);
}

static void func_ov170_021eef68(void *work, int pos, int prevPos) {
    if (pos != 33) {
        func_ov170_021ee408(work, pos);
    }
}

void func_ov170_021eef74(Box2AppWork *app, u32 pos, u32 curPos) {
    const CursorMoveData *from = func_0202616c(app->cursorMove, pos);
    const CursorMoveData *to = func_0202616c(app->cursorMove, curPos);

    func_ov170_021eefa4(app, from->x, from->y, to->x, to->y);
}

static void func_ov170_021eefa4(Box2AppWork *app, u8 fromX, u8 fromY, u8 toX, u8 toY) {
    Box2CursorMoveWork *mv = GFL_HeapAllocate(HEAPID_BOX2_APP, sizeof(Box2CursorMoveWork), FALSE, "box2_ui.c", 2447);

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

static u32 func_ov170_021ef048(const CursorMoveData *data, u32 count) {
    TouchRect rects[2];
    u32 i;

    rects[1] = data_ov170_021f0020;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (func_02035c08(rects) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

static u32 func_ov170_021ef0a8(const CursorMoveData *data, u32 count) {
    TouchRect rects[2];
    u32 i;

    rects[1] = data_ov170_021f0020;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (func_02035ba4(rects) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

static u32 func_ov170_021ef108(const CursorMoveData *data, u32 count, u32 x, u32 y) {
    TouchRect rects[2];
    u32 i;

    rects[1] = data_ov170_021f0020;
    for (i = 0; i < count; i++) {
        rects[0] = data[i].rect;
        if (func_02035d40(rects, x, y) != TOUCH_RECT_NONE) {
            return i;
        }
    }
    return CURSORMOVE_NONE;
}

u32 func_ov170_021ef170(void) {
    return func_ov170_021ef048(data_ov170_021f0dcc, BOX2_TRAY_POKE_MAX);
}


u32 func_ov170_021ef180(u32 x, u32 y) {
    return func_ov170_021ef108(data_ov170_021f0dcc, BOX2_TRAY_POKE_MAX, x, y);
}

u32 func_ov170_021ef194(void)
{
    return func_ov170_021ef048(data_ov170_021f0574, 6);
}

u32 func_ov170_021ef1a4(void)
{
    return func_ov170_021ef048(&data_ov170_021f11ec[30], 6);
}






u32 func_ov170_021ef1b4(void) {
    return func_ov170_021ef0a8(&data_ov170_021f0dcc[31], 2);
}

s32 func_ov170_021ef1c4(void) {
    return func_02035ba4(data_ov170_021f0040);
}

BOOL func_ov170_021ef1d4(u32 *x, u32 *y)
{
    if (func_02035ba4(data_ov170_021f0034) != -1)
    {
        func_02035cb8(x, y);
        return TRUE;
    }
    return FALSE;
}

u32 func_ov170_021ef1fc(u32 *param_1, u32 *param_2) {
    if (func_02035ba4(data_ov170_021f0024) != -1) {
        func_02035cb8(param_1, param_2);
        return 1;
    }
    return 0;
}



u32 func_ov170_021ef224(u32 x, u32 y) {
    u32 res = func_ov170_021ef108(&data_ov170_021f0fdc[34], 4, x, y);

    if (res != CURSORMOVE_NONE) {
        return res + BOX2_BOXLIST_POS;
    }
    return BOX2_GET_NONE;
}

BOOL func_ov170_021ef248(void) {
    if (func_02035ba4(data_ov170_021f002c) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}




u32 func_ov170_021ef264(void) {
    return func_ov170_021ef0a8(&data_ov170_021f0dcc[33], 1);
}

