#include "global.h"

#include "unk_020957B0.h"

void sub_02095838(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event) {
    u32 kind;

    kind = event->kind;
    switch (kind) {
    case 4:
        pos->x = 2;
        pos->y = 0;
        pos->index = pos->x + (pos->y * info->cols);
        return;
    case 3:
        pos->x = 2;
        pos->y = 4;
        pos->index = pos->x + (pos->y * info->cols);
        return;
    case 1:
        pos->x = 5;
        pos->y = event->value;
        pos->index = pos->x + (pos->y * info->cols);
        return;
    case 2:
        pos->x = 0;
        pos->y = event->value;
        pos->index = pos->y * info->cols;
    default:
        return;
    }
}
