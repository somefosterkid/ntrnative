#include "global.h"
#include "unk_02035900_work.h"
#include "unk_struct_0203233c.h"
#include "sys_task.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

BOOL sub_02033ECC(CommQueueMan *param0, UnkStruct_0203233C *param1, BOOL param2);

u16 sub_02033250(void);

void sub_02036F30(u8 *param0);

void sub_02036F30(u8 *param0) {
    param0[0] = 0xb;

    if (_021D4140.sCommunicationSystem->unk_6B5 == 0) {
        param0[1] = 0x0;
    } else {
        param0[1] = 0x1;
    }

    u16 v1 = sub_02033250();

    param0[2] = v1 >> 8;
    param0[3] = v1 & 0xff;

    UnkStruct_0203233C v2;

    v2.unk_04 = 192 - 5;
    v2.unk_00 = &param0[5];

    if (sub_02033ECC(&_021D4140.sCommunicationSystem->commQueueManSendServer, &v2, 0)) {
        _021D4140.sCommunicationSystem->unk_6B5 = 0;
        param0[4] = (192 - 5) - v2.unk_04;
    } else {
        _021D4140.sCommunicationSystem->unk_6B5 = 1;
        param0[4] = 192 - 5;
    }
}
