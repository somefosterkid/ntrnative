// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_bgwfrm.c, ported by tools/port_file.py.
#include "types.h"
#include "app/box2.h"
#include "app/box2_bgwfrm.h"
#include "app/box2_bmp.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "constants/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "system/bgwinfrm.h"

// The PC box's frames of BG screen data that slide in and out (bgwinfrm.c): the party, the box menu and the
// marking frame. The ROM doesn't name this file; box2_bgwfrm.c is a guess after box2_main.c. None of these functions
// has a name yet

static void func_ov170_021ef31c(BGWinFrame *frames, u32 index, u32 datId);
static void func_ov170_021ef4c0(BGWinFrame *frames);
static void func_ov170_021ef630(Box2SysWork *syswk);

void func_ov170_021ef274(Box2SysWork *syswk) {
    Box2AppWork *app = syswk->app;

    app->bgWinFrame = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, 12, HEAPID_BOX2_APP);
    func_ov170_021eb744(app);
    func_ov170_021ef630(syswk);
    func_ov170_021eb79c(syswk);
    func_ov170_021eb7c8(syswk);

    func_0202ca3c(app->bgWinFrame, 8, 1, 11, 15);
    if (syswk->param->mode == 4) {
        func_ov170_021ef31c(app->bgWinFrame, 8, 8);
    } else {
        func_ov170_021ef31c(app->bgWinFrame, 8, 9);
    }

    func_0202ca3c(app->bgWinFrame, 7, 1, 11, 15);
    func_ov170_021ef31c(app->bgWinFrame, 7, 10);
    func_ov170_021eb95c(syswk);

    func_0202ca3c(app->bgWinFrame, 9, 1, 11, 21);
    func_ov170_021ef31c(app->bgWinFrame, 9, 11);

    func_ov170_021ef4c0(app->bgWinFrame);
}

void func_ov170_021ef310(Box2AppWork *app) {
    func_0202ca0c(app->bgWinFrame);
}

static void func_ov170_021ef31c(BGWinFrame *frames, u32 index, u32 datId) {
    func_0202cae4(frames, index, 0x74, datId, TRUE);
}

void func_ov170_021ef32c(BGWinFrame *frames) {
    func_0202cb4c(frames, 8, 2, 6);
}

void func_ov170_021ef33c(BGWinFrame *frames) {
    func_0202cb4c(frames, 8, 21, 6);
}

void func_ov170_021ef34c(BGWinFrame *frames) {
    func_0202cb4c(frames, 8, 2, 24);
}

void func_ov170_021ef35c(BGWinFrame *frames) {
    func_0202cb4c(frames, 8, 21, 24);
}

void func_ov170_021ef36c(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 8, &x, &y);
    if (y != 6) {
        func_0202cd64(frames, 8, 0, -1, y - 6);
    }
}

void func_ov170_021ef3a0(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 8, &x, &y);
    if (y != 24) {
        func_0202cd64(frames, 8, 0, 1, 24 - y);
    }
}

void func_ov170_021ef3d8(BGWinFrame *frames) {
    func_0202cd64(frames, 8, 1, 0, 19);
}

void func_ov170_021ef3ec(BGWinFrame *frames) {
    func_0202cd64(frames, 8, -1, 0, 19);
}

BOOL func_ov170_021ef400(Box2SysWork *syswk) {
    s8 prevX, prevY, x, y;
    BOOL moving;
    BOOL ret;

    func_0202cf98(syswk->app->bgWinFrame, 8, &prevX, &prevY);
    moving = func_0202cdd4(syswk->app->bgWinFrame, 8);
    func_0202cf98(syswk->app->bgWinFrame, 8, &x, &y);
    if (prevX != x || prevY != y) {
        func_ov170_021ec8e4(syswk);
    }
    ret = FALSE;
    if (moving) {
        ret = TRUE;
    }
    return ret;
}

BOOL func_ov170_021ef45c(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 8, &x, &y);
    if (x == 21 && y == 6) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov170_021ef480(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 8, &x, &y);
    if (x == 2 && y == 6) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov170_021ef4a4(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 8, &x, &y);
    if (y == 6) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov170_021ef4c0(BGWinFrame *frames) {
    u32 i;

    for (i = 0; i < 6; i++) {
        func_0202cb4c(frames, i, 32, i * 3 + 2);
    }
}

