#include "global.h"

#include "screen_fade_fx.h"
#include "sys_task.h"
#include "unk_struct_ov66_022342_f4.h"


void sub_02010ED0(UnkStruct_ov66_022342F4 *param0);
void sub_02010EC8(UnkStruct_ov66_022342F4 *param0);

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

void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID);

void sub_02010E64(UnkStruct_02010588 *param0, int param1, enum DSScreen screen, enum HeapID heapID) {
    switch (param1) {
    case 0:
    case 1:
        param0->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_02010FC0));
        param0->unk_04 = 1;
        param0->screen = screen;
        param0->unk_00->unk_600 = param1;
        break;
    case 2: {
        int v0;

        param0->unk_00 = Heap_Alloc(heapID, sizeof(UnkStruct_02010FC0) * 2);
        param0->unk_04 = 2;
        param0->screen = screen;

        for (v0 = 0; v0 < 2; v0++) {
            param0->unk_00[v0].unk_600 = v0;
        }
    } break;
    default:
        break;
    }
}

void sub_02010EC8(UnkStruct_ov66_022342F4 *param0) {
    sub_02010ED0(param0);
}
