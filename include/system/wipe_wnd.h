// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/wipe_wnd.h.
#ifndef BWPORT_SYSTEM_WIPE_WND_H
#define BWPORT_SYSTEM_WIPE_WND_H

#include "types.h"
#include "nitro/gx.h"
#include "struct_decls.h"

// The windows the screen wipes draw with (wipe_wnd.c, a guessed name): window register writes, made at once or at the
// next VBlank

typedef struct {
    u32 visible;
    u32 screen;
} WipeWndDisp;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 window;
    u32 screen;
} WipeWndIn;

typedef struct {
    u32 planes;
    BOOL effect;
    u32 screen;
} WipeWndOut;

typedef struct {
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    u32 window;
    u32 screen;
} WipeWndPos;

// The window writes waiting for the VBlank, per screen and window
struct WipeWnd {
    WipeWndDisp disp[2];
    WipeWndIn in[2][2];
    u8 unk50[0x18];
    WipeWndOut out[2];
    WipeWndPos pos[2][2];
};

// pokebw2's WipeWnd_SetVisible: sets which windows a screen shows
void func_02023b48(u32 visible, u32 screen);
// pokebw2's WipeWnd_SetInsidePlane, WipeWnd_SetOutsidePlane and WipeWnd_SetPosition: the planes inside a window and
// outside the windows, and a window's rectangle, at once
void func_02023b74(u32 planes, BOOL effect, u32 window, u32 screen);
void func_02023bfc(u32 planes, BOOL effect, u32 screen);
void func_02023c3c(int x1, int y1, int x2, int y2, u32 window, u32 screen);
// The same at the next VBlank: pokebw2's WipeWnd_SetVisibleAtVBlank, WipeWnd_SetInsidePlaneAtVBlank,
// WipeWnd_SetOutsidePlaneAtVBlank and WipeWnd_SetPositionAtVBlank
void func_02023cfc(WipeWnd *wnd, u32 visible, u32 screen);
void func_02023d18(WipeWnd *wnd, u32 planes, BOOL effect, u32 window, u32 screen);
void func_02023d40(WipeWnd *wnd, u32 planes, BOOL effect, u32 screen);
void func_02023d60(WipeWnd *wnd, int x1, int y1, int x2, int y2, u32 window, u32 screen);
// The planes inside a window and outside the windows, read back from the registers: pokebw2's dead-stripped
// reconstructions, which only wipe_sub.c's dead-stripped plane swap calls (the ROM has neither)
GXWndPlane WipeWnd_GetInsidePlane(u32 window, u32 screen);
GXWndPlane WipeWnd_GetOutsidePlane(u32 screen);

#endif // BWPORT_SYSTEM_WIPE_WND_H
