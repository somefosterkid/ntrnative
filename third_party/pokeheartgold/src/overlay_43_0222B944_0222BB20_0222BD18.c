#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"

s32 ov43_0222AD00(s32, s32);

s32 ov43_0222BC78(void *, void *, s32, s32);

s32 ov43_0222C844(void *, s32, s32);

s32 ov43_0222BD18(void *arg0, void *arg1, s32 arg2);

s32 ov43_0222BD18(void *arg0, void *arg1, s32 arg2) {
    s32 sp8;
    s32 sp4;
    s32 sp0;
    s16 temp_r0_3;
    s16 temp_r1;
    s32 var_r4;
    u8 temp_r0;

    if (0xCF3 & gSystem.newKeys) {
        *(s32 *)arg1 = 0;
    }
    if (1 & gSystem.newKeys) {
        return ov43_0222BC78(arg0, arg1, arg2, 5);
    }
    if (gSystem.newKeys & 2) {
        *(s16 *)((u8 *)arg0 + 4) = 8;
        ov43_0222C844(arg0 + 8, arg2, *(s16 *)((u8 *)arg0 + 4));
        return ov43_0222BC78(arg0, arg1, arg2, 0);
    }
    temp_r1 = *(s16 *)((u8 *)arg0 + 4);
    var_r4 = 0;
    sp8 = (s32)(u16)temp_r1;
    sp4 = 0;
    if (0x40 & gSystem.newAndRepeatedKeys) {
        if (temp_r1 == 8) {
            var_r4 = 1;
            *(s16 *)((u8 *)arg0 + 4) = (s16) * (s16 *)((u8 *)arg0 + 6);
            ov43_0222AD00(arg2, 1);
        } else if ((s32)temp_r1 < 4) {
            if ((s32)temp_r1 > 0) {
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 - 1);
                var_r4 = 1;
            }
        } else if ((s32)temp_r1 > 4) {
            *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 - 1);
            var_r4 = 1;
        }
    } else if (0x80 & gSystem.newAndRepeatedKeys) {
        if ((temp_r1 == 3) || (temp_r1 == 7)) {
            *(s16 *)((u8 *)arg0 + 4) = 8;
            var_r4 = 1;
        } else if ((s32)temp_r1 < 4) {
            if ((s32)temp_r1 < 3) {
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 + 1);
                var_r4 = 1;
            }
        } else if ((s32)temp_r1 < 7) {
            *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 + 1);
            var_r4 = 1;
        }
    } else if (0x20 & gSystem.newAndRepeatedKeys) {
        if (temp_r1 != 8) {
            if ((s32)temp_r1 < 4) {
                var_r4 = 1;
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 + 4);
                sp4 = 1;
                sp0 = 0;
            } else {
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 - 4);
                var_r4 = 1;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & 0x10) {
        if (temp_r1 != 8) {
            if ((s32)temp_r1 < 4) {
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 + 4);
                var_r4 = 1;
            } else {
                var_r4 = 1;
                *(s16 *)((u8 *)arg0 + 4) = (s16)(temp_r1 - 4);
                sp4 = 1;
                sp0 = 1;
            }
        }
    } else {
        temp_r0 = *((u8 *)&((UnkStruct_ov64_0222E060 *)arg1)->pad_C_[2]);
        if (temp_r0 != 0) {
            if ((temp_r0 != 0) && (0x200 & gSystem.newAndRepeatedKeys)) {
                sp4 = 1;
                sp0 = 0;
            } else if ((temp_r0 != 0) && (0x100 & gSystem.newAndRepeatedKeys)) {
                sp4 = 1;
                sp0 = 1;
            }
        }
    }
    if (sp4 == 1) {
        if (sp0 != 0) {
            *(s16 *)((u8 *)arg0 + 2) = (s16) * (s16 *)arg0;
            *(s16 *)arg0 = (s16)((*(s16 *)arg0 + 1) % 4);
            return ov43_0222BC78(arg0, arg1, arg2, 2);
        }
        *(s16 *)((u8 *)arg0 + 2) = (s16) * (s16 *)arg0;
        *(s16 *)arg0 = (s16)(*(s16 *)arg0 - 1);
        temp_r0_3 = *(s16 *)arg0;
        if ((s32)temp_r0_3 < 0) {
            *(s16 *)arg0 = (s16)(temp_r0_3 + 4);
        }
        return ov43_0222BC78(arg0, arg1, arg2, 1);
    }
    if (var_r4 == 1) {
        PlaySE(0x5E5U);
        ov43_0222C844(arg0 + 8, arg2, *(s16 *)((u8 *)arg0 + 4));
        *(s16 *)((u8 *)arg0 + 6) = (s16)sp8;
    }
    return 0;
}
