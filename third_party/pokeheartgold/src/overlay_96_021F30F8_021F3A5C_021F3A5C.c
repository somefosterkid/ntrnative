#include "global.h"

#include "overlay_96_arena_cpu.h"

s32 ov96_021F3A5C(Ov96ArenaCpuItem *items, s32 x, s32 y) {
    VecFx32 pos;
    VecFx32 itemPos;
    s32 i;

    i = 0;
    pos.x = x;
    pos.y = y;
    pos.z = 0;
    do {
        if (items[i].radius <= 0) {
            break;
        }
        itemPos.x = items[i].x << 0xC;
        itemPos.y = items[i].y << 0xC;
        itemPos.z = 0;
        VEC_Subtract(&pos, &itemPos, &itemPos);
        if (VEC_Mag(&itemPos) < (items[i].radius << 0xC)) {
            return 1;
        }
        i++;
    } while (i < 4);
    return 0;
}

BOOL ov96_021F3AB0(Ov96ArenaCpu *cpu, s32 cell) {
    s32 temp_r4;
    s32 i;

    i = 0;
    do {
        temp_r4 = cpu->itemCells[i];
        if (temp_r4 == -1) {
            break;
        }
        if (cell == temp_r4) {
            return TRUE;
        }
        i++;
    } while (i < 4);
    return FALSE;
}
