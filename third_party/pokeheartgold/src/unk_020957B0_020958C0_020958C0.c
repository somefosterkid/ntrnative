#include "global.h"

#include "unk_020957B0.h"

void sub_020958C0(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy) {
    CursorNavEvent event;

    event.kind = sub_02095BF0(nav, pos, info, dx, dy);
    event.value = pos->x;
    switch (event.kind) {
    case 3:
        event.value = 0;
        sub_02095CB8(nav, 3, &event);
        return;

    case 1:
        if (*nav->screenMode == 0x12) {
            event.value = 0;
            sub_02095CB8(nav, 6, &event);
            return;
        }
        event.value = 4;
        sub_02095CB8(nav, 2, &event);
        break;
    }
}
