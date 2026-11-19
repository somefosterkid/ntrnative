#include "global.h"

#include "screen_fade_fx.h"

struct HardwareWindowSettings;
struct ScreenFadeHBlanks;
void Heap_Free(void *ptr);
static inline void inline_0201035C_sub(int param0, int param1, int param2, int param3, int param4, enum DSScreen screen) {
    if (param4 == 0) {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0Position(param0, param1, param2, param3);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0Position(param0, param1, param2, param3);
            }
        }
    } else {
        if (screen == DS_SCREEN_MAIN) {
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
static inline void inline_020107D8_sub(int param0, BOOL param1, enum DSScreen screen) {
    if (screen == DS_SCREEN_MAIN) {
        if (GX_IsHBlank()) {
            G2_SetWndOutsidePlane(param0, param1);
        }
    } else {
        if (GX_IsHBlank()) {
            G2S_SetWndOutsidePlane(param0, param1);
        }
    }
}
static inline void inline_020107D8_sub_1(int param0, BOOL param1, int param4, enum DSScreen screen) {
    if (param4 == 0) {
        if (screen == DS_SCREEN_MAIN) {
            if (GX_IsHBlank()) {
                G2_SetWnd0InsidePlane(param0, param1);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0InsidePlane(param0, param1);
            }
        }
    } else {
        if (screen == DS_SCREEN_MAIN) {
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
UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);
static void sub_02012238(UnkStruct_02010588 *param0, UnkStruct_020101B0 *param1);

void sub_020121F4(UnkStruct_0201184C *param0);
void sub_02012204(UnkStruct_0201184C *param0);
static void sub_02012238(UnkStruct_02010588 *param0, UnkStruct_020101B0 *param1);

void sub_020121F4(UnkStruct_0201184C *param0) {
    Heap_Free(param0->unk_0C);
    param0->unk_0C = 0L;
}

void sub_02012204(UnkStruct_0201184C *param0) {
    int v0;
    UnkStruct_02010FC0 *v1 = sub_02010EE0(&param0->unk_00, 0);
    memset(v1->unk_300, 0, 768);

    for (v0 = (param0->unk_10 - 1); v0 >= 0; v0--) {
        sub_02012238(&param0->unk_00, &param0->unk_0C[v0].unk_00);
    }
}

static void sub_02012238(UnkStruct_02010588 *param0, UnkStruct_020101B0 *param1) {
    int v0;
    UnkStruct_02010FC0 *v1;
    UnkStruct_020101B0 v2;

    v1 = sub_02010EE0(param0, 0);

    v2.unk_00 = param1->unk_00 / 128;
    v2.unk_04 = param1->unk_04 / 128;
    v2.unk_08 = param1->unk_08 / 128;
    v2.unk_0C = param1->unk_0C / 128;

    for (v0 = v2.unk_04; v0 < v2.unk_0C; v0++) {
        v1->unk_300[0][v0] = v2.unk_00;
        v1->unk_300[1][v0] = v2.unk_08;
    }
}
