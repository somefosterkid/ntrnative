#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"

void ov96_021F3C38(void *arg0);

void ov96_021F3C38(void *arg0) {
    s32 var_r4;
    void *var_r4_p;
    s32 var_r6;
    void *var_r5;
    void *var_r7;

    var_r7 = arg0;
    var_r4 = 0;
    var_r5 = var_r7;
    do {
        Heap_Free(*(void **)((u8 *)var_r5 + 0x98));
        Heap_Free(*(void **)((u8 *)var_r5 + 0x9C));
        var_r4 += 1;
        var_r5 += 8;
    } while (var_r4 < 0xC);
    var_r6 = 0x55;
    var_r5 = NULL;
    var_r4 = (s32)var_r7;
    var_r6 = 0x154;
    do {
        Heap_Free(*(void **)((u8 *)var_r4 + 0x154));
        var_r5 += 1;
        var_r4 += 4;
    } while ((s32)var_r5 < 4);
    Heap_Free(*(void **)((u8 *)var_r7 + 0x164));
    var_r6 = 0x5A;
    var_r5 = NULL;
    var_r4 = (s32)var_r7;
    var_r6 = 0x168;
    do {
        Heap_Free(*(void **)((u8 *)var_r4 + 0x168));
        var_r5 += 1;
        var_r4 += 4;
    } while ((s32)var_r5 < 2);
    MessageFormat_Delete(*(MessageFormat **)((u8 *)var_r7 + 0x60));
    DestroyMsgData(*(MsgData **)((u8 *)var_r7 + 0x5C));
    var_r4_p = var_r7;
    var_r5 = NULL;
    var_r4_p += 0xC;
    do {
        RemoveWindow((Window *)var_r4_p);
        var_r5 += 1;
        var_r4_p += (s32)sizeof(Window);
    } while ((s32)var_r5 < 5);
    Heap_Free(var_r7);
}
