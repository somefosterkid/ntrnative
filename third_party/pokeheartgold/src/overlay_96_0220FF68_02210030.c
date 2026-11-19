#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "pokeathlon/pokeathlon.h"

void ov96_0220FE38(UnkStruct_ov96_0220E3F0_Record *param_1, u16 *param_2, UnkStruct_ov96_0220E3F0_Entry *param_3);

void ov96_0220FF88(UnkStruct_ov96_0220E3F0_Score *param_1, u32 param_2);

void ov96_02210030(UnkStruct_ov96_0220E3F0_Score *param_1, u8 *param_2, u32 param_3);

void ov96_02210030(UnkStruct_ov96_0220E3F0_Score *param_1, u8 *param_2, u32 param_3) {
    u16 *puVar1;
    u32 uVar2;
    UnkStruct_ov96_0220E3F0_Record *piVar3;
    s32 iVar4;

    puVar1 = (u16 *)ov96_021E8A20((void *)(param_2 + 0x28));
    uVar2 = param_1->unk_1C & 0xff;
    if (uVar2 == 0) {
        ov96_0220FF88(param_1, param_3);
    } else {
        param_1->unk_1C = param_1->unk_1C & 0xffffff00U | uVar2 - 1 & 0xff;
    }
    iVar4 = 0;
    piVar3 = param_1->records;
    do {
        ov96_0220FE38(piVar3, puVar1, param_1->entries);
        puVar1 = puVar1 + 1;
        iVar4 = iVar4 + 1;
        piVar3++;
    } while (iVar4 < 2);
    return;
}
