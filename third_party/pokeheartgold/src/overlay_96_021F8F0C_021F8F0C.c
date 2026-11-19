#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "unk_02013534.h"

s32 ov96_021F91CC(void *);

void ov96_021F8F0C(void *arg0);

void ov96_021F8F0C(void *arg0) {
    Window *temp_r4;
    s32 var_r4;
    u8 *var_r5;

    var_r4 = 0;
    var_r5 = (u8 *)(arg0 + 0x34);
    do {
        ov96_021F91CC(var_r5);
        var_r4 += 1;
        var_r5 += 0x28;
    } while (var_r4 < 4);
    temp_r4 = arg0 + 0xC;
    sub_02013938(*(UnkStruct_02013910 **)((u8 *)temp_r4 + 0x10));
    RemoveWindow(temp_r4);
    sub_020135AC(*(UnkStruct_02013534 **)((u8 *)arg0 + 8));
    Heap_Free(arg0);
}
