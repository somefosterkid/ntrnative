#include "global.h"

#include "unk_020957B0.h"

void sub_02095C90(CursorNav *nav, u32 areaId) {
    s32 i;
    const CursorNavArea *area;

    i = 0;
    if ((s32)nav->areaCount > 0) {
        area = nav->areas;
        do {
            if (areaId == area->info->id) {
                nav->area = (s8)i;
                return;
            }
            i = i + 1;
            area = area + 1;
        } while (i < (s32)nav->areaCount);
    }
    return;
}
