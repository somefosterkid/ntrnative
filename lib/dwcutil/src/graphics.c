// libdwcutil's graphics.c (Black's ov016 0x021d603c-0x021d6360): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void DWCi_GXlVisible(s32 lcd, u32 plane) {
    if (lcd == 1) {
        GXS_SetVisiblePlane(GXS_GetVisiblePlane() | plane);
    } else {
        GX_SetVisiblePlane(GX_GetVisiblePlane() | plane);
    }
}

void DWCi_GXlInvisible(s32 lcd, u32 plane) {
    if (lcd == 1) {
        GXS_SetVisiblePlane(GXS_GetVisiblePlane() & ~plane);
    } else {
        GX_SetVisiblePlane(GX_GetVisiblePlane() & ~plane);
    }
}

void DWCi_GXlSetWndPosition(s32 lcd, s32 wnd, u16 *rect) {
    if (lcd == 1) {
        if (wnd == 0) {
            G2S_SetWnd0Position(rect[0], rect[1], rect[2], rect[3]);
        } else {
            G2S_SetWnd1Position(rect[0], rect[1], rect[2], rect[3]);
        }
    } else {
        if (wnd == 0) {
            G2_SetWnd0Position(rect[0], rect[1], rect[2], rect[3]);
        } else {
            G2_SetWnd1Position(rect[0], rect[1], rect[2], rect[3]);
        }
    }
}

void DWCi_GXlSetWndPlane(s32 lcd, s32 wnd, s32 plane, s32 effect) {
    switch (wnd) {
    case 0:
        if (lcd == 1) {
            G2S_SetWnd0InsidePlane(plane, effect);
        } else {
            G2_SetWnd0InsidePlane(plane, effect);
        }
        break;
    case 1:
        if (lcd == 1) {
            G2S_SetWnd1InsidePlane(plane, effect);
        } else {
            G2_SetWnd1InsidePlane(plane, effect);
        }
        break;
    case 2:
        if (lcd == 1) {
            G2S_SetWndOBJInsidePlane(plane, effect);
        } else {
            G2_SetWndOBJInsidePlane(plane, effect);
        }
        break;
    case 3:
        if (lcd == 1) {
            G2S_SetWndOutsidePlane(plane, effect);
        } else {
            G2_SetWndOutsidePlane(plane, effect);
        }
        break;
    }
}


