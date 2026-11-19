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

enum TransmissionType {
    TRANSMISSION_TYPE_SERVER_CLIENT,
    TRANSMISSION_TYPE_PARALLEL,
    TRANSMISSION_TYPE_SWITCH_TO_SERVER_CLIENT,
    TRANSMISSION_TYPE_SWITCH_TO_PARALLEL
};

void sub_02035FA8(int type);

void sub_02035FA8(int type) {
    u8 cur = _021D4140.sCommunicationSystem->transmissionType;
    if (cur == TRANSMISSION_TYPE_SERVER_CLIENT && type == TRANSMISSION_TYPE_PARALLEL) {
        _021D4140.sCommunicationSystem->transmissionType = TRANSMISSION_TYPE_SWITCH_TO_PARALLEL;
        return;
    }
    if (cur == TRANSMISSION_TYPE_PARALLEL && type == TRANSMISSION_TYPE_SERVER_CLIENT) {
        _021D4140.sCommunicationSystem->transmissionType = TRANSMISSION_TYPE_SWITCH_TO_SERVER_CLIENT;
    }
}
