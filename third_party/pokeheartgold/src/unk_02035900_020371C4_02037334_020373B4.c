#include "global.h"

#include "unk_02032844.h"
#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

int sub_02033298(void);

BOOL sub_020373B4(u16 netId) {
    if (!_021D4140.sCommunicationSystem) {
        return FALSE;
    }

    if (sub_02034044(sub_0203993C())) {
        if (_021D4140.sCommunicationSystem->wifiConnected) {
            u16 v0 = DWC_GetAIDBitmap();

            if (v0 & (1 << netId)) {
                return TRUE;
            }
        }

        return FALSE;
    }

    if (!sub_02037474()) {
        return FALSE;
    }

    if (sub_02033298() != 4) {
        return FALSE;
    }

    if (sub_0203769C() == netId) {
        return TRUE;
    } else if (sub_0203769C() == 0) {
        u16 v1 = sub_02033250();

        if (v1 & (1 << netId)) {
            return TRUE;
        }
    } else if (_021D4140.sCommunicationSystem->unk_694 & (1 << netId)) {
        return TRUE;
    }

    return FALSE;
}
