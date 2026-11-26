#include "global.h"

#include "overlay_42.h"
#include "overlay_49.h"

s32 ov49_022593FC(s32 *arg0);

s32 ov49_022593FC(s32 *arg0) {
    if (*arg0 == 0) {
        return 1;
    }
    return 0;
}

void ov49_0225940C(void) {
}

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov49_0225927C(void *, s16, s8, s8);
#else
s32 ov49_0225927C(void *, s32, s32, s32);
#endif

void ov49_02259410(UnkStruct_ov42_02228110 **arg0, void *arg1);

void ov49_02259410(UnkStruct_ov42_02228110 **arg0, void *arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = ov42_02228188(*arg0, 6);
    v1 = ov42_02228188(*arg0, 4);
    if (ov42_02228188(*arg0, 5) == 0) {
        v2 = 2;
        if (gSystem.heldKeys & 2) {
            v2 = 3;
        }
        if (0x40 & gSystem.heldKeys) {
            if (v0 == 0) {
                ov49_0225927C(arg1, v2, v0, v1);
                return;
            }
            ov49_0225927C(arg1, 1, 0, v1);
            return;
        }
        if (0x80 & gSystem.heldKeys) {
            if (v0 == 1) {
                ov49_0225927C(arg1, v2, v0, v1);
                return;
            }
            ov49_0225927C(arg1, 1, 1, v1);
            return;
        }
        if (0x20 & gSystem.heldKeys) {
            if (v0 == 2) {
                ov49_0225927C(arg1, v2, v0, v1);
                return;
            }
            ov49_0225927C(arg1, 1, 2, v1);
            return;
        }
        if (gSystem.heldKeys & 0x10) {
            if (v0 == 3) {
                ov49_0225927C(arg1, v2, v0, v1);
                return;
            }
            ov49_0225927C(arg1, 1, 3, v1);
        }
    }
}
