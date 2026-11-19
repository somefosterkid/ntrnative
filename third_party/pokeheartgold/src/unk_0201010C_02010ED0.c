#include "global.h"

#include "screen_fade_fx.h"

#include "heap.h"
#include "unk_struct_ov66_022342_f4.h"
#include "sys_task.h"
#include "ppw_lobby.h"

struct PPW_LobbyVipRecord;
void sub_02010ED0(UnkStruct_ov66_022342F4 *param0);

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void SysTask_Destroy(SysTask *task);
UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);
UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1);
void sub_02010F00(SysTask *param0, void *param1);

s32 sub_02013424(s32, s32, s32);
s32 sub_02013440(s32, s32, s32, s32, s32);
s32 sub_02013468(s32, s32, s32, s32);
s32 sub_02013488(s32, s32, s32, s32, s32, s32, s32);
void sub_02010F34(s32 arg0, s32 arg1, s32 arg2);

void sub_02010ED0(UnkStruct_ov66_022342F4 *param0) {
    Heap_Free(param0->lobbyVipRecord);
    param0->lobbyVipRecord = 0L;
}

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

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02010588 *param0, int param1) {
    ((param0->unk_04 > param1) ? (void)0 : GF_AssertFail());
    return param0->unk_00 + param1;
}

void sub_02010F00(SysTask *param0, void *param1) {
    UnkStruct_02010588 *v0 = (UnkStruct_02010588 *)param1;
    UnkStruct_02010FC0 *v1;
    int v2;

    for (v2 = 0; v2 < v0->unk_04; v2++) {
        v1 = sub_02010EE0(v0, v2);
        memcpy(v1->unk_00, v1->unk_300, sizeof(short) * 2 * 192);
    }

    SysTask_Destroy(param0);
}

void sub_02010F34(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        sub_02013424(arg1, 0, arg2);
        return;
    }
    sub_02013424(arg1, 1, arg2);
    sub_02013440(arg1, 0x3F, 0, 0, arg2);
    sub_02013488(arg1, 0, 0, 0, 0, 0, arg2);
    sub_02013468(arg1, 0x20, 0, arg2);
}
