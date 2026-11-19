#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "math_util.h"

void ov96_0220FB98(UnkStruct_ov96_0220E3F0_Record *param_1, u8 param_2, u8 param_3);

s32 ov96_0220FF68(u32 param_1);

void ov96_0220FF88(UnkStruct_ov96_0220E3F0_Score *param_1, u32 param_2);

void ov96_0220FF88(UnkStruct_ov96_0220E3F0_Score *param_1, u32 param_2) {
    u32 new_var4;
    UnkStruct_ov96_0220E3F0_Score *new_var3;
    u32 uVar1;
    u32 uVar2;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    s32 *piVar3; // DS: the loop walks a pointer to records[i].unk_4
#endif
    u8 cVar4;
    u32 *new_var;
    s32 iVar6;
    s32 iVar7;
    iVar7 = 1;
    uVar1 = ov96_0220FF68(param_2);
    new_var = &param_1->unk_1C;
    param_1->unk_1C = ((*new_var) & 0xfffff0ff) | ((uVar1 << 0x1c) >> 0x14);
    switch ((new_var4 = param_1->unk_1C << 0x14) >> 0x1c) {
    case 0:

    case 2:
        *new_var = ((*new_var) & 0xffffff00) | 0x1e;
        break;

    case 1:

    case 3:
        *new_var = (*new_var) & 0xffffff00;
    }

    if (((param_1->unk_1C << 0x14) >> 0x1c) < 3) {
        cVar4 = 4;
    } else {
        uVar2 = MTRandom();
        cVar4 = ((uVar2 & 1) + 1) * 4;
    }
    if (((param_1->unk_1C << 0x14) >> 0x1c) != 0) {
        iVar7 = 2;
    }
    iVar6 = 0;
    if (iVar7 > 0) {
#if !defined(SDK_PORT) && !defined(NONMATCHING)
        piVar3 = &param_1->records[0].unk_4;
#endif
        do {
#if defined(SDK_PORT) || defined(NONMATCHING)
            if (!(((u32)(param_1->records[iVar6].unk_4 << 0x1a)) >> 0x1f)) {
#else
            if (!(((u32)(*piVar3 << 0x1a)) >> 0x1f)) {
#endif
                ov96_0220FB98(&(new_var3 = param_1)->records[iVar6], cVar4, iVar6);
                return;
            }
            iVar6 = iVar6 + 1;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
            piVar3 = (s32 *)((u8 *)piVar3 + sizeof(UnkStruct_ov96_0220E3F0_Record));
#endif
        } while (iVar6 < iVar7);
    }
    return;
}
