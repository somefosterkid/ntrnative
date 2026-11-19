#include "global.h"

#include "overlay_96_arena_cpu.h"

void ov96_021F3AD8(Ov96ArenaCpu *cpu, Ov96ArenaCpuItem *items, s32 cell, s32 *px, s32 *py) {
    s32 i;

    i = 0;
    do {
        if (cell == cpu->itemCells[i]) {
            *px = items[i].x;
            *py = items[i].y;
            return;
        }
        i++;
    } while (i < 4);
}
