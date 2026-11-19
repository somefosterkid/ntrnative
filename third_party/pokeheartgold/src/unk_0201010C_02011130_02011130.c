#include "global.h"

#include "screen_fade_fx.h"

struct HardwareWindowDimensions;

struct HardwareWindowInsidePlane;

struct HardwareWindowOutsidePlane;

struct HardwareWindowSettings;

struct HardwareWindowVisibility;

struct ScreenFadeHBlanks;

enum {
    GX_BLEND_BGALL = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3,
    GX_BLEND_ALL = GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
};





























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

void sub_02010F34(int param0, HardwareWindowSettings *param1, enum DSScreen screen);

void sub_02010F84(HardwareWindowSettings *param0, int param1, int param2, int param3, enum DSScreen screen, int param5, int param6, int param7, int param8, int param9);

void sub_02010710(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);

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

static inline void inline_020107D8(UnkStruct_0201076C *param0, int param1, int param2) {
    UnkStruct_02012174 *v0 = &param0->unk_00[param2];

    if (v0->unk_C0[param1] == 0) {
        inline_020107D8_sub(GX_BLEND_ALL, 1, (enum DSScreen)(param0->screen));
        inline_020107D8_sub_1(GX_BLEND_PLANEMASK_BD, 1, v0->unk_180, (enum DSScreen)(param0->screen));
    } else {
        inline_020107D8_sub(GX_BLEND_PLANEMASK_BD, 1, (enum DSScreen)(param0->screen));
        inline_020107D8_sub_1(GX_BLEND_ALL, 1, v0->unk_180, (enum DSScreen)(param0->screen));
    }
}

void sub_02010E48(UnkStruct_02010EA4 *param0, const UnkStruct_0200F980 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5);

void sub_02010FC0(UnkStruct_0201109C *param0, const UnkStruct_0200FB7C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011298(UnkStruct_02011360 *param0, const UnkStruct_0200FC2C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011494(UnkStruct_02011568 *param0, const UnkStruct_0200FD34 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011738(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011A00(UnkStruct_02011AFC *param0, UnkStruct_0200FEA4 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011D34(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011FE8(UnkStruct_020120D4 *param0, UnkStruct_0200FE6C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012290(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, int param7);

void sub_02012384(UnkStruct_02012290 *param0, UnkStruct_0201006C *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02011130(void *param0);

void sub_02011130(void *param0) {
    UnkStruct_0201076C *v0 = (UnkStruct_0201076C *)param0;
    int v1;

    ((param0) ? (void)0 : GF_AssertFail());

    v1 = GX_GetVCount();

    if (v1 < 192) {
        v1++;

        if (v1 > 191) {
            v1 -= 192;
        }

        if (v0->unk_308 == 1) {
            inline_020107D8(v0, v1, 0);
        } else {
            inline_020107D8(v0, v1, 0);
            inline_020107D8(v0, v1, 1);
        }
    }
}
