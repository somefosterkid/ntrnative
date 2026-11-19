#include "global.h"
#include "bg_window.h"

static inline void inline_0201035C_sub(int param0, int param1, int param2, int param3, int param4, enum GFScreen screen) {
    if (param4 == 0) {
        if (screen == SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0Position(param0, param1, param2, param3);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0Position(param0, param1, param2, param3);
            }
        }
    } else {
        if (screen == SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd1Position(param0, param1, param2, param3);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1Position(param0, param1, param2, param3);
            }
        }
    }
}
static inline void inline_020107D8_sub(int param0, BOOL param1, enum GFScreen screen) {
    if (screen == SCREEN_MAIN) {
        if (GX_IsHBlank()) {
            G2_SetWndOutsidePlane(param0, param1);
        }
    } else {
        if (GX_IsHBlank()) {
            G2S_SetWndOutsidePlane(param0, param1);
        }
    }
}
static inline void inline_020107D8_sub_1(int param0, BOOL param1, int param4, enum GFScreen screen) {
    if (param4 == 0) {
        if (screen == SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0InsidePlane(param0, param1);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0InsidePlane(param0, param1);
            }
        }
    } else {
        if (screen == SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd1InsidePlane(param0, param1);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1InsidePlane(param0, param1);
            }
        }
    }
}
fx32 sub_020109BC(int param0);

fx32 sub_020109BC(int param0);
int sub_020109D8(int param0, int param1);

fx32 sub_020109BC(int param0) {
    return FX_Div(FX_SinIdx(param0), FX_CosIdx(param0));
}

int sub_020109D8(int param0, int param1) {
    fx32 v0;
    fx32 v1;

    v0 = sub_020109BC(param0);

    v1 = FX_MulInline(v0, param1 << 12);
    v1 >>= 12;

    return v1;
}
