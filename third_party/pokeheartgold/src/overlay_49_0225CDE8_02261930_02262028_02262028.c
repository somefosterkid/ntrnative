#include "global.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

#include "overlay_45_thumb.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

SaveData *ov45_0222A2C8();

s32 ov45_0222A394(s32);

s32 ov45_0222A404(s32);

s32 ov45_0222A414(s32);

s32 ov45_0222A43C(s32);

s32 ov45_0222A450(s32, s32, s32);

s32 ov45_0222A480(s32, s32);

s32 ov45_0222A498(s32, s32 *);

s32 ov45_0222A548(s32);

s32 ov45_0222A5E8(s32, int);

PlayerProfile *ov45_0222AB1C(s32);

s32 ov45_0222AC14(s32, u32, u32, s32, s32, s32, s32, s32);

s32 ov45_0222EC68(s32);

s32 ov45_0222F1BC(void);

s32 ov45_0222F218(void);

s32 ov45_0222F294(s32);

s32 ov45_0222F2D4(s32);

u32 ov45_0222F314(s32);

u16 ov45_0222F3E8(s32);

s32 ov45_0222F430(void);

s32 ov49_02259FE8(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A08C(UnkStruct_ov70_0225DEE8 *, s32);

s32 ov49_0225A09C(UnkStruct_ov70_0225DEE8 *, s32);

s32 ov49_0225A0AC(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A0BC(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A0CC(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A0EC(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A174(UnkStruct_ov70_0225DEE8 *, void *, s32, s32);

u32 ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *, void *, void *);

s32 ov49_0225A294(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A2C4(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A2F8(UnkStruct_ov70_0225DEE8 *);

s32 ov49_0225A30C(UnkStruct_ov70_0225DEE8 *, s32, s32);

s32 ov49_0225A37C(UnkStruct_ov70_0225DEE8 *, int, s32);

void *ov49_0225EF3C();

s32 ov49_0225EF40(s32, s32);

s32 ov49_0225EF68(s32);

void *ov49_0225EF84(s32);

u32 ov49_0225EF88(s32);

s32 ov49_0225EF8C(s32, s32);

s32 ov49_0225EF90(s32);











s32 ov49_02262028(s32 arg0, UnkStruct_ov70_0225DEE8 *arg1, s32 arg2);

s32 ov49_02262028(s32 arg0, UnkStruct_ov70_0225DEE8 *arg1, s32 arg2) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    u32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 temp_r0_2;
    s32 temp_r0_6;
    s32 temp_r0_9;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r2;
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;
    s32 var_r7_2;
    s8 temp_r0_11;
    u32 *temp_r0_8;
    u32 temp_r0;
    u32 temp_r0_10;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r7;
    u32 temp_r7_2;
    UnkStruct_ov70_02266AF0 *temp_r4;
    void *temp_r6;

    sp10 = arg0;
    sp14 = arg2;
    temp_r6 = ov49_0225EF3C();
    temp_r4 = ov49_0225EF84(arg0);
    sp30 = ov49_02259FE8(arg1);
    temp_r0 = ov49_0225EF88(arg0);
    switch (temp_r0) {
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 26:
    case 27:
        if ((u32) temp_r4->unk_08 > ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1))) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        if ((s32) temp_r4->unk_08 > sub_02037454()) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        if (sub_0203988C() == 0) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        /* fall through */
    case 8:
        if ((ov45_0222F464() == 1) && (ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1)) <= 1U)) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        temp_r0_2 = temp_r4->unk_10 - 1;
        temp_r4->unk_10 = temp_r0_2;
        if (temp_r0_2 < 0) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        /* fall through */
    case 7:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        temp_r0_3 = (u32)sub_020390C4();
        switch (temp_r0_3) {
        case 2:
        case 3:
        case 4:
        case 5:
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        break;
    }
    temp_r0_4 = ov49_0225EF88(sp10);
    switch (temp_r0_4) {
    case 0:
        ov49_0225EF40(sp10, 0x44);
        ov49_0225EF90(sp10);
    default:
        break;
    case 1:
        if (ov45_0222A414(sp30) != 0) {
            *(s8 *)temp_r6 = 7;
            ov49_0225EF8C(sp10, 0x1CU);
        } else if (ov45_0222A394(sp30) != 0) {
            *(s8 *)temp_r6 = 2;
            ov49_0225EF8C(sp10, 0x1CU);
        } else {
            PlaySE(0x5DDU);
            ov49_0225EF90(sp10);
        }
        break;
    case 2:
        ov49_0225A37C(arg1, *(u8 *)((u8 *)temp_r6 + 2), 0);
        ov49_02262BF8(temp_r4, sp10, arg1, 0, 3);
        break;
    case 3:
        ov49_02262C38(temp_r4, arg1, 3, 1);
        ov49_0225A174(arg1, &temp_r4->unk_18, 0, 0);
        ov49_0225EF8C(sp10, 4U);
        break;
    case 4:
        var_r7 = 0;
        temp_r0_5 = ov49_0225A1D4(arg1);
        switch (temp_r0_5) {
        case -2:
        case 2:
            *(s8 *)temp_r6 = 5;
            ov49_0225EF8C(sp10, 0x1CU);
            var_r7 = 1;
            break;
        case 1:
            ov49_0225EF8C(sp10, 5U);
            var_r7 = 1;
            break;
        case 0:
            ov49_0225EF8C(sp10, 6U);
            var_r7 = 1;
            break;
        }
        if (var_r7 == 1) {
            ov49_0225A1E4(arg1, 0, 0);
            ov49_02262CA8(temp_r4, arg1);
        }
        break;
    case 5:
        ov49_0225A37C(arg1, *(u8 *)((u8 *)temp_r6 + 2), 0);
        ov49_02262BF8(temp_r4, sp10, arg1, *(u8 *)((u8 *)temp_r6 + 2) + 0x22, 2);
        break;
    case 6:
        if (ov45_0222A394(sp30) != 0) {
            *(s8 *)temp_r6 = 2;
            ov49_0225EF8C(sp10, 0x1CU);
            break;
        }
        if (ov45_0222F274((s32) * (u8 *)((u8 *)temp_r6 + 1)) == 1) {
            if ((ov45_0222F2D4(*(u8 *)((u8 *)temp_r6 + 1)) == 0) || (ov45_0222F294(*(u8 *)((u8 *)temp_r6 + 1)) == 0)) {
                *(s8 *)temp_r6 = 1;
                ov49_0225EF8C(sp10, 0x1CU);
                break;
            }
            if (ov45_0222F3E8(*(u8 *)((u8 *)temp_r6 + 1)) == 0) {
                *(s8 *)temp_r6 = 7;
                ov49_0225EF8C(sp10, 0x1CU);
                break;
            }
        }
        temp_r4->unk_00 = ov45_0222F3E8(*(u8 *)((u8 *)temp_r6 + 1));
        temp_r4->unk_10 = 0x384;
        if ((s32) temp_r4->unk_00 > 0x14A) {
            var_r0 = ov49_0225A30C(arg1, 0, 0xA);
        } else {
            temp_r4->unk_14 = 1;
            var_r0 = ov49_0225A30C(arg1, 0, 0xB);
        }
        ov49_0225A09C(arg1, var_r0);
        ov49_0225A0BC(arg1);
        sub_0203981C((s32) * (u8 *)((u8 *)temp_r6 + 1));
        if (ov45_0222F464() == 1) {
            ov45_0222AC14(sp30, *(u8 *)((u8 *)temp_r6 + 2), 1U, sp14, 0, 0, 0, 0);
            temp_r4->unk_07 = 1U;
        }
        ov49_02262D70(&temp_r4->unk_3C, arg1, *(u8 *)((u8 *)temp_r6 + 2), 1, (s32) temp_r4->unk_00);
        ov49_02262E04(&temp_r4->unk_3C, arg1, 1);
        ov49_0225EF8C(sp10, 7U);
        break;
    case 7:
        sp2C = sub_0203988C();
        temp_r7 = ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1));
        temp_r4->unk_00 = ov45_0222F3E8(*(u8 *)((u8 *)temp_r6 + 1));
        ov49_02262DD4(&temp_r4->unk_3C, temp_r4->unk_00);
        if (((s32) temp_r4->unk_00 <= 0x14A) && (temp_r4->unk_14 == 0)) {
            ov49_0225A0CC(arg1);
            temp_r4->unk_14 = 1;
            ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, 0xB));
            ov49_0225A0BC(arg1);
        }
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        if (sp2C == 0) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
        } else if (sp2C == 2) {
            *(s8 *)temp_r6 = 8;
            if (temp_r7 == 4) {
                var_r6 = 0x10;
            } else {
                var_r6 = 0x12;
            }
            ov49_0225A0CC(arg1);
            ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, var_r6));
            ov49_0225A0BC(arg1);
            ov49_0225EF8C(sp10, 9U);
            temp_r4->unk_08 = (u16)temp_r7;
            ov49_02262DD4(&temp_r4->unk_3C, 0);
            ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        } else if ((temp_r4->unk_14 == 0) && (2 & gSystem.newKeys)) {
            PlaySE(0x5DCU);
            if (ov45_0222F464() == 0) {
                *(s8 *)temp_r6 = 6;
                ov49_0225A0CC(arg1);
                ov49_0225EF8C(sp10, 0x17U);
            } else {
                *(s8 *)temp_r6 = 6;
                ov49_0225A0CC(arg1);
                ov49_0225EF8C(sp10, 0x10U);
            }
        } else {
            if (ov45_0222F464() == 1) {
                if (temp_r4->unk_07 != temp_r7) {
                    temp_r4->unk_07 = (u8)temp_r7;
                    if (temp_r7 != 4) {
                        ov45_0222AC14(sp30, *(u8 *)((u8 *)temp_r6 + 2), temp_r7, sp14, 0, 0, 0, 0);
                    }
                }
                if ((temp_r4->unk_00 == 0) && (ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1)) <= 1U)) {
                    *(s8 *)temp_r6 = 3;
                    ov49_0225EF8C(sp10, 0x17U);
                    break;
                }
            }
            if ((ov45_0222F274((s32) * (u8 *)((u8 *)temp_r6 + 1)) == 1) && (ov45_0222F294(*(u8 *)((u8 *)temp_r6 + 1)) == 0)) {
                *(s8 *)temp_r6 = 0;
                ov49_0225EF8C(sp10, 0x17U);
            } else if (temp_r4->unk_00 == 0) {
                ov49_0225EF8C(sp10, 8U);
                ov49_0225A0CC(arg1);
                ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, 0x12));
                ov49_0225A0BC(arg1);
            }
        }
        break;
    case 8:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        temp_r5 = sub_0203988C();
        temp_r7_2 = ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1));
        temp_r0_6 = sub_020390C4();
        switch (temp_r0_6) {
        case 3:
        case 4:
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
            break;
        }
        if (temp_r5 == 0) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
        } else if (temp_r5 == 2) {
            *(s8 *)temp_r6 = 8;
            ov49_0225EF8C(sp10, 9U);
            temp_r4->unk_08 = (u16)temp_r7_2;
        } else if ((ov45_0222F274((s32) * (u8 *)((u8 *)temp_r6 + 1)) == 1) && (ov45_0222F294(*(u8 *)((u8 *)temp_r6 + 1)) == 0)) {
            *(s8 *)temp_r6 = 0;
            ov49_0225EF8C(sp10, 0x17U);
        }
        break;
    case 9:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        ov45_0222A5E8(ov49_02259FE8(arg1), *(u8 *)((u8 *)temp_r6 + 3));
        sub_02039B38();
        sub_02034354(ov45_0222A2C8(ov49_02259FE8(arg1)), 0);
        sub_02034B00(ov45_0222AB1C(sp30));
        sub_020378E4(0);
        ov49_02262C20(temp_r4, sp10, 0xA, 0x11);
        break;
    case 10:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        sub_02034434();
        sub_0203476C((s32)sub_0203769C());
        ov49_0225EF8C(sp10, 0xBU);
        break;
    case 11:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        var_r0_2 = sub_02034780();
        if (var_r0_2 != 0xFF) {
            do {
                sub_0203476C(var_r0_2);
                var_r0_2 = sub_02034780();
            } while (var_r0_2 != 0xFF);
        }
        temp_r5_2 = sub_020347A0();
        if ((u32)temp_r5_2 >= ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1))) {
            if ((ov45_0222F464() == 1) && (temp_r4->unk_02 == 0)) {
                ov45_0222F1BC();
                temp_r4->unk_02 = 1U;
            }
            if (ov45_0222F218() == 1) {
                ov49_0225EF8C(sp10, 0xCU);
            }
        } else if (ov45_0222F218() == 1) {
            *(s8 *)temp_r6 = 4;
            ov49_0225EF8C(sp10, 0x17U);
        }
        break;
    case 12:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        sub_02037BEC();
        ov45_0222A43C(sp30);
        ov49_02262C20(temp_r4, sp10, 0xD, 0xE);
        break;
    case 13:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        temp_r4->unk_0C = ov45_0222A548(sp30);
        if (sub_02037C0C((u32)sub_0203769C(), (s16 *)&temp_r4->unk_0C) == 1) {
            ov49_0225EF8C(sp10, 0xEU);
        }
        break;
    case 14:
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 0);
        sp18 = 0;
        sp28 = sub_020347A0();
        sp24 = (s32)sub_0203769C();
        var_r7_2 = 0;
        sp20 = 0;
        sp1C = ov45_0222F430();
        var_r4 = 0;
        for (var_r4 = 0; var_r4 < sp28; var_r4 += 1) {
            if (sp24 != var_r4) {
                temp_r0_8 = (u32 *)sub_02037C44(var_r4);
                if (temp_r0_8 != NULL) {
                    if (ov45_0222EC68(*temp_r0_8) == -1) {
                        sp18 = 1;
                        *(s8 *)temp_r6 = 4;
                        ov49_0225EF8C(sp10, 0x17U);
                        break;
                    }
                    ov45_0222A450(sp30, *temp_r0_8, var_r4);
                    if (*temp_r0_8 == sp1C) {
                        sp20 = 1;
                    }
                    var_r7_2 += 1;
                }
            } else {
                ov45_0222A480(sp30, var_r4);
                if (ov45_0222F464() == 1) {
                    sp20 = 1;
                }
                var_r7_2 += 1;
            }
        }
        if ((sp18 == 0) && (var_r7_2 == sp28)) {
            if (sp20 == 1) {
                ov49_0225EF8C(sp10, 0xFU);
            } else {
                *(s8 *)temp_r6 = 4;
                ov49_0225EF8C(sp10, 0x17U);
            }
        }
        break;
    case 15:
        sub_020398D4(0, 1);
        if (ov45_0222F464() == 1) {
            ov45_0222A498(sp30, &sp34);
            ov45_0222AC14(sp30, *(u8 *)((u8 *)temp_r6 + 2), ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1)), (s32) * (u8 *)(&sp34), (s32) * (u8 *)((u8 *)(&sp34) + 1), (s32) * (u8 *)((u8 *)(&sp34) + 2), (s32) * (u8 *)((u8 *)(&sp34) + 3), 1);
        }
        ov49_0225A0CC(arg1);
        ov49_02262C20(temp_r4, sp10, 0x1C, 0x12);
        break;
    case 16:
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 0x1A));
        ov49_0225EF8C(sp10, 0x11U);
        ov49_02262DB8(&temp_r4->unk_3C, arg1);
        break;
    case 17:
        if ((ov49_02262CB4(temp_r4, arg1, temp_r6, sp10, sp14) == 0) && (ov49_0225A0AC(arg1) == 1)) {
            ov49_0225EF8C(sp10, 0x12U);
        }
        break;
    case 18:
        ov49_0225A294(arg1);
        ov49_0225EF8C(sp10, 0x13U);
        break;
    case 19:
        temp_r0_9 = ov49_0225A2C4(arg1);
        switch (temp_r0_9) {
        case 0:
            ov49_0225A2F8(arg1);
            ov49_0225EF8C(sp10, 0x17U);
            ov45_0222A404(sp30);
            break;
        case 1:
            ov49_0225A2F8(arg1);
            ov49_0225EF8C(sp10, 0x14U);
            break;
        case 2:
            if (ov49_02262CB4(temp_r4, arg1, temp_r6, sp10, sp14) == 1) {
                ov49_0225A2F8(arg1);
            }
            break;
        }
        break;
    case 20:
        temp_r4->unk_00 = ov45_0222F3E8(*(u8 *)((u8 *)temp_r6 + 1));
        ov49_02262D70(&temp_r4->unk_3C, arg1, *(u8 *)((u8 *)temp_r6 + 2), 1, (s32) temp_r4->unk_00);
        ov49_02262E04(&temp_r4->unk_3C, arg1, 1);
        if ((s32) temp_r4->unk_00 > 0x14A) {
            var_r0_3 = ov49_0225A30C(arg1, 0, 0xA);
        } else {
            temp_r4->unk_14 = 1;
            var_r0_3 = ov49_0225A30C(arg1, 0, 0xB);
        }
        ov49_0225A09C(arg1, var_r0_3);
        ov49_0225A0BC(arg1);
        ov49_0225EF8C(sp10, 7U);
        break;
    case 21:
        temp_r0_10 = ov45_0222F314(*(u8 *)((u8 *)temp_r6 + 1));
        *(s8 *)temp_r6 = 8;
        if (temp_r0_10 == 4) {
            var_r2 = 0x10;
        } else {
            var_r2 = 0x12;
        }
        ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, var_r2));
        ov49_0225A0BC(arg1);
        ov49_0225EF8C(sp10, 9U);
        ov49_02262D70(&temp_r4->unk_3C, arg1, *(u8 *)((u8 *)temp_r6 + 2), 1, 0);
        ov49_02262E04(&temp_r4->unk_3C, arg1, 1);
        break;
    case 22:
        ov49_0225A09C(arg1, ov49_0225A30C(arg1, 0, 0x12));
        ov49_0225A0BC(arg1);
        ov49_02262D70(&temp_r4->unk_3C, arg1, *(u8 *)((u8 *)temp_r6 + 2), 1, 0);
        ov49_02262DD4(&temp_r4->unk_3C, 0);
        ov49_02262DF8(&temp_r4->unk_3C, arg1, 1);
        ov49_0225EF8C(sp10, 8U);
        break;
    case 23:
        sub_020343E4();
        sub_0203986C();
        ov49_0225EF8C(sp10, 0x18U);
        break;
    case 24:
        if (sub_0203988C() == 0) {
            ov49_0225EF8C(sp10, 0x1CU);
        }
        break;
    case 25:
        if (ov49_0225A0AC(arg1) == 1) {
            ov49_0225EF8C(sp10, temp_r4->unk_04);
        }
        break;
    case 26:
        temp_r0_11 = temp_r4->unk_05;
        if ((s32)temp_r0_11 > 0) {
            temp_r4->unk_05 = (s8)(temp_r0_11 - 1);
        }
        if (temp_r4->unk_05 == 0) {
            sub_02037AC0(temp_r4->unk_06);
            ov49_0225EF8C(sp10, 0x1BU);
        }
        break;
    case 27:
        temp_r4->unk_0A = (u16)(temp_r4->unk_0A + 1);
        if ((u32) temp_r4->unk_0A >= 0x96U) {
            sub_02037AC0(temp_r4->unk_06);
            temp_r4->unk_0A = 0U;
        }
        if (sub_02037B38(temp_r4->unk_06) != 0) {
            ov49_0225EF8C(sp10, temp_r4->unk_04);
        }
        break;
    case 28:
        ov49_0225A0EC(arg1);
        ov49_02262CA8(temp_r4, arg1);
        ov49_02262DB8(&temp_r4->unk_3C, arg1);
        ov49_0225A2F8(arg1);
        ov49_0225EF68(sp10);
        return 1;
    }
    return 0;
}
