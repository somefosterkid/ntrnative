// Black's code at main 0x02087f60-0x02088168, among TwlSDK's OS files: the DeliverArg API (os_argument.c), a block in
// main memory that the ARM7 shares (0x300 bytes, a CRC-16 over it) and a 64-bit value kept in the LTD main memory.
// Its file is not known.

#include "types.h"
#include "gfl/std.h"
#include "nitro/os.h"

#pragma thumb off

#define OS_DELIVER_ARG_BUFFER_SIZE 0x2ec
#define OS_DELIVER_ARG_ENCODE_FLAG 1
#define OS_DELIVER_ARG_VALID_FLAG 2

static u8 *sPtr;
static OSDeliverArgInfo *sBufferPtr = NULL;
static u32 sBufferState = 0;
static u64 sEncodedTitleId = 0;
BOOL func_02088150(void);
u16 GetCRC16(u32 init, void *data, u32 size);
void OS_FinalizeDeliverArg(void);

void OS_InitDeliverArg(void *buf, u16 binSize) {
    sBufferPtr = (OSDeliverArgInfo *)buf;
    MI_CpuFill8(buf, 0, 0x300);
    sBufferState = 3;
    sBufferPtr->header.argBufferSize = OS_DELIVER_ARG_BUFFER_SIZE - binSize;
    sBufferPtr->header.binarySize = 0;
    sPtr = &sBufferPtr->buf[0];
    *sPtr = 0;
    *(sPtr + 1) = 0;
}

s32 OS_EncodeDeliverArg(void) {
    if (!(sBufferState & 1)) {
        return -1;
    }
    sBufferPtr->header.makerCode = *(u16 *)0x02ffe010;
    sBufferPtr->header.titleId = *(u64 *)(0x02ffe010 + 0x220);
    sBufferPtr->header.flag = OS_DELIVER_ARG_ENCODE_FLAG | OS_DELIVER_ARG_VALID_FLAG;
    sEncodedTitleId = sBufferPtr->header.titleId;
    sBufferPtr->header.crc = 0;
    sBufferPtr->header.crc = GetCRC16(0xffff, sBufferPtr, 0x300);
    MI_CpuCopy8(sBufferPtr, (void *)0x02000000, 0x300);
    MI_CpuFill8(sBufferPtr, 0, 0x300);
    sBufferState = 0;
    return 0;
}

s32 OS_DecodeDeliverArg(void) {
    u16 crc;

    if (!func_02088150()) {
        return -1;
    }
    if (sBufferPtr == NULL) {
        return -1;
    }
    MI_CpuCopy8((void *)0x02000000, sBufferPtr, 0x300);
    crc = sBufferPtr->header.crc;
    sBufferPtr->header.crc = 0;
    sBufferPtr->header.crc = GetCRC16(0xffff, sBufferPtr, 0x300);
    if (crc != sBufferPtr->header.crc) {
        MI_CpuFill8(sBufferPtr, 0, 0x300);
        OS_FinalizeDeliverArg();
        return 0;
    }
    sBufferPtr->header.flag &= ~OS_DELIVER_ARG_ENCODE_FLAG;
    sBufferState = 1;
    return 0;
}

void OS_FinalizeDeliverArg(void) {
    sBufferState = 0;
}

u64 OS_GetTitleIdFromDeliverArg(void) {
    if (sBufferState & 1) {
        return sBufferPtr->header.titleId;
    }
    return 0;
}

BOOL func_02088150(void) {
    u8 *p = (u8 *)0x02000000;
    return (p[9] & 2) != 0;
}
