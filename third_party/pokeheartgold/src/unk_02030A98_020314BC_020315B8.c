#include "global.h"

#include "pm_string.h"

String *sub_020315B8(u16 *arg0, enum HeapID arg1);

String *sub_020315B8(u16 *src, enum HeapID heapID) {
    String *dest;

    dest = String_New(0xFU, heapID);
    CopyU16ArrayToStringN(dest, src, 0xFU);
    return dest;
}

u8 sub_020315D0(void *arg0);

u8 sub_020315D0(void *arg0) {
    u8 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0x14);
    if ((v0 != 0) && (v0 != 1)) {
        v0 = 0;
    }
    return v0;
}

u16 sub_020315E0(void *arg0);

u16 sub_020315E0(void *arg0) {
    u16 v0;

    v0 = *(u16 *)((u8 *)arg0 + 0x1C);
    if ((u32)v0 >= 0x1EFU) {
        v0 = 0;
    }
    return v0;
}
