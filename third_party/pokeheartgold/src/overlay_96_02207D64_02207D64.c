#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

void ov96_02207D64(void *arg0);

void ov96_02207D64(void *arg0) {
    void *sp0;
    s32 var_r4;
    void *var_r4_p;
    s32 var_r6;
    s32 var_r7;
    void *var_r5;

    var_r7 = 0x42;
    var_r7 = 0x108;
    sp0 = arg0;
    var_r4 = 0;
    var_r5 = arg0;
    var_r6 = 0x10C;
    do {
        Heap_Free(*(void **)((u8 *)var_r5 + 0x108));
        Heap_Free(*(void **)((u8 *)var_r5 + 0x10C));
        var_r4 += 1;
        var_r5 += 8;
    } while (var_r4 < 3);
    var_r4 = (s32)sp0;
    var_r5 = NULL;
    do {
        Heap_Free(*(void **)((u8 *)var_r4 + 0x9C));
        var_r5 += 1;
        var_r4 += 4;
    } while ((s32)var_r5 < 0xC);
    Heap_Free(*(void **)((u8 *)sp0 + 0x24));
    var_r6 = 0x6A;
    var_r4_p = sp0;
    var_r5 = NULL;
    var_r6 = 0x1A8;
    do {
        String_Delete(*(String **)((u8 *)var_r4_p + 0x1A8));
        var_r5 += 1;
        var_r4_p += 4;
    } while ((s32)var_r5 < 3);
    MessageFormat_Delete(*(MessageFormat **)((u8 *)sp0 + 0x20));
    DestroyMsgData(*(MsgData **)((u8 *)sp0 + 0x1C));
    RemoveWindow(sp0 + 0xC);
    Heap_Free(sp0);
}
