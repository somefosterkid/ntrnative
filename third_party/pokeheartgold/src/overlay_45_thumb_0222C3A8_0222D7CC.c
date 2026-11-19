#include "global.h"

#include "heap.h"
#include "overlay_00_thumb.h"
#include "unk_02037C94.h"

s32 ov45_0222D7CC(u32 arg0, u32 arg1);

s32 ov45_0222D7CC(u32 arg0, u32 arg1) {
    s32 v0;

    v0 = ov00_021E6A70(arg0, arg1);
    if ((v0 == 0xB) || (arg0 == 0x19)) {
        if (arg1 != 2) {
            return 0xE;
        }
        return 0xB;
    }
    if (arg0 == 0x1A) {
        return 0xD;
    }
    if (v0 < 0) {
        v0 = 0xB;
    }
    return v0;
}

s32 ov45_0222D7FC(u32 arg0, u32 arg1);

s32 ov45_0222D7FC(u32 arg0, u32 arg1) {
    s32 v0;

    v0 = ov00_021E6A70(arg0, arg1);
    if (arg0 == 0x19) {
        v0 = 0xB;
    } else if (arg0 == 0x1A) {
        v0 = 0xC;
    }
    switch (v0) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return 0;
    default:
        return 1;
    }
}

s32 ov45_0222D844(void);

s32 ov45_0222D844(void) {
    if ((sub_020393C8() != 0) || (sub_020397FC() != 0)) {
        return 1;
    }
    return 0;
}

s32 ov45_0222DE1C(void *);

s32 ov45_0222DF78(void *, enum HeapID);

void *ov45_0222D860(enum HeapID arg0);

void *ov45_0222D860(enum HeapID heapID) {
    void *v0;

    v0 = Heap_Alloc(heapID, 0x230U);
    memset(v0, 0, 0x230U);
    ov45_0222DE1C(v0 + 8);
    ov45_0222DF78(v0 + 0x5C, heapID);
    return v0;
}
