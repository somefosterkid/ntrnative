#ifndef POKEHEARTGOLD_UNK_02035900_WORK_H
#define POKEHEARTGOLD_UNK_02035900_WORK_H

// Port (hk253, crash 73): the communication system of unk_02035900_* (sub_02035900 puts it 32-aligned in a Heap_Alloc'd
// block of sizeof + 0x20, 0x6C0 + 0x20 on the DS) and the statics that hold it (_021D4140, 0xC on the DS: the block at +4,
// the system at +8). The 59 files read it at raw DS offsets or through ~7 disagreeing local copies (some with Platinum's
// offsets); the layout here is the asm's (asm/unk_02035900.s at c3776c093). Names: Platinum's where the field is the same
// one (HGSS = Platinum + 8 at 0x488-0x694, + 7 at 0x696-0x6A9, + 8 at 0x6AB-0x6AE, + 7 at 0x6B6), else the DS offset. The comments are the DS offsets.

#include "comm_queue_man.h"
#include "comm_ring.h"

struct SysTask;

typedef struct CommRecvPackage {
    int unk_00; // 0x0
    u8 *unk_04; // 0x4
    u16 unk_08; // 0x8
    u8 unk_0A; // 0xA
} CommRecvPackage; // 0xC

typedef struct CommunicationSystem {
    u8 sendBuffer[2][64]; // 0x0
    u8 sendBufferServer[2][192]; // 0x80
    u8 sendBufferCommRing[264]; // 0x200
    u8 sendBufferCommRingServer[384]; // 0x308
    u8 *unk_488; // 0x488
    u8 *recvBufferRingServer; // 0x48C
    u8 *recvBufferRing; // 0x490
    u8 *tempBuffer; // 0x494
    CommRing sendRing; // 0x498
    CommRing recvRing; // 0x4A4
    CommRing unk_4B0[8]; // 0x4B0
    CommRing sendRingServer; // 0x510
    CommRing sendRingClient[8]; // 0x51C
    struct SysTask *unk_57C; // 0x57C
    CommQueueMan commQueueManSend; // 0x580
    CommQueueMan commQueueManSendServer; // 0x5A0
    CommRecvPackage commRecvServer[8]; // 0x5C0
    CommRecvPackage commRecvClient; // 0x620
    MATHRandContext32 rand; // 0x62C
    u16 unk_644[8]; // 0x644
    u8 recvSpeed[8]; // 0x654
    u16 sendHeldKeys; // 0x65C
    u8 unk_65E; // 0x65E
    u8 sendSpeed; // 0x65F
    u8 playerMovementState; // 0x660
    s8 randomPadKeyTimer; // 0x661
    u16 randomPadKey; // 0x662
    BOOL unk_664; // 0x664
    volatile int unk_668; // 0x668
    volatile int unk_66C[8]; // 0x66C
    int allocSize; // 0x68C
    int maxPacketSize; // 0x690
    u16 unk_694; // 0x694
    u8 unk_696[8]; // 0x696
    u8 unk_69E[8]; // 0x69E
    u8 unk_6A6[4]; // 0x6A6
    u8 unk_6AA; // 0x6AA
    u8 transmissionState; // 0x6AB
    u8 unk_6AC; // 0x6AC
    u8 transmissionType; // 0x6AD
    u8 unk_6AE; // 0x6AE
    u8 unk_6AF; // 0x6AF
    u8 unk_6B0; // 0x6B0
    u8 unk_6B1; // 0x6B1
    u8 unk_6B2; // 0x6B2
    u8 unk_6B3; // 0x6B3
    u8 unk_6B4; // 0x6B4
    u8 unk_6B5; // 0x6B5
    u8 wifiConnected; // 0x6B6 (Platinum's 0x6AF: HGSS has one byte less in 0x6AF-0x6B5)
    u8 unk_6B7; // 0x6B7
    u8 unk_6B8; // 0x6B8
    u8 unk_6B9; // 0x6B9
    u8 unk_6BA; // 0x6BA
    u8 unk_6BB; // 0x6BB
    u8 unk_6BC; // 0x6BC
    u8 unk_6BD; // 0x6BD
} CommunicationSystem; // 0x6C0

typedef struct CommSysStatics {
    u8 unk_00; // 0x0
    u8 unk_01; // 0x1
    void *unk_04; // 0x4: the Heap_Alloc'd block (sizeof(CommunicationSystem) + 0x20)
    CommunicationSystem *sCommunicationSystem; // 0x8: 32-aligned inside unk_04
} CommSysStatics; // 0xC

extern CommSysStatics _021D4140;

#endif // POKEHEARTGOLD_UNK_02035900_WORK_H
