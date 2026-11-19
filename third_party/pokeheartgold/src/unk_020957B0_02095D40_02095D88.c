#include "global.h"

#include "unk_020957B0.h"

s32 sub_02095D88(CursorNav *nav, s32 dx, s8 dy) {
    short sp4;
    const CursorNavArea *area;
    u8 temp_r7;
    CursorNavPos *pos;
    temp_r7 = nav->area;
    pos = nav->positions + temp_r7;
    area = nav->areas + temp_r7;
    sp4 = (s32)pos->index;
    area->move(nav, pos, area->info, dx, dy);
    if (temp_r7 != nav->area) {
        return 1;
    }
    if (sp4 != pos->index) {
        return 1;
    }
    return 0;
}
