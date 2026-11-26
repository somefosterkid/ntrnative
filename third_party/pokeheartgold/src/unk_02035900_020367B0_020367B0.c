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

void sub_020367B0(u16 param0, u16 *param1, u16 param2);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0203772C(u8 param0);
#else
s32 sub_0203772C(u16 param0);
#endif


void sub_02033AF0(CommRing *ring, u8 *buffer, int size, int unused);

void sub_020367B0(u16 param0, u16 *param1, u16 param2) {
    u8 *v0 = (u8 *)param1;
    int i;
    int v2 = param2;

    _021D4140.sCommunicationSystem->unk_668--;

    if (v0 == 0L) {
        return;
    }

    if (v0[0] == 0xb) {
        if (sub_02035FF0() == 1) {
            return;
        }

        v0++;
        v2--;
    } else if (sub_02035FF0() == 0) {
        return;
    }

    if ((_021D4140.sCommunicationSystem->unk_6B2) && (v0[0] & 0x1)) {
        return;
    }

    _021D4140.sCommunicationSystem->unk_6B2 = 0;

    if (sub_02035FF0() == 1) {
        int v3 = sub_0203772C(sub_0203993C());
        int v4 = sub_02033FC4(sub_0203993C()) + 1;

        for (i = 0; i < v4; i++) {
            if (v0[0] == 0xff) {
                _021D4140.sCommunicationSystem->unk_694 = _021D4140.sCommunicationSystem->unk_694 & ~(1 << i);
            } else {
                _021D4140.sCommunicationSystem->unk_694 = _021D4140.sCommunicationSystem->unk_694 | (1 << i);
            }

            if (v0[0] == 0xff) {
                v0 += v3;
            } else if (v0[0] == 0xe) {
                v0 += v3;
            } else if ((_021D4140.sCommunicationSystem->unk_69E[i]) && (v0[0] & 0x1)) {
                v0 += v3;
            } else {
                v0++;
                sub_02033AF0(&_021D4140.sCommunicationSystem->sendRingClient[i], v0, v3 - 1, 1356 + i);
                v0 += (v3 - 1);
                _021D4140.sCommunicationSystem->unk_69E[i] = 0;
            }
        }
    } else {
        v0++;
        _021D4140.sCommunicationSystem->unk_694 = v0[0];
        _021D4140.sCommunicationSystem->unk_694 *= 256;

        v0++;
        _021D4140.sCommunicationSystem->unk_694 += v0[0];

        v0++;
        v2 -= 3;
        v2 = v0[0];

        v0++;
        sub_02033AF0(&_021D4140.sCommunicationSystem->recvRing, v0, v2, 1376);
    }
}
