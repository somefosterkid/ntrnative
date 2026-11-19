#include "global.h"

struct CommQueueMan;
struct SysTask;
struct SysTaskManager;
struct UnkStruct_020322D8_t;
typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;
typedef struct {
    UnkStruct_020322D8 *unk_00;
    UnkStruct_020322D8 *unk_04;
} UnkStruct_02032318;
typedef struct {
    u8 *buffer;
    s16 startIndex;
    volatile s16 endIndex;
    volatile s16 backupEndIndex;
    s16 size;
} CommRing;
void CommRing_Init(CommRing *ring, u8 *buffer, int size);
typedef struct CommQueueMan {
    UnkStruct_02032318 unk_00;
    UnkStruct_02032318 unk_08;
    UnkStruct_020322D8 *unk_10;
    CommRing *unk_14;
    void *unk_18;
    int unk_1C;
} CommQueueMan;
int sub_0203993C(void);
typedef struct SysTask SysTask;
typedef struct SysTaskManager SysTaskManager;
typedef void (*SysTaskFunc)(SysTask *, void *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
u16 CommLocal_MaxMachines(u16 param0);
typedef struct {
    int unk_00;
    u8 *unk_04;
    u16 unk_08;
    u8 unk_0A;
} CommRecvPackage;
typedef struct {
    u8 sendBuffer[2][64];
    u8 sendBufferServer[2][192];
    u8 sendBufferCommRing[264 ];
    u8 sendBufferCommRingServer[384];
    u8 *unk_488;
    u8 *recvBufferRingServer;
    u8 *recvBufferRing;
    u8 *tempBuffer;
    CommRing sendRing;
    CommRing recvRing;
    CommRing unk_4B0[8];
    CommRing sendRingServer;
    CommRing sendRingClient[8];
    SysTask *unk_57C;
    CommQueueMan commQueueManSend;
    CommQueueMan commQueueManSendServer;
    CommRecvPackage commRecvServer[8];
    CommRecvPackage commRecvClient;
    MATHRandContext32 rand;
    u16 unk_63C[8];
    u8 recvSpeed[8];
    u16 sendHeldKeys;
    u8 unk_656;
    u8 sendSpeed;
    u8 playerMovementState;
    s8 randomPadKeyTimer;
    u16 randomPadKey;
    BOOL unk_65C;
    volatile int unk_660;
    volatile int unk_664[8];
    int allocSize;
    int maxPacketSize;
    u16 unk_68C;
    u8 unk_68E[8]; /* HGSS layout: the 8-byte array starts at 0x68E (Platinum: u8 unk_68E; u8 unk_68F[8]); one pad byte after unk_69F */
    u8 unk_697[8];
    u8 unk_69F[4];
    u8 padding_6A3;
    u8 transmissionState;
    u8 unk_6A4;
    u8 transmissionType;
    u8 unk_6A6;
    u8 unk_6A7;
    u8 unk_6A8;
    u8 unk_6A9;
    u8 unk_6AA;
    u8 unk_6AB;
    u8 unk_6AC;
    u8 unk_6AD;
    u8 unk_6AE;
    u8 wifiConnected;
    u8 unk_6B0;
    u8 unk_6B1;
    u8 shuttingDown;
    u8 unk_6B3;
    u8 unk_6B4;
    u8 unk_6B5;
} CommunicationSystem;
typedef struct CommSysStatics {
    u8 unk_00[8];
    CommunicationSystem *sCommunicationSystem;
} CommSysStatics;
extern CommSysStatics _021D4140;

void sub_02035DE0(int netId);

void sub_02033AE0(CommRing *ring, u8 *buffer, int size);
s32 sub_02033FC4(u16 arg0);
void sub_02035DE0(int netId)
{
  CommunicationSystem *new_var;
  _021D4140.sCommunicationSystem->unk_68E[netId] = 0;
  _021D4140.sCommunicationSystem->unk_697[netId] = 1;
  _021D4140.sCommunicationSystem->unk_664[netId] = 0;
  int v0 = sub_02033FC4(sub_0203993C()) + 1;
  int v1 = _021D4140.sCommunicationSystem->allocSize / v0;
  new_var = _021D4140.sCommunicationSystem;
  sub_02033AE0(&_021D4140.sCommunicationSystem->unk_4B0[netId], &_021D4140.sCommunicationSystem->unk_488[netId * v1], v1);
  sub_02033AE0(&new_var->sendRingClient[netId], &_021D4140.sCommunicationSystem->recvBufferRingServer[netId * v1], v1);
  _021D4140.sCommunicationSystem->commRecvServer[netId].unk_0A = 0xee;
  _021D4140.sCommunicationSystem->commRecvServer[netId].unk_08 = 0xffff;
  _021D4140.sCommunicationSystem->commRecvServer[netId].unk_04 = 0L;
  _021D4140.sCommunicationSystem->commRecvServer[netId].unk_00 = 0;
}

