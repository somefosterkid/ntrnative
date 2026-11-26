#include "global.h"

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

void sub_02033F70(CommQueueMan *param0);

u16 CommLocal_MaxMachines(u16 param0);

enum MovementState {
    MOVEMENT_STATE_NORMAL = 0,
    MOVEMENT_STATE_RANDOM,
    MOVEMENT_STATE_REVERSE,
};

s32 sub_02033298(void);

s32 sub_02033800(void *, s32, s32, void (*)(s32));

u8 sub_02035FF0(void);

void sub_02036438(int);

void sub_020367B0(u16 param0, u16 *param1, u16 param2);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0203772C(u8);
#else
s32 sub_0203772C(u16);
#endif

void sub_02036ABC(s32);

extern volatile u8 _0210F900[2];

void sub_02036508(void);

void sub_02036508(void) {
    int i;
    int maxMachines;

    if (_021D4140.sCommunicationSystem == NULL) {
        return;
    }
    if (sub_02034044(sub_0203993C())) {
        return;
    }
    sub_0203772C((int)sub_0203993C());
    maxMachines = sub_02033FC4((int)sub_0203993C()) + 1;
    if (_0210F900[0] == 2 || _0210F900[0] == 0) {
        _0210F900[0]++;
        if (sub_02035FF0() == 1 && _021D4140.unk_00 == 0) {
            sub_02036438(_021D4140.sCommunicationSystem->unk_6B0);
            _021D4140.unk_00 = 1;
        }
        if (sub_02033298() == 4 && sub_02033800(_021D4140.sCommunicationSystem->sendBufferServer[_021D4140.sCommunicationSystem->unk_6B0], 0xC0, 0xE, sub_02036ABC) == 0) {
            _0210F900[0]--;
        }
        if (_0210F900[0] == 1 || _0210F900[0] == 3) {
            _021D4140.unk_00 = 0;
            for (i = 0; i < maxMachines; i++) {
                if (sub_020373B4(i)) {
                    _021D4140.sCommunicationSystem->unk_66C[i]++;
                }
            }
            sub_020367B0(0, (u16 *)_021D4140.sCommunicationSystem->sendBufferServer[_021D4140.sCommunicationSystem->unk_6B0], 192);
            _021D4140.sCommunicationSystem->unk_6B0 = 1 - _021D4140.sCommunicationSystem->unk_6B0;
        }
        if (sub_02033298() != 4) {
            _0210F900[0]++;
        }
    }
}
