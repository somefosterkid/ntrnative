#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"

s32 ov96_021F4558(void *);

void *ov96_021F3BF0(enum HeapID arg0, s32 arg1, void *arg2);

void *ov96_021F3BF0(enum HeapID heapID, s32 arg1, void *arg2) {
    void *dest;

    dest = Heap_Alloc(heapID, 0x174U);
    MI_CpuFill8(dest, 0U, 0x174U);
    *(enum HeapID *)dest = heapID;
    *(s32 *)((u8 *)dest + 8) = arg1;
    *(void **)((u8 *)dest + 4) = arg2;
    ov96_021F4558(dest);
    *(MsgData **)((u8 *)dest + 0x5C) = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    *(MessageFormat **)((u8 *)dest + 0x60) = MessageFormat_New(heapID);
    return dest;
}
