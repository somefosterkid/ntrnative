#include "global.h"

#include "unk_020957B0.h"

void sub_02095CB8(CursorNav *nav, u32 areaId, const CursorNavEvent *event) {
    sub_02095C90(nav, areaId);
    nav->areas[nav->area].enter(nav, nav->positions + nav->area, nav->areas[nav->area].info, event);
}
