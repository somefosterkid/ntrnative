#include "global.h"
#include "bg_window.h"

fx32 sub_020109BC(int param0);
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

int sub_02010A54(int param0, int param1);
int sub_02010A6C(int param0, int param1, int param2);

int sub_02010A54(int param0, int param1) {
    fx32 v0;
    fx32 v1;
    int v2;

    v0 = sub_020109BC(param0);
    v1 = (param1 / 2) << 12;
    v2 = FX_Div(v1, v0);

    return v2;
}

int sub_02010A6C(int param0, int param1, int param2) {
    int v0 = param1 - param0;
    v0 *= 128;
    v0 /= param2;

    return v0;
}
