// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dpc_ui.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dpc_ui.h"
#include "app/dpc_main.h"
#include "app/dpc_obj.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"

// The Hall of Fame PC's input: the buttons, the keys and the Pokémon touched on the ring. The ROM doesn't name this
// file; the name is a guess, after box2_ui.c

static u32 func_ov171_021de960(DendouPCWork *wk);

// The buttons, in the order of DPCUI_*
// pokebw2's sDpcUiTouchRects
static const TouchRect data_ov171_021ded24[] = {
    { 168, 191, 8, 31 },    { 168, 191, 88, 111 },      { 168, 191, 200, 223 },
    { 168, 191, 232, 255 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's DpcUi_PageMain
u32 func_ov171_021de8a8(DendouPCWork *wk) {
    u32 ret = func_02035c08(data_ov171_021ded24);

    if (ret == TOUCH_RECT_NONE) {
        ret = func_ov171_021de960(wk);
        if (ret == DPCUI_NONE) {
            if (GCTX_HIDGetTypedKeys() & (PAD_KEY_LEFT | PAD_BUTTON_L)) {
                return DPCUI_LEFT;
            }
            if (GCTX_HIDGetTypedKeys() & (PAD_KEY_RIGHT | PAD_BUTTON_R)) {
                return DPCUI_RIGHT;
            }
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
                return DPCUI_DECIDE;
            }
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                return DPCUI_RETURN;
            }
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) {
                return DPCUI_CLOSE;
            }
            return DPCUI_NONE;
        }
    }
    return ret;
}

// pokebw2's DpcUi_PokeMain
u32 func_ov171_021de914(DendouPCWork *wk) {
    u32 ret = func_02035c08(data_ov171_021ded24);

    if (ret != DPCUI_RETURN) {
        ret = func_ov171_021de960(wk);
        if (ret == DPCUI_NONE) {
            if (GCTX_HIDGetTypedKeys() & (PAD_KEY_LEFT | PAD_BUTTON_L)) {
                return DPCUI_LEFT;
            }
            if (GCTX_HIDGetTypedKeys() & (PAD_KEY_RIGHT | PAD_BUTTON_R)) {
                return DPCUI_RIGHT;
            }
            if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
                return DPCUI_RETURN;
            }
            return DPCUI_NONE;
        }
    }
    return ret;
}

// The Pokémon touched this frame as a DPCUI_POKE, the one in front where several overlap
// pokebw2's DpcUi_HitPokemon
static u32 func_ov171_021de960(DendouPCWork *wk) {
    u32 tpX, tpY;
    u32 i;
    s16 base;
    s16 hit;
    s16 hitY;
    s16 x, y;
    s16 left, right, top, bottom;

    if (func_02035d20(&tpX, &tpY) == FALSE) {
        return DPCUI_NONE;
    }

    base = func_ov171_021de5f4(wk);
    hit = -1;
    for (i = base; i < base + HALL_OF_FAME_TEAM_SIZE; i++) {
        if (wk->actors[i] == NULL) {
            continue;
        }
        func_ov171_021de0e4(wk, i, &x, &y);
        left = x - 48;
        right = x + 48;
        top = y - 48;
        bottom = y + 48;
        if (left < 0) {
            left = 0;
        }
        if (right > 255) {
            right = 255;
        }
        if (top < 0) {
            top = 0;
        }
        if (bottom > 255) {
            bottom = 255;
        }
        if (tpX >= left && tpX < right && tpY >= top && tpY < bottom) {
            if (hit == -1) {
                hitY = y;
                hit = i - base;
            } else if (y > hitY) {
                hitY = y;
                hit = i - base;
            }
        }
    }

    if (hit != -1) {
        return DPCUI_POKE + hit % HALL_OF_FAME_TEAM_SIZE;
    }
    return DPCUI_NONE;
}
