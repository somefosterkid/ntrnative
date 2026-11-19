#include "global.h"

u32 ov96_021FF574(s32 *param_1, u8 *param_2);

u32 ov96_021FF574(s32 *param_1, u8 *param_2) {
    s32 iVar1;
    s32 *new_var2;
    s32 new_var;
    s32 iVar2;
    s32 new_var3;
    new_var = *param_1;
    new_var2 = (s32 *)(param_2 + 4);
    new_var3 = param_1[1];
    iVar1 = (*new_var2) * 0x1000;
    iVar2 = (*((s32 *)(param_2 + 8))) * 0x1000;
    if ((((iVar1 < (*param_1)) && (new_var < (iVar1 + 0x40000))) && (iVar2 < new_var3)) && (new_var3 < (iVar2 + 0x40000))) {
        return 1;
    }
    return 0;
}
