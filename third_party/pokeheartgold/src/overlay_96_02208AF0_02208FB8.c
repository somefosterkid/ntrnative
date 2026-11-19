#include "global.h"

#include "math_util.h"
#include "overlay_96_hurdle_cpu.h"

Ov96HurdleCpuCell *ov96_02208FB8(Ov96HurdleCpu *cpu) {
    u8 count;
    u16 rnd;
    s32 i;
    u32 remainder;
    Ov96HurdleCpuCell *sorted[48];
    Ov96HurdleCpuCell *cells[48];

    i = 0;
    do {
        cells[i] = &cpu->cells[i];
        i++;
    } while (i < 48);
    count = ov96_02208FF0(cells, sorted);
    rnd = LCRandom();
    remainder = (s32)rnd % (s32)count;
    return sorted[remainder & 0xff];
}
