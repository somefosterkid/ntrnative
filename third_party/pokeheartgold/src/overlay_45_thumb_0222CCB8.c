#include "global.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"

struct SaveData;


void ov45_0222CCB8(UnkStruct_ov66_02230A6C *param0);

u16 ov45_0222CCDC(void *arg0);

extern void *ov45_02254A84;
s32 ov45_0222CCE4(u32 param_1);

void ov45_0222CCB8(UnkStruct_ov66_02230A6C *param0) {
    u32 v0 = SaveArray_CalcCRC16(param0->saveData, param0, (sizeof(UnkStruct_ov66_02230A6C) - 8));

    if (v0 != param0->unk_48) {
        ((0) ? (void)0 : GF_AssertFail());
        param0->unk_4A = 1;
    }
}

u16 ov45_0222CCDC(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4A);
}

s32 ov45_0222CCE4(u32 param_1) {
    s32 iVar1;
    u16 *puVar2;

    puVar2 = (u16 *)&ov45_02254A84;
    iVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return iVar1;
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 2;
    } while (iVar1 < 0x10);
    return 0x10;
}
