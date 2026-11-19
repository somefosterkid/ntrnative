#include "global.h"

#include "heap.h"
#include "unk_02035900.h"

s32 sub_02037190(void);

s32 sub_02075554(void *, void *, void *);

s32 sub_0207527C(void **arg0);

s32 sub_0207527C(void **arg0) {
    s32 var_r0;
    s32 var_r4;
    void **var_r5;
    void *temp_r4;

    var_r5 = arg0;
    if (sub_02037190() != 0x108) {
        return 0;
    }
    temp_r4 = *var_r5;
    if (0x80 & *(s32 *)temp_r4) {
        var_r0 = sub_0203769C() * 8;
    } else {
        var_r0 = sub_0203769C() * 4;
    }
    sub_02075554(*(void **)((u8 *)(temp_r4 + var_r0) + 0xF8), *(void **)((u8 *)temp_r4 + 0x148), ((u8 *)var_r5) + 0x20);
    var_r4 = 0;
    do {
        *(void **)((u8 *)var_r5 + 0x10) = Heap_Alloc(HEAP_ID_BATTLE, 0x88U);
        var_r4 += 1;
        var_r5 = (void *)((u8 *)var_r5 + (4));
    } while (var_r4 < 4);
    return 1;
}
