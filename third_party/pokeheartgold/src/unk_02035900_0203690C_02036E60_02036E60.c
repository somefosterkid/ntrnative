#include "global.h"
#include "unk_02035900_work.h"
#include "unk_struct_0203233c.h"
#include "sys_task.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

u16 sub_02036128(u16 param0);

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

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

BOOL sub_02033C50(CommQueueMan *param0);

BOOL sub_02033ECC(CommQueueMan *param0, UnkStruct_0203233C *param1, BOOL param2);

u8 sub_02033FC4(u16 param0);

BOOL sub_02035730(u8 *param0);

u8 sub_02035FF0(void);

BOOL sub_02036E60(u8 *param0);

s32 sub_0203772C(u16 param0);

BOOL sub_02036DA0(u8 *param0);

BOOL sub_02036E60(u8 *param0) {
    int v1 = sub_0203772C(sub_0203993C());
    sub_02033FC4(sub_0203993C());

    if (_021D4140.sCommunicationSystem->unk_6B4 == 0) {
        param0[0] = 0x0;
    } else {
        param0[0] = 0x1;
    }

    if (sub_02035FF0() == 0) {
        sub_02036DA0(param0);
    }

    _021D4140.sCommunicationSystem->unk_6B4 = 0;

    if (sub_02033C50(&_021D4140.sCommunicationSystem->commQueueManSend)) {
        param0[0] |= 0x2;

        if (param0[0] == 0x2) {
            return 0;
        }
    } else {
        UnkStruct_0203233C v3;

        v3.unk_04 = v1 - 1;
        v3.unk_00 = &param0[1];

        if (!sub_02033ECC(&_021D4140.sCommunicationSystem->commQueueManSend, &v3, 1)) {
            _021D4140.sCommunicationSystem->unk_6B4 = 1;
        }

        if (sub_02035FF0() == 1) {
            _021D4140.sCommunicationSystem->unk_6AA++;

            param0[0] |= ((_021D4140.sCommunicationSystem->unk_6AA << 4) & 0xf0);
        }
    }

    return 1;
}
