#include "global.h"

#include "unk_02033AE0.h"
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

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02033AE0(CommRing *ring, u8 *buffer, s16 size);
#else
void sub_02033AE0(CommRing *ring, u8 *buffer, int size);
#endif

void sub_02035AE0(s32 arg0);

void sub_02035AE0(s32 arg0) {
    int netId, size;
    int maxMachines = sub_02033FC4((int)sub_0203993C()) + 1;

    _021D4140.sCommunicationSystem->playerMovementState = MOVEMENT_STATE_NORMAL;
    _021D4140.sCommunicationSystem->randomPadKeyTimer = 0;

    MI_CpuClear8(_021D4140.sCommunicationSystem->recvBufferRingServer, _021D4140.sCommunicationSystem->allocSize);
    MI_CpuClear8(_021D4140.sCommunicationSystem->sendRingClient, sizeof(CommRing) * (7 + 1));

    size = _021D4140.sCommunicationSystem->allocSize / maxMachines;

    for (netId = 0; netId < maxMachines; netId++) {
        sub_02033AE0(&_021D4140.sCommunicationSystem->sendRingClient[netId], &_021D4140.sCommunicationSystem->recvBufferRingServer[netId * size], size);
    }

    MI_CpuClear8(_021D4140.sCommunicationSystem->unk_488, _021D4140.sCommunicationSystem->allocSize);
    MI_CpuClear8(_021D4140.sCommunicationSystem->unk_4B0, sizeof(CommRing) * (7 + 1));

    for (netId = 0; netId < maxMachines; netId++) {
        sub_02033AE0(&_021D4140.sCommunicationSystem->unk_4B0[netId], &_021D4140.sCommunicationSystem->unk_488[netId * size], size);
    }

    MI_CpuClear8(_021D4140.sCommunicationSystem->sendBufferCommRingServer, (192 * 2));
    sub_02033AE0(&_021D4140.sCommunicationSystem->sendRingServer, _021D4140.sCommunicationSystem->sendBufferCommRingServer, (192 * 2));

    MI_CpuFill8(_021D4140.sCommunicationSystem->sendBufferServer[0], 0xee, (192 * 2));
    MI_CpuFill8(_021D4140.sCommunicationSystem->sendBufferServer[1], 0xee, (192 * 2));
    MI_CpuClear8(_021D4140.sCommunicationSystem->sendBufferCommRing, (12 * 22));

    sub_02033AE0(&_021D4140.sCommunicationSystem->sendRing, _021D4140.sCommunicationSystem->sendBufferCommRing, (12 * 22));

    MI_CpuFill8(_021D4140.sCommunicationSystem->sendBuffer[0], 0xee, 38);
    MI_CpuFill8(_021D4140.sCommunicationSystem->sendBuffer[1], 0xee, 38);

    _021D4140.sCommunicationSystem->sendBuffer[0][0] = 0xff;
    _021D4140.sCommunicationSystem->sendBuffer[1][0] = 0xff;

    MI_CpuClear8(_021D4140.sCommunicationSystem->recvBufferRing, _021D4140.sCommunicationSystem->maxPacketSize * 2);
    sub_02033AE0(&_021D4140.sCommunicationSystem->recvRing, _021D4140.sCommunicationSystem->recvBufferRing, _021D4140.sCommunicationSystem->maxPacketSize * 2);

    _021D4140.sCommunicationSystem->unk_6B4 = 0;
    _021D4140.sCommunicationSystem->unk_6B5 = 0;

    for (netId = 0; netId < (7 + 1); netId++) {
        _021D4140.sCommunicationSystem->unk_696[netId] = 0;
        _021D4140.sCommunicationSystem->unk_69E[netId] = 1;
        _021D4140.sCommunicationSystem->unk_644[netId] = 0;
        _021D4140.sCommunicationSystem->commRecvServer[netId].unk_0A = 0xee;
        _021D4140.sCommunicationSystem->commRecvServer[netId].unk_08 = 0xffff;
        _021D4140.sCommunicationSystem->commRecvServer[netId].unk_04 = NULL;
        _021D4140.sCommunicationSystem->commRecvServer[netId].unk_00 = 0;
        _021D4140.sCommunicationSystem->unk_66C[netId] = 0;
    }

    _021D4140.sCommunicationSystem->unk_668 = 0;
    _021D4140.sCommunicationSystem->commRecvClient.unk_0A = 0xee;
    _021D4140.sCommunicationSystem->commRecvClient.unk_08 = 0xffff;
    _021D4140.sCommunicationSystem->commRecvClient.unk_04 = NULL;
    _021D4140.sCommunicationSystem->commRecvClient.unk_00 = 0;
    _021D4140.sCommunicationSystem->unk_6B2 = 1;
    _021D4140.sCommunicationSystem->unk_6B3 = 1;
    _021D4140.unk_00 = 0;

    sub_02033F70(&_021D4140.sCommunicationSystem->commQueueManSend);
    sub_02033F70(&_021D4140.sCommunicationSystem->commQueueManSendServer);

    _021D4140.sCommunicationSystem->unk_6B7 = 0;
}
