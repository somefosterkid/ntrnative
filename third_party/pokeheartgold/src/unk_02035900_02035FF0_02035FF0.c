#include "global.h"

#include "heap.h"
#include "overlay_00_thumb.h"
#include "sys_task_api.h"
#include "unk_02033AE0.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

u8 sub_02035FF0(void);

u8 sub_02035FF0(void) {
    u8 v0;

    v0 = _021D4140.sCommunicationSystem->transmissionType;
    if (v0 == 2) {
        return 1U;
    }
    if (v0 == 3) {
        v0 = 0;
    }
    return v0;
}

s32 sub_02036010(void);

s32 sub_02036010(void) {
    if (sub_02035FF0() == 1) {
        return 1;
    }
    return 0;
}

s32 sub_0203507C(void);

s32 sub_020379F8(void);

void sub_02036024(void) {
    s32 v0;

    v0 = 0;
    if (_021D4140.sCommunicationSystem != NULL) {
        if (sub_02034044((s32)sub_0203993C()) != 0) {
            ov00_021E6CB8();
            v0 = 1;
        } else if (sub_0203507C() != 0) {
            v0 = 1;
        }
    }
    if (v0 != 0) {
        sub_020379F8();
        sub_020343E4();
        _021D4140.unk_01 = 0;
        SysTask_Destroy(_021D4140.sCommunicationSystem->unk_57C);
        _021D4140.sCommunicationSystem->unk_57C = NULL;
        Heap_Free(_021D4140.sCommunicationSystem->recvBufferRing);
        Heap_Free(_021D4140.sCommunicationSystem->tempBuffer);
        Heap_Free(_021D4140.sCommunicationSystem->recvBufferRingServer);
        Heap_Free(_021D4140.sCommunicationSystem->unk_488);
        sub_02033F90(&_021D4140.sCommunicationSystem->commQueueManSendServer);
        sub_02033F90(&_021D4140.sCommunicationSystem->commQueueManSend);
        Heap_Free(_021D4140.unk_04);
        _021D4140.sCommunicationSystem = NULL;
        _021D4140.unk_04 = NULL;
    }
}
