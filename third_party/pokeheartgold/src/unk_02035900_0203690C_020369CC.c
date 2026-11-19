#include "global.h"
#include "unk_02035900_work.h"
#include "sys_task.h"
#include "unk_02033AE0.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

u16 sub_02036128(u16 param0);

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

void CommRring_Write(CommRing *ring, u8 *buffer, int size, int unused);

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

u16 CommLocal_MaxMachines(u16 param0);

u8 sub_02035FF0(void);

void sub_020369CC(u16 param0, u16 *param1, u16 param2);

s32 sub_0203772C(u16 param0);

void sub_02033AF0(CommRing *ring, u8 *buffer, int size, int unused);


void sub_020369CC(u16 param0, u16 *param1, u16 param2) {
    u8 *buffer = (u8 *)param1;
    int v1;

    _021D4140.sCommunicationSystem->unk_66C[param0]--;

    if (buffer == 0L) {
        return;
    }

    if ((_021D4140.sCommunicationSystem->unk_69E[param0]) && (buffer[0] & 0x1)) {
        v1 = 0;
        return;
    }

    _021D4140.sCommunicationSystem->unk_69E[param0] = 0;

    if (sub_02035FF0() == 1) {
        int v2 = sub_0203772C(sub_0203993C());
        int v3 = sub_02033FC4(sub_0203993C()) + 1;

        if (buffer[0] == 0xff) {
            _021D4140.sCommunicationSystem->unk_694 = _021D4140.sCommunicationSystem->unk_694 & ~(1 << param0);
        } else {
            _021D4140.sCommunicationSystem->unk_694 = _021D4140.sCommunicationSystem->unk_694 | (1 << param0);
        }

        if (buffer[0] == 0xff) {
            (void)0;
        } else if (buffer[0] == 0x2) {
            (void)0;
        } else if (buffer[0] == 0xe) {
            (void)0;
        } else if ((_021D4140.sCommunicationSystem->unk_69E[param0]) && (buffer[0] & 0x1)) {
            (void)0;
        } else {
            buffer++;
            sub_02033AF0(&_021D4140.sCommunicationSystem->sendRingClient[param0], buffer, v2 - 1, 1511);
            _021D4140.sCommunicationSystem->unk_69E[param0] = 0;
        }
    }
}
