#include "global.h"
#include "unk_02035900_work.h"
#include "sys_task.h"
#include "unk_02033AE0.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

BOOL sub_020373B4(u16 netId);

u16 sub_02036128(u16 param0);

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

int CommRing_Read(CommRing *ring, u8 *buffer, int size);

void sub_02033C28(CommRing *ring);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 sub_0203993C(void);
#else
int sub_0203993C(void);
#endif

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


BOOL sub_02036438(int param0);

s32 sub_0203772C(u16 param0);

int sub_02033B4C(CommRing *ring, u8 *buffer, int size);

BOOL sub_02036438(int param0) {
    int v0;
    int v1;
    int i, v3, v4 = 0;

    v0 = sub_0203772C(sub_0203993C());
    v1 = sub_02033FC4(sub_0203993C()) + 1;

    for (i = 0; i < v1; i++) {
        sub_02033C28(&_021D4140.sCommunicationSystem->unk_4B0[i]);

        if (sub_020373B4(i)) {
            _021D4140.sCommunicationSystem->sendBufferServer[param0][i * v0] = 0xe;
        } else {
            _021D4140.sCommunicationSystem->sendBufferServer[param0][i * v0] = 0xff;
            v4++;
            continue;
        }

        v3 = sub_02033B4C(&_021D4140.sCommunicationSystem->unk_4B0[i], &_021D4140.sCommunicationSystem->sendBufferServer[param0][i * v0], v0);

        if (_021D4140.sCommunicationSystem->sendBufferServer[param0][i * v0] == 0xe) {
            v4++;
        }
    }

    if (v4 == v1) {
        return 0;
    }

    return 1;
}
