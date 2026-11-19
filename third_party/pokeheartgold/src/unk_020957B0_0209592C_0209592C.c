#include "global.h"

#include "unk_020957B0.h"

void sub_0209592C(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy) {
    CursorNavEvent event;
    s32 r;

    r = sub_02095BF0(nav, pos, info, dx, dy);
    event.kind = r;
    if (pos->y == 0) {
        event.value = 0;
    } else if (pos->y == 1) {
        event.value = 2;
    } else {
        event.value = 3;
    }
    switch (r) {
    case 1:
        if (*nav->screenMode == 0x12) {
            event.value = 0;
            sub_02095CB8(nav, 6, &event);
            return;
        }
        sub_02095CB8(nav, 2, &event);
        return;
    case 2:
        if (*nav->screenMode == 0x12) {
            event.value = 0;
            sub_02095CB8(nav, 6, &event);
            return;
        }
        sub_02095CB8(nav, 2, &event);
        return;
    case 4:
        if (nav->mode != 0) {
            sub_02095CB8(nav, 1, &event);
        }
        return;
    }
}
