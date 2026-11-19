#include "global.h"

s32 ov96_021FF0BC(void *, s32, s32 *, s32 *, void *, s32 *);

s32 ov96_021FF1E0(s32, s32, s32 *);

s32 ov96_021FF2A0(s32 *, void *, s32, s32 *, s32 *);

s32 ov96_021FF574(s32 *, s32 *);

u16 ov96_021FF5A8(void *arg0, s32 arg1, s32 arg2, s32 *arg3, void *arg4, void *arg5);

u16 ov96_021FF5A8(void *arg0, s32 arg1, s32 arg2, s32 *arg3, void *arg4, void *arg5) {
    s32 sp164[30];
    s32 spEC[30];
    s32 sp88[25];
    s32 sp24[25];
    s32 sp18[3];
    u8 sp14[4];
    u8 sp10;
    u8 spC;
    s32 sp8;
    s32 *var_r4_2;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5_2;

    sp8 = arg1;
    ov96_021FF0BC(arg0, *arg3, sp164, spEC, &sp14[1], (s32 *)sp14);
    var_r4 = 0;
    sp10 = 0;
    if ((s32) * (u8 *)((s32 *)sp14) > 0) {
        s32 *var_r5;
        var_r5 = spEC;
loop_2:
        ov96_021FF1E0(arg2, *var_r5, sp88);
        if (ov96_021FF574(arg3, sp88) != 0) {
            sp10 = 1;
        } else {
            var_r4 += 1;
            var_r5 += 1;
            if (var_r4 < (s32) * (u8 *)((s32 *)sp14)) {
                goto loop_2;
            }
        }
    }
    spC = 0;
    var_r5_2 = 0;
    if ((s32) * (u8 *)((u8 *)((s32 *)sp14) + 1) > 0) {
        var_r4_2 = sp164;
        do {
            ov96_021FF1E0(arg2, *var_r4_2, sp24);
            temp_r0 = ov96_021FF2A0(arg3, arg4, arg2, sp24, sp18);
            if (temp_r0 != 0) {
                if (temp_r0 >= 0xA) {
                    *(s32 *)arg5 = sp18[0];
                    *(s32 *)((u8 *)arg5 + 4) = sp18[1];
                    *(s32 *)((u8 *)arg5 + 8) = 0;
                } else if (temp_r0 != sp8) {
                    *(s32 *)arg5 = sp18[0];
                    *(s32 *)((u8 *)arg5 + 4) = sp18[1];
                    *(s32 *)((u8 *)arg5 + 8) = 0;
                }
                spC = temp_r0;
                break;
            }
            var_r5_2 += 1;
            var_r4_2 += 1;
        } while (var_r5_2 < (s32) * (u8 *)((u8 *)((s32 *)sp14) + 1));
    }
    return (u16)((sp10 << 8) + spC);
}
