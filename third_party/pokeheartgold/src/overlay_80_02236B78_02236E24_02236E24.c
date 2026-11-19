#include "global.h"

#include "frontier/overlay_80_02229EE0.h"

#include "overlay_80_02236B78.h"

void *ov80_02236C78(int round, int isOpenLevel);

void ov80_02236C9C(s32, s32, u32, u32, u16 *, u32, void *, u16, u8 *);

void ov80_02236E24(u32 param_1, u32 param_2, u16 *param_3, void *param_4, u8 *param_5, void *param_6, u16 param_7, s32 param_8) {
    void *uVar1;

    uVar1 = ov80_02236C78(param_1, param_2);
    if (param_8 == 0) {
        ov80_02236C9C(0, 0, 0, 6, param_3, 0xb, uVar1, param_7, param_5);
    } else {
        ov80_02236C9C(param_8, param_8, 6, 6, param_3, 0xb, uVar1, param_7, param_5);
    }
    ov80_0222A52C(param_4, param_3, param_5, (u32 *)0x0, param_6, 6, 0xb, 0xcd);
    return;
}
