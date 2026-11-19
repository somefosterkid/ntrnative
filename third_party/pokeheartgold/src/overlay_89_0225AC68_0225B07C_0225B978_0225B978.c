#include "global.h"

s32 ov89_0225AEA8(void *, void *, u8 *, u32);

u32 ov89_0225B978(u32 param_1, u8 *param_2, u8 *param_3, u32 param_4);

u32 ov89_0225B978(u32 param_1, u8 *param_2, u8 *param_3, u32 param_4) {
    s32 iVar1;

    iVar1 = ov89_0225AEA8(param_2 + 0x9c, param_3 + 0x98, param_3 + 0x98, param_4);
    if (iVar1 == 1) {
        return 1;
    }
    return 0;
}
