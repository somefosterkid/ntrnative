s32 ov96_021EB0A4(s32, s32, s32, s32 *, s32 *);
s32 ov96_021EB588(s32, s32 *);
s32 ov96_021FEBF0(s32, void *, s32 *, s32, s32, s32, s32, s32);
s32 ov96_021FED3C(void *, s32 *, s32, s32, s32, s32, s32);
s32 ov96_021FEE60(void *, s32, s32, s32, s32, s32);

void ov96_021FEECC(s32 arg0, void *arg1, void *arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    
    
    s32 sp20[3];
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 temp_r2;
    s32 temp_r3;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    u8 temp_r0;
    u8 temp_r0_2;

    sp10 = arg0;
    sp14 = 0;
    temp_r0 = ((s32) *(u8 *)((u8 *)arg3 + 0x1D) >> (arg4 * 2)) & 3;
    if (*(u8 *)((u8 *)arg1 + 0x9F) != temp_r0) {
        *(u8 *)((u8 *)arg1 + 0x9F) = temp_r0;
        *(s8 *)((u8 *)arg1 + 0xA0) = (s8) sp14;
        sp14 = 1;
    }
    temp_r2 = *(s32 *)arg2;
    temp_r3 = *(s32 *)((u8 *)arg2 + 4);
    ov96_021EB0A4(*(s32 *)(arg1 + (arg5 * 4)), (temp_r2 / 0x1000), (temp_r3 / 0x1000), &sp1C, &sp18);
    sp20[0] = sp1C << 0xC;
    sp20[1] = sp18 << 0xC;
    sp20[2] = 0;
    temp_r0_2 = *(u8 *)((u8 *)arg1 + 0x9F);
    switch (temp_r0_2) {
    case 1:
        if ((arg7 != 0) && (arg9 != 0)) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        ov96_021FEBF0(sp10, arg1, sp20, arg4, arg5, arg6, var_r1, arg8);
        return;
    case 2:
        if ((arg7 != 0) && (arg9 != 0)) {
            var_r1_2 = 1;
        } else {
            var_r1_2 = 0;
        }
        ov96_021FED3C(arg1, sp20, arg4, arg5, arg6, var_r1_2 | arg8, arg8);
        return;
    case 0:
        if (sp14 != 0) {
            if ((arg7 != 0) && (arg9 != 0)) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            ov96_021FEE60(arg1, arg4, arg5, arg6, var_r0 | arg8, arg8);
        }
        ov96_021EB588(*(s32 *)((u8 *)arg1 + 0x70), sp20);
        return;
    }
}
