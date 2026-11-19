#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"

s32 ov96_021ED6E8(void);

void *ov96_021ED054(enum HeapID arg0, void *arg1, void *arg2);

void *ov96_021ED054(enum HeapID heapID, void *arg1, void *arg2) {
    void *dest;

    if (arg1 == 0) {
        GF_AssertFail();
    }
    if (arg2 == 0) {
        GF_AssertFail();
    }
    dest = Heap_Alloc(heapID, 0x180U);
    MI_CpuFill8(dest, 0U, 0x180U);
    *(enum HeapID *)dest = heapID;
    *(void **)((u8 *)dest + 0x16C) = arg1;
    *(void **)((u8 *)dest + 0x170) = arg2;
    *(s32 *)((u8 *)dest + 0x178) = ov96_021ED6E8();
    return dest;
}

void ov96_021ED09C(void *arg0);

void ov96_021ED09C(void *ptr) {
    ManagedSprite *managedSprite;
    s32 v0;
    void *v1;

    if (ptr == NULL) {
        GF_AssertFail();
    }
    v0 = 0;
    v1 = ptr;
    do {
        managedSprite = *(ManagedSprite **)((u8 *)v1 + 4);
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
            *(ManagedSprite **)((u8 *)v1 + 4) = NULL;
        }
        v0 += 1;
        v1 += 0x18;
    } while (v0 < 0xF);
    Heap_Free(ptr);
}
