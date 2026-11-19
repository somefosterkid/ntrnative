#include "global.h"

#include "overlay_96_arena_cpu.h"

void ov96_021F3888(Ov96ArenaCpu *cpu, u8 n, Ov96ArenaCpuItem *items, Ov96ArenaCpuMon *mon, Ov96ArenaCpuCell **cells) {
    Ov96ArenaCpuCell *cell;
    u32 i;
    s32 score;
    BOOL onItem;

    i = 0;
    while (i < n) {
        cell = cells[i];
        if (cell->valid == FALSE) {
            cell->score = -100;
        } else {
            score = ov96_021F3930(cpu, items, cell->cell, mon, cell->px, cell->py, &onItem);
            cells[i]->score = score;
            if (onItem != FALSE) {
                cell = cells[i];
                ov96_021F3AD8(cpu, items, cell->cell & 0xff, &cell->px, &cell->py);
            }
        }
        i = (i + 1) & 0xff;
    }
}