void func_ov170_021ef4e4(BGWinFrame *frames) {
    u32 i;

    for (i = 0; i < 6; i++) {
        func_0202cb4c(frames, i, 21, i * 3 + 2);
    }
}

void func_ov170_021ef508(BGWinFrame *frames) {
    u32 i;

    for (i = 0; i < 6; i++) {
        func_0202cc70(frames, i);
        func_0202cb4c(frames, i, 32, i * 3 + 2);
    }
}

void func_ov170_021ef534(BGWinFrame *frames) {
    s8 x, y;
    u16 i = 0;

    func_0202cf98(frames, 0, &x, &y);
    if (x != 21) {
        for (; i < 6; i++) {
            func_0202cd64(frames, i, -1, 0, x - 21);
        }
    }
}

void func_ov170_021ef57c(BGWinFrame *frames) {
    s8 x, y;
    u16 i = 0;

    func_0202cf98(frames, 0, &x, &y);
    if (x != 32) {
        for (; i < 6; i++) {
            func_0202cd64(frames, i, 1, 0, 32 - x);
        }
    }
}

BOOL func_ov170_021ef5c4(BGWinFrame *frames) {
    BOOL moving = FALSE;
    u32 i;

    for (i = 0; i < 6; i++) {
        if (func_0202cdd4(frames, i) == TRUE) {
            moving = TRUE;
        }
    }
    return moving;
}

BOOL func_ov170_021ef5e8(BGWinFrame *frames) {
    s8 x, y;
    BOOL ret = FALSE;

    func_0202cf98(frames, 0, &x, &y);
    if (x != 32) {
        ret = TRUE;
    }
    return ret;
}

BOOL func_ov170_021ef60c(BGWinFrame *frames) {
    s8 x, y;
    BOOL ret = FALSE;

    func_0202cf98(frames, 0, &x, &y);
    if (x == 21) {
        ret = TRUE;
    }
    return ret;
}

static void func_ov170_021ef630(Box2SysWork *syswk) {
    u16 *screen = GFL_BGSysIsScrHeapExists(0);

    func_0202ca3c(syswk->app->bgWinFrame, 6, 0, 32, 3);
    func_0202cac4(syswk->app->bgWinFrame, 6, screen + 32 * 21);
}

void func_ov170_021ef660(BGWinFrame *frames) {
    func_0202cb4c(frames, 6, 0, 21);
}

void func_ov170_021ef670(Box2AppWork *app) {
    func_0202cb4c(app->bgWinFrame, 10, 0, 21);
}

void func_ov170_021ef680(Box2AppWork *app) {
    func_0202cc70(app->bgWinFrame, 10);
}

void func_ov170_021ef68c(Box2AppWork *app) {
    func_0202cb4c(app->bgWinFrame, 11, 0, 21);
}

void func_ov170_021ef69c(Box2AppWork *app) {
    func_0202cc70(app->bgWinFrame, 11);
}

void func_ov170_021ef6a8(BGWinFrame *frames) {
    func_0202cb4c(frames, 7, 21, 24);
    func_0202cd64(frames, 7, 0, -1, 19);
}

void func_ov170_021ef6cc(BGWinFrame *frames) {
    func_0202cd64(frames, 7, 0, 1, 19);
}

BOOL func_ov170_021ef6e0(Box2SysWork *syswk) {
    s8 prevX, prevY, x, y;
    BOOL moving;

    func_0202cf98(syswk->app->bgWinFrame, 7, &prevX, &prevY);
    moving = func_0202cdd4(syswk->app->bgWinFrame, 7);
    func_0202cf98(syswk->app->bgWinFrame, 7, &x, &y);
    if (prevX != x || prevY != y) {
        func_ov170_021ed8a4(syswk);
    }
    return moving;
}

void func_ov170_021ef738(BGWinFrame *frames) {
    func_0202cb4c(frames, 9, 32, 0);
    func_0202cd64(frames, 9, -1, 0, 11);
}

void func_ov170_021ef75c(BGWinFrame *frames) {
    func_0202cd64(frames, 9, 1, 0, 11);
}

BOOL func_ov170_021ef770(BGWinFrame *frames) {
    s8 x, y;

    func_0202cf98(frames, 9, &x, &y);
    if (x == 21) {
        return TRUE;
    }
    return FALSE;
}
