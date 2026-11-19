#include "global.h"

typedef struct {
    BOOL unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_0223CAA4;

void ov39_02227F74(void *, void *);

void *ov40_0223D540(void *);

void ov40_0223A324(void *param0, UnkStruct_ov62_0223CAA4 *param1);

void ov40_0223A324(void *param0, UnkStruct_ov62_0223CAA4 *param1) {
    void *uVar1;
    u8 *iVar2;

    iVar2 = *(u8 **)((u8 *)param0 + 0x860);
    if (param1->unk_00 == 1) {
        *(u8 **)(iVar2 + 0x71c) = iVar2 + 0x710;
        return;
    }
    uVar1 = ov40_0223D540(param0);
    ov39_02227F74(uVar1, iVar2 + 0x71c);
    MI_CpuCopy8(*(void **)(iVar2 + 0x71c), (void *)(iVar2 + 0x710), 4);
    return;
}
