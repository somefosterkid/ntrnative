#include "global.h"

#include "unk_02035900.h"

void ov85_021E9D9C(void *, u32, u8);

void sub_02096ED0(u32 param_1, u32 param_2, u8 *param_3, u8 **param_4);

void sub_02096ED0(u32 param_1, u32 param_2, u8 *param_3, u8 **param_4) {
    u16 uVar1;

    ov85_021E9D9C(*(void **)(*param_4 + 0x38), 1, *param_3);
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        param_4[0xd] = 0;
    }
    return;
}
