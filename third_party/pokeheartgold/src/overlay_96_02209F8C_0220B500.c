#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220B500(void *param_1);

void ov96_0220B500(void *param_1) {
    s32 iVar1;
    void *pvVar2;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    iVar1 = 0;
    pvVar2 = param_1;
    do {
        if (*(ManagedSprite **)((u8 *)pvVar2 + 0x10) != NULL) {
            Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)pvVar2 + 0x10));
        }
        iVar1 = iVar1 + 1;
        pvVar2 = (void *)((u8 *)pvVar2 + 4);
    } while (iVar1 < 9);
    Heap_Free(param_1);
    return;
}
