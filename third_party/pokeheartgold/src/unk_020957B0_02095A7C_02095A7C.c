#include "global.h"

#include "unk_020957B0.h"

void sub_02095A7C(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy) {
    CursorNavEvent event;

    event.kind = sub_02095BF0(nav, pos, info, dx, dy);
    event.value = pos->x;
    switch (event.kind) {
    case 0:
    case 1:
    case 2:
    default:
        break;
    case 3:
        if (*nav->screenMode == 0x12) {
            sub_02095CB8(nav, 6, &event);
            return;
        }
        sub_02095CB8(nav, 2, &event);
        return;
    case 4:
        if (*nav->screenMode == 0x12) {
            sub_02095CB8(nav, 6, &event);
            return;
        }
        sub_02095CB8(nav, 2, &event);
        return;
    }
}
