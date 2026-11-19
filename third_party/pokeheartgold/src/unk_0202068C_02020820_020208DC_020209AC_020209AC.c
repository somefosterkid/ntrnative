#include "global.h"

#include "unk_0202068C.h"

void sub_0202094C(void *, void *);

void sub_020209AC(u8 *param_1, u32 param_2) {
    u32 new_var;
    u8 *iVar1;
    if (param_1 != 0) {
        iVar1 = param_1 + 0x3c;
        if ((iVar1 != 0) && (param_2 < (*((u8 *)(param_1 + 0x3d))))) {
            iVar1 = ((iVar1 + ((u32)(*((u16 *)(param_1 + 0x42))))) + 4) + ((*((u16 *)(iVar1 + ((u32)(*((u16 *)(param_1 + 0x42))))))) * (new_var = param_2));
        } else {
            iVar1 = 0;
        }
    } else {
        iVar1 = 0;
    }
    sub_0202094C(param_1, iVar1);
    return;
}
