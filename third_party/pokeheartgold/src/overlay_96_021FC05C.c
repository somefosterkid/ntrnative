#include "global.h"

#include "heap.h"
#include "error_handling.h"

void *ov96_021FBE3C(s32);

void ov96_021FC05C(s32 *arg0);

void *ov96_021EB4F4(void *, u32, u32);
u32 ov96_021FBE44(u32, s32);
void ov96_021FBE4C(u32, void *, void *);
void ov96_021FC07C(u32 *param_1, void *param_2);

s32 ov96_021FC0D0(void *arg0, u32 arg1);
s32 ov96_021FC0E4(s32 param_1);
s32 ov96_021FC0E8(s32 param_1);
u32 ov96_021FC0F4(void *);

void ov96_021FC05C(s32 *ptr) {
    s32 v0;
    s32 *v1;

    v0 = 0;
    v1 = ptr;
    do {
        ov96_021FBE3C(*v1);
        v0 += 1;
        v1 += 2;
    } while (v0 < 3);
    Heap_Free(ptr);
}

void ov96_021FC07C(u32 *param_1, void *param_2) {
    u32 uVar1;
    void *uVar2;
    void *uVar3;
    s32 iVar4;
    u32 *puStack_20;
    s32 iStack_1c;

    iStack_1c = 0;
    puStack_20 = param_1;
    do {
        iVar4 = 0;
        do {
            uVar1 = ov96_021FBE44(*puStack_20, iVar4);
            uVar2 = ov96_021EB4F4(param_2, 0x65, 2);
            uVar3 = ov96_021EB4F4(param_2, 0x67, 6);
            ov96_021FBE4C(uVar1, uVar2, uVar3);
            iVar4 = iVar4 + 1;
        } while (iVar4 < 4);
        puStack_20 = puStack_20 + 2;
        iStack_1c = iStack_1c + 1;
    } while (iStack_1c < 3);
    return;
}

s32 ov96_021FC0D0(void *arg0, u32 arg1) {
    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    return *(s32 *)(arg0 + (arg1 * 8));
}

s32 ov96_021FC0E4(s32 param_1) {
    return param_1 + 0x18;
}

s32 ov96_021FC0E8(s32 param_1) {
    return param_1 + 0x618;
}

u32 ov96_021FC0F4(void *param_1) {
    return 0x600;
}
