#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02033AE0.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_02035900_work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/unk_02035900.s: 0xC bytes on the DS, 4-aligned: the block and
// system pointers sat misaligned). The asm label stays for the DS build.
CommSysStatics _021D4140;
#endif

void sub_02035DA4(void);

void sub_020360EC(SysTask *task, void *data);

s32 sub_02033F44(void *, s32, void *);

s32 sub_02035900(s32 shouldAlloc, s32 maxPacketSize);

s32 sub_02035900(s32 shouldAlloc, s32 maxPacketSize) {
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    s32 temp_r0;
    void *temp_r0_2;
#endif
    s32 temp_r6;
    s32 var_r0;
    s32 var_r4;

    var_r4 = 0;
    _021D4140.unk_01 = 0;
    if (shouldAlloc == 0) {
        goto assertBlock;
    }
    temp_r6 = sub_02033FC4((u32)(u16)sub_0203993C()) + 1;
    if (_021D4140.sCommunicationSystem != NULL) {
        return 1;
    }
    sub_020379A0(HEAP_ID_15);
#if defined(SDK_PORT) || defined(NONMATCHING)
    // PC: no DMA, so no 32-byte alignment; the system sits at the end of the 0x20 slack (DS: 1-32 bytes in)
    _021D4140.unk_04 = Heap_Alloc(HEAP_ID_15, sizeof(CommunicationSystem) + 0x20);
    _021D4140.sCommunicationSystem = (CommunicationSystem *)((u8 *)_021D4140.unk_04 + 0x20);
    MI_CpuFill8(_021D4140.sCommunicationSystem, 0U, sizeof(CommunicationSystem));
#else
    temp_r0 = (s32)Heap_Alloc(HEAP_ID_15, sizeof(CommunicationSystem) + 0x20);
    _021D4140.unk_04 = (void *)temp_r0;
    temp_r0_2 = (void *)((0x20 - (temp_r0 & 0x1F)) + temp_r0);
    _021D4140.sCommunicationSystem = temp_r0_2;
    MI_CpuFill8(temp_r0_2, 0U, sizeof(CommunicationSystem));
#endif
    if (sub_02034044((s32)sub_0203993C()) != 0) {
        _021D4140.sCommunicationSystem->maxPacketSize = (u32)((maxPacketSize * 2) + 0x40);
    } else {
        _021D4140.sCommunicationSystem->maxPacketSize = (u32)(maxPacketSize + 0x40);
    }
    _021D4140.sCommunicationSystem->allocSize = (u32)(_021D4140.sCommunicationSystem->maxPacketSize * temp_r6);
    _021D4140.sCommunicationSystem->transmissionType = 0;
    _021D4140.sCommunicationSystem->unk_6AE = 0x2A;
    _021D4140.sCommunicationSystem->recvBufferRing = Heap_Alloc(HEAP_ID_15, _021D4140.sCommunicationSystem->maxPacketSize * 2);
    _021D4140.sCommunicationSystem->tempBuffer = Heap_Alloc(HEAP_ID_15, _021D4140.sCommunicationSystem->maxPacketSize);
    _021D4140.sCommunicationSystem->recvBufferRingServer = Heap_Alloc(HEAP_ID_15, _021D4140.sCommunicationSystem->allocSize);
    _021D4140.sCommunicationSystem->unk_488 = Heap_Alloc(HEAP_ID_15, _021D4140.sCommunicationSystem->allocSize);
    if (sub_0203993C() == 0xA) {
        sub_02033F44(&_021D4140.sCommunicationSystem->commQueueManSend, 0x64, &_021D4140.sCommunicationSystem->sendRing);
        sub_02033F44(&_021D4140.sCommunicationSystem->commQueueManSendServer, 0x320, &_021D4140.sCommunicationSystem->sendRingServer);
    } else {
        sub_02033F44(&_021D4140.sCommunicationSystem->commQueueManSend, 0x14, &_021D4140.sCommunicationSystem->sendRing);
        sub_02033F44(&_021D4140.sCommunicationSystem->commQueueManSendServer, 0x118, &_021D4140.sCommunicationSystem->sendRingServer);
    }
    goto block_12;
assertBlock:
    var_r4 = 1;
    if (_021D4140.sCommunicationSystem == NULL) {
        GF_AssertFail();
    }
block_12:
    var_r0 = 0;
    _021D4140.sCommunicationSystem->unk_694 = 0;
    do {
        _021D4140.sCommunicationSystem->unk_6A6[var_r0] = 0xFF;
        var_r0++;
    } while (var_r0 < 4);
    if (var_r4 == 0) {
        sub_02035DA4();
    }
    sub_0203778C(&_021D4140.sCommunicationSystem->rand);
    if (var_r4 == 0) {
        _021D4140.sCommunicationSystem->unk_57C = SysTask_CreateOnVBlankQueue((void (*)(SysTask *, void *))sub_020360EC, NULL, 0U);
    }
    _021D4140.sCommunicationSystem->wifiConnected = 0;
    return 1;
}
