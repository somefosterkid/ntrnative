#include "global.h"

#include "screen_fade_fx.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task_api.h"

struct HardwareWindowSettings;

struct ScreenFadeHBlanks;

void sub_0200FF88(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen, enum HeapID heapID);








void sub_02010AB0(UnkStruct_020101B0 *param0, UnkStruct_020101B0 *param1, UnkStruct_020101B0 *param2, const UnkStruct_0200FD8C *param3, const UnkStruct_0200FD8C *param4, int param5);

void sub_02010C38(void *param0);

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);

void sub_02010F00(SysTask *param0, void *param1);

void sub_02012204(UnkStruct_0201184C *param0);

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

void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID);

void sub_02010F84(HardwareWindowSettings *param0, int param1, int param2, int param3, enum DSScreen screen, int param5, int param6, int param7, int param8, int param9);

void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);

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

void sub_02012090(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_02012090(UnkStruct_0201184C *param0, const UnkStruct_02011738 *param1, int param2, int param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    int v0;
    UnkStruct_02010FC0 *v1;

    param0->unk_0C = Heap_Alloc(heapID, sizeof(UnkStruct_0201184C_sub1) * param1->unk_08);
    GF_ASSERT(param0->unk_0C != NULL);
    param0->unk_10 = param1->unk_08;

    for (v0 = 0; v0 < param1->unk_08; v0++) {
        sub_02010AB0(&param0->unk_0C[v0].unk_00, &param0->unk_0C[v0].unk_20, &param0->unk_0C[v0].unk_10, (param1->unk_00 + v0), (param1->unk_04 + v0), param2);
    }

    sub_02010E64(&param0->unk_00, param1->unk_0A, screen, heapID);

    param0->unk_14 = param2;
    param0->unk_18 = param3;
    param0->unk_1C = 0;
    param0->unk_24 = param5;
    param0->unk_28 = param6;
    param0->heapID = heapID;
    param0->unk_20 = param1->unk_0E;

    sub_02012204(param0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);

    v1 = sub_02010EE0(&param0->unk_00, 0);
    sub_02010F84(param5, param1->unk_0C, param1->unk_0D, param1->unk_0A, screen, v1->unk_300[0][0], 0, v1->unk_300[1][0], 192, param0->unk_20);

    if (param1->unk_0A == 0) {
        sub_02011068(param0->unk_24, GX_WNDMASK_W0, screen, param0->unk_20);
    } else {
        sub_02011068(param0->unk_24, GX_WNDMASK_W1, screen, param0->unk_20);
    }

    sub_0200FF88(param0->unk_28, &param0->unk_00, sub_02010C38, screen, heapID);
}

void sub_0200FFB4(ScreenFadeHBlanks *hblanks, enum DSScreen screen, enum HeapID heapID);

void sub_02012290(UnkStruct_0201184C *param0);

BOOL sub_020121A4(UnkStruct_0201184C *param0);

BOOL sub_020121A4(UnkStruct_0201184C *param0) {
    param0->unk_1C++;

    if (param0->unk_1C >= param0->unk_18) {
        param0->unk_1C = 0;

        if ((param0->unk_14 - 1) > 0) {
            param0->unk_14--;
            sub_02012290(param0);
            sub_02012204(param0);
            SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);
        } else {
            sub_0200FFB4(param0->unk_28, param0->unk_00.screen, param0->heapID);
            return 1;
        }
    }

    return 0;
}
