#include "global.h"

#include "save.h"

void sub_0202E498(u8 *param_1);

void sub_0202E498(u8 *param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        *(u8 *)(param_1 + iVar1) = 0;
        iVar1 = iVar1 + 1;
    } while (iVar1 < 4);
    SaveSubstruct_UpdateCRC(0x17);
    return;
}
