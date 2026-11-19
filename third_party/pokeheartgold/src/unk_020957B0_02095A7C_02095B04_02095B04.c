#include "global.h"

#include "unk_020957B0.h"

void sub_02095B04(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy) {
    CursorNavEvent event;

    event.kind = sub_02095BF0(nav, pos, info, dx, dy);
    event.value = pos->x;
    switch (event.kind) {
    case 1:
        event.value = pos->y;
        sub_02095CB8(nav, 3, &event);
        return;
    case 2:
        event.value = pos->y;
        sub_02095CB8(nav, 3, &event);
        return;
    case 3:
        event.value = pos->x;
        sub_02095CB8(nav, 5, &event);
        return;
    case 4:
        event.value = pos->x;
        sub_02095CB8(nav, 5, &event);
    }
    return;
}
