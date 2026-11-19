#include "global.h"
#include "unk_02035900_work.h"
#include "sys_task.h"

struct CommQueueMan;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_020322D8_t;

typedef struct UnkStruct_020322D8_t UnkStruct_020322D8;

int CommRing_Read(CommRing *ring, u8 *buffer, int size);

u8 CommRing_ReadByte(CommRing *ring);

int sub_02033BC4(CommRing *ring);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

int sub_020341DC(int param0);

BOOL sub_02034244(int param0);

void *sub_02034280(int param0, int param1, int param2);

void sub_020371A8(int netId, int command, int param2, void *param3, CommRecvPackage *param4);

void sub_020371C4(CommRing *ring, int netId, u8 *buffer, CommRecvPackage *param3);

u8 sub_02033B68(CommRing *ring);

int sub_02033B4C(CommRing *ring, u8 *buffer, int size);

void sub_020371C4(CommRing *ring, int netId, u8 *buffer, CommRecvPackage *param3) {
    int size;
    u8 cmd;
    int v2;
    int v3;

    while (sub_02033BC4(ring) != 0) {
        v2 = ring->startIndex;

        if (param3->unk_0A != 0xee) {
            cmd = param3->unk_0A;
        } else {
            cmd = sub_02033B68(ring);

            if (cmd == 0xee) {
                continue;
            }
        }

        v2 = ring->startIndex;
        param3->unk_0A = cmd;

        if (param3->unk_08 != 0xffff) {
            size = param3->unk_08;
        } else {
            size = sub_020341DC(cmd);

            if (_021D4140.sCommunicationSystem->unk_6B8) {
                return;
            }

            if (size == 0xFFFF) {
                if (sub_02033BC4(ring) < 1) {
                    ring->startIndex = v2;
                    break;
                }

                size = sub_02033B68(ring) * 0x100;
                size += sub_02033B68(ring);
                v2 = ring->startIndex;
            }

            param3->unk_08 = size;
        }

        if (sub_02034244(cmd)) {
            if (param3->unk_04 == 0L) {
                param3->unk_04 = sub_02034280(cmd, netId, param3->unk_08);
            }

            v3 = sub_02033B4C(ring, buffer, size - param3->unk_00);

            if (param3->unk_04) {
                MI_CpuCopy8(buffer, &param3->unk_04[param3->unk_00], v3);
            }

            param3->unk_00 += v3;

            if (param3->unk_00 >= size) {
                sub_020371A8(netId, cmd, size, param3->unk_04, param3);

                if (cmd == 17) {
                    break;
                }
            }
        } else {
            if (sub_02033BC4(ring) >= size) {
                sub_02033B4C(ring, buffer, size);
                sub_020371A8(netId, cmd, size, (void *)buffer, param3);

                if (cmd == 17) {
                    break;
                }
            } else {
                ring->startIndex = v2;
                break;
            }
        }
    }
}
