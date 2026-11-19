#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_112_unk_struct_ov112_021_eb28_c.h"
#include "overlay_112_hitbox.h"

s32 ov112_021EB9C0(void);

s32 ov112_021EB9E4(void);

s32 ov112_021EA51C(UnkStruct_ov112_021EB28C *, s32);

s32 ov112_021EB90C(UnkStruct_ov112_021EB28C *, s32);

s32 ov112_021EB950(UnkStruct_ov112_021EB28C *, s32);

s32 ov112_021EC440(UnkStruct_ov112_021EB28C *, s32, s32, s32);

s32 ov112_021EB28C(UnkStruct_ov112_021EB28C *arg0, s32 arg1);

s32 ov112_021EB28C(UnkStruct_ov112_021EB28C *arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r1;
    temp_r0 = ov112_021E9888(1);
    if (temp_r0 != (-1)) {
        ov112_021EC440(arg0, temp_r0, temp_r0 + 2, 9);
        PlaySE(0x5DDU);
        return 0x19;
    }
    if (0x40 & gSystem.newKeys) {
        if (arg0->sel != 0) {
            PlaySE(0x5DCU);
        }
        ov112_021EA51C(arg0, 2);
        arg0->sel = 0;
    } else if (0x80 & gSystem.newKeys) {
        if (arg0->sel != 1) {
            PlaySE(0x5DCU);
        }
        ov112_021EA51C(arg0, 3);
        arg0->sel = 1;
    } else if (1 & gSystem.newKeys) {
        PlaySE(0x5DDU);
        temp_r1 = arg0->sel;
        ov112_021EC440(arg0, temp_r1, temp_r1 + 2, 9);
        return 0x19;
    } else if (gSystem.newKeys & 2) {
        PlaySE(0x5DDU);
        ov112_021EC440(arg0, 1, 3, 9);
        return 0x19;
    } else if (ov112_021EB9C0() != 0) {
        PlaySE(0x5DCU);
        ov112_021EB90C(arg0, 3);
        return 0x1A;
    } else if ((ov112_021EB9E4() != 0) && (ov112_021EB950(arg0, 3) != 0)) {
        PlaySE(0x5DCU);
        return 0x1A;
    }
    return 8;
}
