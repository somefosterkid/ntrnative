#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"

extern u8 gSystem;

u32 ov14_021EB1E0(void *param_1);

u32 ov14_021EB1E0(void *param_1) {
    BOOL BVar1;

    BVar1 = System_GetTouchNew();
    if (BVar1 == 1) {
        PlaySE(0x5dd);
        return *(u32 *)(param_1 + 0x30);
    }
    if ((((u32 *)&gSystem)[0x12] & 3) != 0) {
        PlaySE(0x5dd);
        return *(u32 *)(param_1 + 0x30);
    }
    return 6;
}
