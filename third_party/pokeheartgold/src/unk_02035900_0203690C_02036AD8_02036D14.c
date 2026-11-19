#include "global.h"
#include "unk_02035900_work.h"
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

BOOL sub_02036D14(u8 *param0, int param1);

BOOL sub_02036D14(u8 *param0, int param1) {
    u8 v1[2];

    _021D4140.sCommunicationSystem->unk_644[param1] = 0;

    if (0x10 == (*param0 & 0x10)) {
        v1[0] = *param0 & 0xc;

        if (v1[0] == 0x0) {
            _021D4140.sCommunicationSystem->unk_644[param1] |= 0x0040;
        } else if (v1[0] == 0x4) {
            _021D4140.sCommunicationSystem->unk_644[param1] |= 0x0080;
        } else if (v1[0] == 0x8) {
            _021D4140.sCommunicationSystem->unk_644[param1] |= 0x0020;
        } else if (v1[0] == 0xC) {
            _021D4140.sCommunicationSystem->unk_644[param1] |= 0x0010;
        }

        _021D4140.sCommunicationSystem->recvSpeed[param1] = (*param0 >> 5) & 0x7;
    }

    return 1;
}
