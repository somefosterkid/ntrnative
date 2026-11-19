#include "global.h"

#include "math_util.h"
#include "overlay_96_arena_cpu.h"

void ov96_021F380C(u8 n, Ov96ArenaCpuCell **cells) {
    u32 sp0;
    u16 temp_r0;
    u16 temp_r0_2;
    u8 i;
    Ov96ArenaCpuCell *temp_r0_3;
    Ov96ArenaCpuCell *temp_r6;
    Ov96ArenaCpuCell *temp_r6_2;

    sp0 = n;
    i = 0;
    while (i < sp0) {
        temp_r6 = cells[i];
        if (temp_r6->valid) {
            temp_r0 = LCRandom();
            temp_r6->px = (temp_r6->x << 5) + 0x10 + (temp_r0 % 32);
            temp_r6_2 = cells[i];
            temp_r0_2 = LCRandom();
            temp_r6_2->py = (temp_r6_2->y << 5) + 0x10 + (temp_r0_2 % 32);
            temp_r0_3 = cells[i];
            temp_r0_3->cell = temp_r0_3->x + (7 * temp_r0_3->y);
        } else {
            temp_r6->px = -1;
            cells[i]->py = -1;
            cells[i]->cell = -1;
        }
        i++;
    }
}
