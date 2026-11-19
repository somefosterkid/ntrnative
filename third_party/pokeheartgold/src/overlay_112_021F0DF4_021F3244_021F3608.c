#include "global.h"

#include "save.h"
#include "save_vars_flags.h"

extern u32 ov112_021FF4D4;

void ov112_021F3608(SaveData *param_1, u8 *param_2);

void ov112_021F3608(SaveData *param_1, u8 *param_2) {
    SaveVarsFlags *varsFlags;
    BOOL BVar1;
    s32 iVar2;
    u16 *puVar3;

    varsFlags = Save_VarsFlags_Get(param_1);
    puVar3 = (u16 *)&ov112_021FF4D4;
    iVar2 = 0;
    do {
        BVar1 = Save_VarsFlags_CheckFlagInArray(varsFlags, *puVar3);
        *(s8 *)(param_2 + iVar2) = (s8)BVar1;
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 1;
    } while (iVar2 < 10);
    return;
}
