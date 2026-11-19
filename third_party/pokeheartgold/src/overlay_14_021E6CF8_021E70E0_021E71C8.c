#include "global.h"
#include "overlay_14_box_funcs.h"

extern u8 ov14_021F7BC0[];

u32 ov14_021E71C8(u32 param_1) {
    u32 uVar1;
    u16 *puVar2;

    puVar2 = (u16 *)ov14_021F7BC0;
    uVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return uVar1;
        }
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (uVar1 < 4);
    return 0xffffffff;
}
