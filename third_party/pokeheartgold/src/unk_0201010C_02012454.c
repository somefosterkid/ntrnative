#include "global.h"

#include "screen_fade_fx.h"
#include "sys_task.h"

struct HardwareWindowSettings;
struct ScreenFadeHBlanks;
struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void sub_0200FFB4(ScreenFadeHBlanks *hblanks, enum DSScreen screen, enum HeapID heapID);
SysTask * SysTask_CreateOnVWaitQueue(SysTaskFunc func, void *data, u32 priority);
void sub_02010F00(SysTask *param0, void *param1);
void sub_020124B0(UnkStruct_02011AFC *param0);
void sub_020125D4(UnkStruct_02011C7C *param0, int param1, int param2);
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

BOOL sub_02012454(UnkStruct_02011AFC *param0);

BOOL sub_02012454(UnkStruct_02011AFC *param0) {
    param0->unk_24++;

    if (param0->unk_24 >= param0->unk_20) {
        param0->unk_24 = 0;

        if ((param0->unk_1C + 1) <= param0->unk_18) {
            param0->unk_1C++;

            sub_020125D4(&param0->unk_0C, param0->unk_1C, param0->unk_18);
            sub_020124B0(param0);
            SysTask_CreateOnVWaitQueue(sub_02010F00, &param0->unk_00, 1023);
        } else {
            sub_0200FFB4(param0->unk_34, param0->unk_00.screen, param0->heapID);
            return 1;
        }
    }

    return 0;
}
