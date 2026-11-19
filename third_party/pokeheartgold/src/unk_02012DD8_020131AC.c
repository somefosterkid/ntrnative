#include "global.h"

#include "unk_02012DD8.h"
#include "unk_struct_ov66_022342_f4.h"
#include "bg_window.h"
#include "screen_fade_internal.h"

void *sub_02010EE0(void *, s32);

s32 sub_02013004(s32, s32, s32, s32, s32, s32, s32 *, s32 *);

void sub_020131F4(GXWndMask windowMask, enum GFScreen screen);
void sub_02013220(int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum GFScreen screen);
void sub_020132A8(int wnd, BOOL applyColorEffect, enum GFScreen screen);

void sub_020131AC(void *arg0) {
    s32 v1;
    s32 v0;
    s32 v3;
    void *v2;

    v2 = sub_02010EE0(arg0, 0);
    v3 = 0;
    do {
        sub_02013004(*((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_14), *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_C), *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_10), v3, *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_18), *((s32 *)&((UnkStruct_ov66_022342F4 *)arg0)->unk_1C), &v1, &v0);
        *(s16 *)((u8 *)v2 + 0x300) = (s16)v1;
        *(s16 *)((u8 *)v2 + 0x480) = (s16)v0;
        v2 += 2;
        v3 += 1;
    } while (v3 < 0xC0);
}

void sub_020131F4(GXWndMask windowMask, enum GFScreen screen) {
    if (screen == SCREEN_MAIN) {
        GX_SetVisibleWnd(windowMask);
    } else {
        GXS_SetVisibleWnd(windowMask);
    }
}

void sub_02013220(int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum GFScreen screen) {
    if (windowID == HW_WINDOW_WND0) {
        if (screen == SCREEN_MAIN) {
            G2_SetWnd0InsidePlane(wnd, applyColorEffect);
        } else {
            G2S_SetWnd0InsidePlane(wnd, applyColorEffect);
        }
    } else {
        if (screen == SCREEN_MAIN) {
            G2_SetWnd1InsidePlane(wnd, applyColorEffect);
        } else {
            G2S_SetWnd1InsidePlane(wnd, applyColorEffect);
        }
    }
}

void sub_020132A8(int wnd, BOOL applyColorEffect, enum GFScreen screen) {
    if (screen == SCREEN_MAIN) {
        G2_SetWndOutsidePlane(wnd, applyColorEffect);
    } else {
        G2S_SetWndOutsidePlane(wnd, applyColorEffect);
    }
}
