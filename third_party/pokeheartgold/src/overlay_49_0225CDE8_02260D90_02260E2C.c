#include "global.h"

#include "error_handling.h"
#include "system.h"
#include "unk_02005D10.h"

s32 ov45_0222A330(s32);

s32 ov45_0222A374(s32);

s32 ov45_0222A5E8(s32, s32);

s32 ov45_0222ADB8(s32, s32, int);

s32 ov45_0222ADC8(s32, s32);

s32 ov45_0222AE44(s32);

s32 ov49_02258D70(s32, s32);

s32 ov49_02258E60(void *, s32);

s32 ov49_02258EAC(s32, s32, s32, s32);

s32 ov49_02258EEC(s32, s32, s32);

s32 ov49_02259130(void *, s32);

s32 ov49_02259154(void *, s32 *);

s32 ov49_02259160(void *, s32);

s32 ov49_0225916C(void *, s32);

s32 ov49_022591B4(void *, s32);

s32 ov49_02259FE8(void *);

s32 ov49_02259FF0(void *);

void *ov49_02259FF8(void *);

void *ov49_0225A008(void *);

s32 ov49_0225A010(void *);

s32 ov49_0225A034(void *, s32);

s32 ov49_0225A038(void *, s32);

s32 ov49_0225A040(void *);

s32 ov49_0225A044(void *);

s32 ov49_0225A08C(void *, s32);

s32 ov49_0225A09C(void *, s32);

s32 ov49_0225A0AC(void *);

s32 ov49_0225A0BC(void *);

s32 ov49_0225A0CC(void *);

s32 ov49_0225A0EC(void *);

s32 ov49_0225A30C(void *, s32, s32);

s32 ov49_0225A510(void *);

s32 ov49_0225A53C(void *, s32);

s32 ov49_0225CC44(void *);

void *ov49_0225EF40(void *, s32);

s32 ov49_0225EF68(void *);

void *ov49_0225EF84(void *);

u32 ov49_0225EF88(void *);

s32 ov49_0225EF8C(void *, s32);

s32 ov49_0225EF98(s32, s32, s32 *, s32);

s32 ov49_022611D4(void *, void *);

s32 ov49_022611F4(void *, void *);

s32 ov49_02261234(void *, void *, void *);

s32 ov49_022613AC(void *, void *);

s32 ov49_02261434(void *, void *, void *);

s32 ov49_02261460(void *, void *, void *);

extern s32 ov49_02269B38;

s32 ov49_02260E2C(void *arg0, void *arg1, s32 arg2);

