#include "global.h"

u32 ov45_0222E540(u8 *arg0, u32 arg1);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};
BOOL ov45_0222E5B4(enum PlazaMinigame param0, u32 param1);

u32 ov45_0222E540(u8 *arg0, u32 arg1) {
    if (arg1 >= 2U) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

u32 ov45_0222E550(u8 *arg0, u32 arg1);

u32 ov45_0222E550(u8 *arg0, u32 arg1) {
    if (arg1 >= 2U) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

u32 ov45_0222E560(void *arg0, u32 arg1);

u32 ov45_0222E560(void *arg0, u32 arg1) {
    if ((u32) * (u32 *)((u8 *)arg0 + 8) <= arg1) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

u32 ov45_0222E574(u8 *arg0, u32 arg1);

u32 ov45_0222E574(u8 *arg0, u32 arg1) {
    if (arg1 >= 1U) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

u32 ov45_0222E584(void *arg0, u32 arg1);

u32 ov45_0222E584(void *arg0, u32 arg1) {
    if ((u32) * (u32 *)((u8 *)arg0 + 4) <= arg1) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

u32 ov45_0222E598(void);

u32 ov45_0222E598(void) {
    return 0x14;
}

u32 ov45_0222E59C(void);

u32 ov45_0222E59C(void) {
    return 0x14;
}

u32 ov45_0222E5A0(void *arg0, u32 arg1);

u32 ov45_0222E5A0(void *arg0, u32 arg1) {
    if ((u32) * (u32 *)((u8 *)arg0 + 8) <= arg1) {
        return 0x14U;
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20);
}

BOOL ov45_0222E5B4(enum PlazaMinigame param0, u32 param1) {
    if ((param0 == UnkEnum_ov66_022324D0_00) || (param0 == UnkEnum_ov66_022324D0_01)) {
        if ((param1 < 1) || (param1 > 4)) {
            return 0;
        }
    } else {
        if ((param1 < 2) || (param1 > 4)) {
            return 0;
        }
    }

    return 1;
}
