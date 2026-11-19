s8 ov96_02213EC4(void *);

void ov96_02211DE4(void *arg0, void *arg1) {
    s32 sp14;
    s32 var_r7;
    void *sp10;
    s32 spC;
    void *sp0;
    s32 sp8;
    u8 hA;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r3;
    s32 var_r4;
    s32 var_r6;
    s32 var_r6_2;
    s32 sp4;
    s8 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r2_3;
    void *temp_r0;
    void *temp_r2_2;
    void *var_r4_2;

    sp0 = arg0;
    spC = 0;
    sp8 = 0;
    sp4 = 0;
    var_r4 = 0;
    var_r6 = 0;
    sp10 = arg0;
    sp10 = arg0 + 0x5C;
    do {
        temp_r0 = sp10 + ((var_r4 / 3) * 0x174) + (0x7C * (var_r4 % 3));
        temp_r2 = *(s32 *)((u8 *)temp_r0 + 0x34) / 0x1000;
        temp_r1 = (u8)*(s32 *)((u8 *)temp_r0 + 0x78);
        temp_r3 = *(s32 *)((u8 *)temp_r0 + 0x30);
        hA = *(u16 *)((u8 *)temp_r0 + 0x5C) - 1;
        *(u8 *)(arg1 + var_r4) = (temp_r3 / 0x1000);
        *(s8 *)((u8 *)((arg1 + var_r4)) + 0xC) = temp_r2;
        spC += hA << var_r6;
        var_r4 += 1;
        sp8 += temp_r1 << var_r6;
        var_r6 += 2;
    } while (var_r4 < 0xC);
    var_r6_2 = 0;
    var_r4_2 = sp0 + 0x62C;
    var_r7 = 0;
    do {
        sp14 = *(s32 *)((u8 *)var_r4_2 + 8) / 0x1000;
        temp_r0_3 = ov96_02213EC4(var_r4_2);
        temp_r2_2 = arg1 + var_r6_2;
        var_r6_2 += 1;
        *(s8 *)((u8 *)temp_r2_2 + 0x18) = sp14;
        *(s8 *)((u8 *)temp_r2_2 + 0x1A) = temp_r0_3;
        temp_r0_4 = *(u8 *)((u8 *)var_r4_2 + 0x39);
        var_r4_2 += 0x4C;
        temp_r1_2 = temp_r0_4 << var_r7;
        var_r7 += 2;
        sp4 += temp_r1_2;
    } while (var_r6_2 < 2);
    temp_r2_3 = *(u8 *)((u8 *)sp0 + 0x73C);
    *(s32 *)((u8 *)arg1 + 0x1C) = (s32) (spC + (sp4 << 0x18) + (temp_r2_3 << 0x1C));
    *(u8 *)((u8 *)sp0 + 0x73C) = (((s32) (temp_r2_3 + 1) % 4));
    if (*(u8 *)((u8 *)sp0 + 0x664) == 2) {
        *(s32 *)((u8 *)arg1 + 0x1C) += 0x40000000;
    }
    if (*(u8 *)((u8 *)sp0 + 0x6B0) != 0) {
        *(s32 *)((u8 *)arg1 + 0x1C) = (s32) (*(s32 *)((u8 *)arg1 + 0x1C) - 0x80000000);
    }
    *(s32 *)((u8 *)arg1 + 0x20) = 0;
    *(s32 *)((u8 *)arg1 + 0x20) += sp8;
    *(s32 *)((u8 *)arg1 + 0x20) = (s32) (*(s32 *)((u8 *)arg1 + 0x20) + (*(u8 *)((u8 *)sp0 + 0x73E) << 0x18) + (*(s32 *)((u8 *)((sp0 + (temp_r2_3 * 4))) + 0x6F4) << 0x19));
}
