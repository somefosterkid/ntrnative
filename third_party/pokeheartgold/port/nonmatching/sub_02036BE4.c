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
typedef struct CommQueueMan {
    UnkStruct_02032318 unk_00;
    UnkStruct_02032318 unk_08;
    UnkStruct_020322D8 *unk_10;
    CommRing *unk_14;
    void *unk_18;
    int unk_1C;
} CommQueueMan;
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
enum MovementState {
    MOVEMENT_STATE_NORMAL = 0,
    MOVEMENT_STATE_RANDOM,
    MOVEMENT_STATE_REVERSE,
};
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
    u8 unk_68E;
    u8 unk_68F[8];
    u8 unk_697[8];
    u8 unk_69F[4];
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
extern u8 _021D4140[]; /* HGSS: the comm system pointer is word 2 of this .bss block (Platinum: sCommunicationSystem) */

void sub_02036BE4(void);
void sub_02036BE4(void)
{
    u16 newHeldKeys = 0;

    if ((((CommunicationSystem **)_021D4140)[2])->playerMovementState == MOVEMENT_STATE_NORMAL) {
        return;
    }

    if (!((((CommunicationSystem **)_021D4140)[2])->sendHeldKeys & (PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_KEY_UP | PAD_KEY_DOWN))) {
        return;
    }

    if ((((CommunicationSystem **)_021D4140)[2])->playerMovementState == MOVEMENT_STATE_REVERSE) {
        if ((((CommunicationSystem **)_021D4140)[2])->sendHeldKeys & PAD_KEY_LEFT) {
            newHeldKeys |= PAD_KEY_RIGHT;
        }

        if ((((CommunicationSystem **)_021D4140)[2])->sendHeldKeys & PAD_KEY_RIGHT) {
            newHeldKeys |= PAD_KEY_LEFT;
        }

        if ((((CommunicationSystem **)_021D4140)[2])->sendHeldKeys & PAD_KEY_UP) {
            newHeldKeys |= PAD_KEY_DOWN;
        }

        if ((((CommunicationSystem **)_021D4140)[2])->sendHeldKeys & PAD_KEY_DOWN) {
            newHeldKeys |= PAD_KEY_UP;
        }
    } else {
        if ((((CommunicationSystem **)_021D4140)[2])->randomPadKey) {
            newHeldKeys = (((CommunicationSystem **)_021D4140)[2])->randomPadKey;
            (((CommunicationSystem **)_021D4140)[2])->randomPadKeyTimer--;

            if ((((CommunicationSystem **)_021D4140)[2])->randomPadKeyTimer < 0) {
                (((CommunicationSystem **)_021D4140)[2])->randomPadKey = 0;
            }
        } else {
            switch (MATH_Rand32(&(((CommunicationSystem **)_021D4140)[2])->rand, 4)) {
            case 0:
                newHeldKeys = PAD_KEY_LEFT;
                break;
            case 1:
                newHeldKeys = PAD_KEY_RIGHT;
                break;
            case 2:
                newHeldKeys = PAD_KEY_UP;
                break;
            case 3:
                newHeldKeys = PAD_KEY_DOWN;
                break;
            }

            (((CommunicationSystem **)_021D4140)[2])->randomPadKeyTimer = MATH_Rand32(&(((CommunicationSystem **)_021D4140)[2])->rand, 16);
            (((CommunicationSystem **)_021D4140)[2])->randomPadKey = newHeldKeys;
        }
    }

    (((CommunicationSystem **)_021D4140)[2])->sendHeldKeys &= ~(PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_KEY_UP | PAD_KEY_DOWN);
    (((CommunicationSystem **)_021D4140)[2])->sendHeldKeys += newHeldKeys;
}
