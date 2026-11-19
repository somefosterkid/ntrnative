#include "global.h"

s32 ov48_02258F0C(s32 *);

u32 ov48_022592E0(s32 *, s32 *);

s32 ov48_022598CC(u8 *, s32 *);

s32 ov48_02259B3C(void *, s32 *, u32);

s32 ov48_02259B68(u8 *, u32);

u32 ov48_02259BBC(u8 *);

u32 ov48_022591D8(u8 *arg0);

u32 ov48_022591D8(u8 *arg0) {
    s32 pos1[3];
    s32 pos2[3];
    s32 sp18;
    s32 sp14;
    s32 sp10;
    u32 count;
    u32 sp8;
    s32 sp4;
    s32 sp0;
    s32 dist;
    s32 temp;
    s32 temp2;
    s32 var_r6;
    u32 i;
    u32 var_r7;

    count = ov48_02259BBC(arg0 + 0x224);
    ov48_022598CC(arg0 + 0x178, pos1);
    sp4 = (s16)(pos1[0] - 0x80);
    sp0 = (s16)(pos1[0] + 0x80);
    var_r6 = (s16)(pos1[1] - 0x80);
    sp18 = (s16)(pos1[1] + 0x80);
    dist = sp18 - var_r6;
    if (dist < 0) {
        dist = -dist;
    }
    if (dist > 0x100) {
        var_r6 = (s16)pos1[1];
        sp18 = var_r6;
        if (var_r6 > 0) {
            sp14 = var_r6 - 0x10000;
            sp10 = sp14;
        } else {
            sp14 = var_r6 + 0x10000;
            sp10 = sp14;
        }
        var_r6 -= 0x80;
        sp18 += 0x80;
        sp14 -= 0x80;
        sp10 += 0x80;
    } else {
        sp14 = var_r6;
        sp10 = sp18;
    }
    var_r7 = 0x100;
    sp8 = count;
    for (i = 0; i < count; i++) {
        ov48_02259B3C(arg0 + 0x224, pos2, i);
        temp = ov48_02259B68(arg0 + 0x224, i);
        if (pos2[0] > sp4 && pos2[0] < sp0 && ((pos2[1] > var_r6 && pos2[1] < sp18) || (pos2[1] > sp14 && pos2[1] < sp10)) && temp != 3) {
            ov48_02258F0C(pos1);
            ov48_02258F0C(pos2);
            temp2 = ov48_022592E0(pos1, pos2);
            if (temp2 < var_r7) {
                var_r7 = temp2;
                sp8 = i;
            }
        }
    }
    return sp8;
}
