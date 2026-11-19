#include "global.h"

#include "sys_task.h"
#include "system.h"
#include "screen_fade_internal.h"

typedef struct System System;

struct SoftwareSprite;

struct SysTask;

struct SysTaskQueue;

struct System;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;

BOOL ov41_02245F04(UnkStruct_ov22_02255040 *param0);

void ov41_02245FA8(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);

int ov41_022464BC(NNSG2dCharacterData *param0, int param1, int param2, int param3);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

BOOL ov41_02245F14(UnkStruct_ov22_02255040 *param0, int *param1, int *param2, NNSG2dCharacterData *param3);

BOOL ov41_02245F14(UnkStruct_ov22_02255040 *param0, int *param1, int *param2, NNSG2dCharacterData *param3) {
    BOOL v0;
    int v1, v2;
    int v3, v4;

    v0 = ov41_02245F04(param0);

    if (v0 == 0) {
        return 0;
    }

    ov41_02245FA8(param0, &v1, &v2);

    *param1 = gSystem.touchX - v1;
    *param2 = gSystem.touchY - v2;

    for (v3 = (*param2 - 4); v3 < (*param2 + 4); v3++) {
        if (v3 >= 0) {
            for (v4 = (*param1 - 4); v4 < (*param1 + 4); v4++) {
                if (v4 >= 0) {
                    v0 = ov41_022464BC(param3, v4, v3, 0);

                    if (v0 == 0) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}
