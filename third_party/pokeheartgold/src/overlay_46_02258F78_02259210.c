#include "global.h"

void ov46_02259210(void *arg0, void *arg1);

void ov46_02259210(void *arg0, void *arg1) {
    u32 temp_r0;

    if ((*(s32 *)((u8 *)arg1 + 4) != 0) && (*(u8 *)((u8 *)arg1 + 0x32B) = (u8)(*(u8 *)((u8 *)arg1 + 0x32B) ^ 1), ((1 & *(u8 *)((u8 *)arg1 + 0x32B)) == 0))) {
        if ((u32) * (u32 *)((u8 *)arg1 + 0x32C) <= 1U) {
            GX_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        temp_r0 = *(u32 *)((u8 *)arg1 + 0x32C);
        if ((temp_r0 == 0) || (temp_r0 == 2)) {
            GXS_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        if (*(s8 *)((u8 *)arg1 + 0x32A) == 0) {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) + 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) >= 0x15) {
                *(s16 *)((u8 *)arg1 + 0x328) = 0x13;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(1 ^ *(s8 *)((u8 *)arg1 + 0x32A));
            }
        } else {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) - 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) < 0) {
                *(s16 *)((u8 *)arg1 + 0x328) = 1;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(*(s8 *)((u8 *)arg1 + 0x32A) ^ 1);
            }
        }
    }
}
