#include "global.h"

s32 ov96_021F32FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4);

s32 ov96_021F32FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    s32 sp0[6];

    sp0[3] = arg0 << 0xC;
    sp0[4] = arg1 << 0xC;
    sp0[0] = arg2 << 0xC;
    sp0[5] = 0;
    sp0[1] = arg3 << 0xC;
    sp0[2] = 0;
    VEC_Subtract((VecFx32 *)&sp0[3], (VecFx32 *)&sp0[0], (VecFx32 *)&sp0[3]);
    if ((s32)(arg4 << 0xC) > VEC_Mag((VecFx32 *)&sp0[3])) {
        return 1;
    }
    return 0;
}
