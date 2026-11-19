#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220B8A0(void *param_1);

void ov96_0220B8A0(void *param_1) {
    s32 iVar1;
    void *pvVar2;
    void *pvVar3;
    s32 iVar4;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    iVar4 = 0;
    pvVar3 = param_1;
    do {
        iVar1 = 0;
        pvVar2 = pvVar3;
        do {
            if (*(ManagedSprite **)((u8 *)pvVar2 + 0x30) != NULL) {
                Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)pvVar2 + 0x30));
            }
            iVar1 = iVar1 + 1;
            pvVar2 = (void *)((u8 *)pvVar2 + 4);
        } while (iVar1 < 6);
        iVar4 = iVar4 + 1;
        pvVar3 = (void *)((u8 *)pvVar3 + 0x3c);
    } while (iVar4 < 3);
    Heap_Free(param_1);
    return;
}

s32 ov96_0220C40C(void *);

void ov96_0220B8D8(u8 *arg0);

void ov96_0220B8D8(u8 *arg0) {
    s32 v0;
    u8 *v1;

    v0 = 0;
    v1 = arg0 + 0x14;
    do {
        ov96_0220C40C(v1);
        v0 += 1;
        v1 += 0x3C;
    } while (v0 < 3);
}
