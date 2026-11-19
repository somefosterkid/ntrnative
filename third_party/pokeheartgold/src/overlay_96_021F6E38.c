#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F6E68(UnkStruct_ov96_021F5018 *param_1, s32 param_2);

void ov96_021F6E38(UnkStruct_ov96_021F5018 *param_1);

void ov96_021F6E38(UnkStruct_ov96_021F5018 *param_1) {
    u32 uVar1;
    s32 iVar2;
    u16 *puVar4;

    uVar1 = 0;
    iVar2 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        puVar4 = &param_1->unk_90[iVar2].unk_20;
        iVar2 = iVar2 + 1;
        uVar1 = uVar1 + *puVar4 & 0xffff;
    } while (iVar2 < 3);
#else
    u8 *iVar3 = (u8 *)param_1;
    do {
        puVar4 = (u16 *)(iVar3 + 0xb0);
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 0x38;
        uVar1 = uVar1 + *puVar4 & 0xffff;
    } while (iVar2 < 3);
#endif
    if (999 < uVar1) {
        uVar1 = 999;
    }
    ov96_021F6E68(param_1, uVar1);
    return;
}
