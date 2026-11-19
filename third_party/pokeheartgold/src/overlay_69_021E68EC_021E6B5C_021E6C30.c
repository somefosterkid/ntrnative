#include "global.h"

#include "geonet_globe.h"

s32 ov69_021E75A0(s32 *);

u32 ov69_021E75F8(s32 *, s32 *);

BOOL ov69_021E6C30(GeonetGlobe *globe, u32 *index);

BOOL ov69_021E6C30(GeonetGlobe *globe, u32 *index) {
    s32 sp24[4];
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    u32 sp4;
    u32 *sp0;
    s16 temp_r0;
    s16 temp_r0_4;
    s16 temp_r1_2;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r3;
    s32 var_r0;
    u32 temp_r0_5;
    u32 var_r4;
    u32 var_r7;
    sp20 = 0;
    sp0 = index;
    temp_r1 = globe->rotationAngle.x;
    sp1C = (s32)((s16)(temp_r1 - 0x80));
    sp18 = (s32)((s16)(temp_r1 + 0x80));
    temp_r3 = globe->rotationAngle.y;
    sp14 = (s16)(temp_r3 - 0x80);
    sp10 = (s32)((s16)(temp_r3 + 0x80));
    var_r7 = 0x100;
    sp4 = globe->markers.count;
    sp24[2] = temp_r1;
    sp24[3] = globe->rotationAngle.y;
    ov69_021E75A0(&sp24[2]);
    var_r0 = sp10 - sp14;
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    if (var_r0 > 0x100) {
        temp_r0 = (s16)(globe->rotationAngle.y);
        spC = (s32)temp_r0;
        sp8 = (s32)temp_r0;
        if (((s32)temp_r0) > 0) {
            temp_r0_2 = temp_r0 - 0x10000;
            sp14 = temp_r0_2;
            sp10 = temp_r0_2;
        } else {
            temp_r0_3 = temp_r0 + 0x10000;
            sp14 = temp_r0_3;
            sp10 = temp_r0_3;
        }
        spC -= 0x80;
        sp8 += 0x80;
        sp14 -= 0x80;
        sp10 += 0x80;
    } else {
        spC = sp14;
        sp8 = sp10;
    }
    var_r4 = 0;
    if (var_r4 < globe->markers.count) {
        do {
            temp_r0_4 = globe->markers.markers[var_r4].x;
            if ((((((s32)temp_r0_4) > sp1C) && (((s32)temp_r0_4) < sp18)) && (((temp_r1_2 = globe->markers.markers[var_r4].y, ((s32)temp_r1_2) > sp14) && (((s32)temp_r1_2) < sp10)) || ((((s32)temp_r1_2) > spC) && (((s32)temp_r1_2) < sp8)))) && (globe->markers.markers[var_r4].recency != 0)) {
                sp24[0] = (s32)temp_r0_4;
                sp24[1] = (s32)(globe->markers.markers[var_r4].y);
                ov69_021E75A0(&sp24[0]);
                temp_r0_5 = ov69_021E75F8(&sp24[2], &sp24[0]);
                if (temp_r0_5 < var_r7) {
                    var_r7 = temp_r0_5;
                    sp4 = var_r4;
                }
            }
            var_r4 += 1;
        } while (var_r4 < globe->markers.count);
    }
    *sp0 = sp4;
    if (sp4 != globe->markers.count) {
        sp20 = 1;
    }
    return sp20;
}
