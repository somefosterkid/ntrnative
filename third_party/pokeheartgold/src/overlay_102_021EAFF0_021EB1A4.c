#include "global.h"

#include "heap.h"
#include "msgdata.h"

void *ov102_021EA268(void *);

struct EasyChatApp;
struct EasyChatWordList;

void *ov102_021EB1A4(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2);

void *ov102_021EB1A4(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_35, 0x68U);
    *(void **)v0 = arg0;
    *(struct EasyChatApp **)((u8 *)v0 + 4) = arg1;
    *(struct EasyChatWordList **)((u8 *)v0 + 8) = arg2;
    *(s32 *)((u8 *)v0 + 0xC) = 0;
    *(s32 *)((u8 *)v0 + 0x64) = 0;
    *(void **)((u8 *)v0 + 0x10) = ov102_021EA268(*(void **)v0);
    *(MsgData **)((u8 *)v0 + 0x14) = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x11C, HEAP_ID_35);
    return v0;
}
