#include "global.h"

#include "overlay_96_ov96_02203468work.h"

void *ov96_021EB594(void *);

s32 ov96_021EB630(void *, s32);

s32 ov96_02203924(void *elem1, void *elem2);

void ov96_02203970(Ov96_02203468Work *arg0, s16 arg1);

void ov96_02203970(Ov96_02203468Work *arg0, s16 arg1) {
    u8 sp4[93];
    s32 temp_r1;
    s32 temp_r6_2;
    u8 var_r4;
    u8 var_r4_2;
    void *temp_r0;
    void *temp_r2;

    var_r4 = 0;
    do {
        temp_r0 = ov96_021EB594(arg0->balls[var_r4].unk_4);
        temp_r1 = var_r4 * 8;
        temp_r2 = sp4 + temp_r1;
        *(s16 *)((u8 *)temp_r2 + 2) = (s16)arg0->balls[var_r4].index;
        *(s32 *)((u8 *)temp_r2 + 4) = (s32) * (s32 *)((u8 *)temp_r0 + 4);
        *(s16 *)(sp4 + temp_r1) = arg1;
        var_r4 += 1;
    } while ((u32)var_r4 < 0xCU);
    MATH_QSort(sp4, 0xCU, 8U, ov96_02203924, arg0->unk_598);
    var_r4_2 = 0;
    do {
        temp_r6_2 = (u8) * (u16 *)((u8 *)(sp4 + (var_r4_2 * 8)) + 2);
        ov96_021EB630(arg0->balls[temp_r6_2].unk_0, var_r4_2 + 0x14);
        ov96_021EB630(arg0->balls[temp_r6_2].unk_8, var_r4_2 + 5);
        var_r4_2 += 1;
    } while ((u32)var_r4_2 < 0xCU);
}