s32 ov49_02260E2C(void *arg0, void *arg1, s32 arg2) {
    s32 sp10[3];
    void *spC;
    s32 sp8;
    s32 sp4;
    s32 sp0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    void *temp_r7;
    s32 temp_r7_2;
    u32 temp_r0;
    void *temp_r4;
    void *temp_r4_2;

    sp0 = arg2;
    sp8 = ov49_02259FF0(arg1);
    temp_r7 = ov49_02259FF8(arg1);
    spC = ov49_0225A008(arg1);
    sp4 = ov49_02259FE8(arg1);
    temp_r4 = ov49_0225EF84(arg0);
    temp_r0 = ov49_0225EF88(arg0);
    switch (temp_r0) {
    case 0:
        temp_r4_2 = ov49_0225EF40(arg0, 0x18);
        *(s32 *)((u8 *)temp_r4_2 + 0xC) = ov49_02258D70(sp8, sp0);
        temp_r0_2 = ov49_0225A040(arg1);
        switch (temp_r0_2) {
        case 33:
            *(s16 *)((u8 *)temp_r4_2 + 6) = 2;
            break;
        case 34:
            *(s16 *)((u8 *)temp_r4_2 + 6) = 1;
            break;
        case 35:
            *(s16 *)((u8 *)temp_r4_2 + 6) = 0;
            break;
        default:
            GF_AssertFail();
            break;
        }
        ov49_0225EF8C(arg0, 1U);
        break;
    case 1:
        if (ov45_0222A374(sp4) == 0) {
            *(u16 *)((u8 *)temp_r4 + 4) = 8U;
            ov49_0225EF8C(arg0, 6U);
        } else if (ov45_0222A330(sp4) == 1) {
            *(u16 *)((u8 *)temp_r4 + 4) = 0xAU;
            ov49_0225EF8C(arg0, 6U);
        } else {
            temp_r0_3 = ov45_0222ADB8(sp4, sp0, *(u16 *)((u8 *)temp_r4 + 6));
            *(s32 *)temp_r4 = temp_r0_3;
            if (temp_r0_3 != -1) {
                ov49_0225EF8C(arg0, 2U);
                ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, 0x1F));
                ov49_0225A0BC(arg1);
            } else {
                *(u16 *)((u8 *)temp_r4 + 4) = 9U;
                ov49_0225EF8C(arg0, 6U);
            }
        }
        break;
    case 2:
        if ((2 & gSystem.newKeys) || (gSystem.newKeys & 0x80)) {
            ov45_0222ADC8(sp4, sp0);
            ov49_0225A0CC(arg1);
            *(u8 *)((u8 *)temp_r4 + 0xA) = 6U;
            *(u16 *)((u8 *)temp_r4 + 4) = 0xFU;
            ov49_0225EF8C(arg0, 0xCU);
        } else {
            if (ov49_022611F4(temp_r4, temp_r7) != 1) {
                break;
            }
            ov49_0225A0CC(arg1);
            ov45_0222AE44(sp4);
            *(s16 *)((u8 *)temp_r4 + 8) = 0;
            ov45_0222A5E8(ov49_02259FE8(arg1), 0xA);
            ov49_0225A044(arg1);
            ov49_0225CC44(spC);
            ov49_02259130(*(void **)((u8 *)temp_r4 + 0xC), 0);
            ov49_0225916C(*(void **)((u8 *)temp_r4 + 0xC), 1);
            PlaySE(0x5C1U);
            ov49_0225A510(arg1);
            *(u8 *)((u8 *)temp_r4 + 0xA) = 3U;
            ov49_0225EF8C(arg0, 0xCU);
        }
        break;
    case 3:
        if (ov49_02261234(temp_r4, temp_r7, spC) == 1) {
            ov49_0225916C(*(void **)((u8 *)temp_r4 + 0xC), 0);
            ov49_02259160(*(void **)((u8 *)temp_r4 + 0xC), 2);
            ov49_022591B4(*(void **)((u8 *)temp_r4 + 0xC), 8);
            ov49_0225EF8C(arg0, 4U);
            *(s16 *)((u8 *)temp_r4 + 8) = 0;
        }
        break;
    case 4:
        ov49_02261434(temp_r4, temp_r7, arg1);
        ov49_022611D4(temp_r4, spC);
        if (ov49_022613AC(temp_r4, temp_r7) == 1) {
            ov49_0225EF8C(arg0, 5U);
            ov49_0225A53C(arg1, 0);
        }
        break;
    case 5:
        ov49_02261434(temp_r4, temp_r7, arg1);
        ov49_022611D4(temp_r4, spC);
        ov49_02259154(*(void **)((u8 *)temp_r4 + 0xC), sp10);
        ov49_02261460(temp_r4, temp_r7, arg1);
        if (sp10[0] < 0x60000) {
            *(u8 *)((u8 *)temp_r4 + 0xB) = 0U;
            ov49_0225EF8C(arg0, 0xDU);
        }
        break;
    case 6:
        ov49_02258EAC(sp8, *(s32 *)((u8 *)temp_r4 + 0xC), 2, 1);
        ov49_0225EF8C(arg0, 7U);
        break;
    case 7:
        if (ov49_02258E60(*(void **)((u8 *)temp_r4 + 0xC), 5) != 0) {
            break;
        }
        ov49_0225EF8C(arg0, *(u16 *)((u8 *)temp_r4 + 4));
        break;
    case 8:
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 0x1E));
        *(u16 *)((u8 *)temp_r4 + 4) = 0xFU;
        ov49_0225EF8C(arg0, 0xBU);
        break;
    case 9:
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 0x20));
        *(u16 *)((u8 *)temp_r4 + 4) = 0xFU;
        ov49_0225EF8C(arg0, 0xBU);
        break;
    case 10:
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 0x21));
        *(u16 *)((u8 *)temp_r4 + 4) = 0xFU;
        ov49_0225EF8C(arg0, 0xBU);
        break;
    case 11:
        if (ov49_0225A0AC(arg1) == 1) {
            ov49_0225EF8C(arg0, *(u16 *)((u8 *)temp_r4 + 4));
        }
        break;
    case 12:
        ov49_0225A0EC(arg1);
        ov49_0225EF8C(arg0, (u16) * (u8 *)((u8 *)temp_r4 + 0xA));
        break;
    case 13:
        ov49_02261434(temp_r4, temp_r7, arg1);
        ov49_02261460(temp_r4, temp_r7, arg1);
        *(u8 *)((u8 *)temp_r4 + 0xB) = (u8)(*(u8 *)((u8 *)temp_r4 + 0xB) + 1);
        if ((u32) * (u8 *)((u8 *)temp_r4 + 0xB) > 0x78U) {
            ov49_0225A034(arg1, 1);
            ov49_0225A038(arg1, 0);
            ov49_0225A0EC(arg1);
            ov45_0222A5E8(ov49_02259FE8(arg1), 0xB);
            ov49_0225EF8C(arg0, 0xEU);
        }
        break;
    case 14:
        ov49_02261434(temp_r4, temp_r7, arg1);
        ov49_02261460(temp_r4, temp_r7, arg1);
        break;
    case 15:
        temp_r7_2 = ov49_0225A010(arg1);
        ov49_0225A0EC(arg1);
        ov49_02258EEC(sp8, *(s32 *)((u8 *)temp_r4 + 0xC), 1);
        ov49_0225EF68(arg0);
        ov49_0225EF98(temp_r7_2, sp0, &ov49_02269B38, 0);
        break;
    }
    return 0;
}
