#include "global.h"

#include "screen_fade_fx.h"
#include "sys_task.h"

struct WipeHBlank;

typedef struct WipeHBlank WipeHBlank;

typedef void (*WipeHBlankFunc)(void *work);

struct WipeHBlank {
    void *work[2];
    WipeHBlankFunc func[2];
    BOOL active[2];
};

void sub_0200FFB4(WipeHBlank *hblank, int slot, u32 heapId);

typedef struct {

    u8 prev[192];
    u8 lines[192];
    u32 window;
} WipeLineBuf;

typedef struct {
    WipeLineBuf buf[2];
    u8 count;
    u8 screen;
} WipeLineWnd;

void sub_020110F4(WipeHBlank *hblank, WipeLineWnd *lines, u32 heapId);

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void SysTask_Destroy(SysTask *task);
void sub_02011104(SysTask *param0, void *param1);

void sub_020110F4(WipeHBlank *hblank, WipeLineWnd *lines, u32 heapId) {
    sub_0200FFB4(hblank, lines->screen, heapId);
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

void sub_02011104(SysTask *param0, void *param1) {
    UnkStruct_0201076C *v0 = param1;
    int v1;

    for (v1 = 0; v1 < 2; v1++) {
        memcpy(v0->unk_00[v1].unk_C0, v0->unk_00[v1].unk_00, sizeof(u8) * 192);
    }

    SysTask_Destroy(param0);
}
