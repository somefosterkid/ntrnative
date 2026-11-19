#include "global.h"

#include "unk_020957B0.h"

void sub_02095A34(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy) {
    CursorNavEvent event;
    event.kind = sub_02095BF0(nav, pos, info, dx, dy);
    event.value = 0;
    switch (event.kind) {
    case 3:
        sub_02095CB8(nav, 4, &event);
        return;
    case 4:
        sub_02095CB8(nav, 4, &event);
        return;
    }
}
