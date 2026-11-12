// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/wipe_wnd.c, which is the same code in
// Black 2/White 2. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "types.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "system/wipe_wnd.h"

// The windows the screen wipes draw with: each window register write, made at once or kept in the WipeWnd and made
// by a task at the next VBlank. The file's name is a guess: the ROM has no string for it

static void func_02023d94(TCB *tcb, void *data);
static void func_02023da8(TCB *tcb, void *data);
static void func_02023dc4(TCB *tcb, void *data);
static void func_02023ddc(TCB *tcb, void *data);

// WipeWnd_SetVisible
void func_02023b48(u32 visible, u32 screen) {
    if (screen == 0) {
        GX_SetVisibleWnd(visible);
    } else {
        GXS_SetVisibleWnd(visible);
    }
}

// WipeWnd_SetInsidePlane
void func_02023b74(u32 planes, BOOL effect, u32 window, u32 screen) {
    if (window == 0) {
        if (screen == 0) {
            G2_SetWnd0InsidePlane(planes, effect);
        } else {
            G2S_SetWnd0InsidePlane(planes, effect);
        }
    } else {
        if (screen == 0) {
            G2_SetWnd1InsidePlane(planes, effect);
        } else {
            G2S_SetWnd1InsidePlane(planes, effect);
        }
    }
}

// WipeWnd_SetOutsidePlane
void func_02023bfc(u32 planes, BOOL effect, u32 screen) {
    if (screen == 0) {
        G2_SetWndOutsidePlane(planes, effect);
    } else {
        G2S_SetWndOutsidePlane(planes, effect);
    }
}

// Dead-stripped (pokebw2's): the planes inside a window and outside the windows, read back from the registers. Only
// wipe_sub.c's dead-stripped plane swap reads them, so the original link dropped both, and the ROM has neither.
// pokebw2 reconstructed them from HGSS's window module (pret's pokeheartgold, sub_020132E8 and sub_0201333C in
// asm/unk_02012DD8.s, between the counterparts of WipeWnd_SetOutsidePlane and WipeWnd_SetPosition)
GXWndPlane WipeWnd_GetInsidePlane(u32 window, u32 screen) {
    if (window == 0) {
        if (screen == 0) {
            return G2_GetWnd0InsidePlane();
        } else {
            return G2S_GetWnd0InsidePlane();
        }
    } else {
        if (screen == 0) {
            return G2_GetWnd1InsidePlane();
        } else {
            return G2S_GetWnd1InsidePlane();
        }
    }
}

GXWndPlane WipeWnd_GetOutsidePlane(u32 screen) {
    if (screen == 0) {
        return G2_GetWndOutsidePlane();
    } else {
        return G2S_GetWndOutsidePlane();
    }
}

// WipeWnd_SetPosition
void func_02023c3c(int x1, int y1, int x2, int y2, u32 window, u32 screen) {
    if (window == 0) {
        if (screen == 0) {
            G2_SetWnd0Position(x1, y1, x2, y2);
        } else {
            G2S_SetWnd0Position(x1, y1, x2, y2);
        }
    } else {
        if (screen == 0) {
            G2_SetWnd1Position(x1, y1, x2, y2);
        } else {
            G2S_SetWnd1Position(x1, y1, x2, y2);
        }
    }
}

// WipeWnd_SetVisibleAtVBlank
void func_02023cfc(WipeWnd *wnd, u32 visible, u32 screen) {
    WipeWndDisp *disp = &wnd->disp[screen];

    disp->visible = visible;
    disp->screen = screen;
    GFL_VBlankTCBAdd(func_02023d94, disp, 1);
}

// WipeWnd_SetInsidePlaneAtVBlank
void func_02023d18(WipeWnd *wnd, u32 planes, BOOL effect, u32 window, u32 screen) {
    WipeWndIn *in = &wnd->in[screen][window];

    in->planes = planes;
    in->effect = effect;
    in->window = window;
    in->screen = screen;
    GFL_VBlankTCBAdd(func_02023da8, in, 1);
}

// WipeWnd_SetOutsidePlaneAtVBlank
void func_02023d40(WipeWnd *wnd, u32 planes, BOOL effect, u32 screen) {
    WipeWndOut *out = &wnd->out[screen];

    out->planes = planes;
    out->effect = effect;
    out->screen = screen;
    GFL_VBlankTCBAdd(func_02023dc4, out, 1);
}

// WipeWnd_SetPositionAtVBlank
void func_02023d60(WipeWnd *wnd, int x1, int y1, int x2, int y2, u32 window, u32 screen) {
    WipeWndPos *pos = &wnd->pos[screen][window];

    pos->x1 = x1;
    pos->y1 = y1;
    pos->x2 = x2;
    pos->y2 = y2;
    pos->window = window;
    pos->screen = screen;
    GFL_VBlankTCBAdd(func_02023ddc, pos, 1);
}

// WipeWnd_SetVisibleTask
static void func_02023d94(TCB *tcb, void *data) {
    WipeWndDisp *disp = data;

    func_02023b48(disp->visible, disp->screen);
    GFL_TCBRemove(tcb);
}

// WipeWnd_SetInsidePlaneTask
static void func_02023da8(TCB *tcb, void *data) {
    WipeWndIn *in = data;

    func_02023b74(in->planes, in->effect, in->window, in->screen);
    GFL_TCBRemove(tcb);
}

// WipeWnd_SetOutsidePlaneTask
static void func_02023dc4(TCB *tcb, void *data) {
    WipeWndOut *out = data;

    func_02023bfc(out->planes, out->effect, out->screen);
    GFL_TCBRemove(tcb);
}

// WipeWnd_SetPositionTask
static void func_02023ddc(TCB *tcb, void *data) {
    WipeWndPos *pos = data;

    func_02023c3c(pos->x1, pos->y1, pos->x2, pos->y2, pos->window, pos->screen);
    GFL_TCBRemove(tcb);
}
