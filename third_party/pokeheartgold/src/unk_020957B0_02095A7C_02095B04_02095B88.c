#include "global.h"

#include "unk_020957B0.h"

void sub_02095B88(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event) {
    switch (event->kind) {
    case 3:
    case 4:
        pos->x = 2;
        pos->y = 0;
        pos->index = pos->x + pos->y * info->cols;
        return;
    case 1:
        pos->x = 5;
        pos->y = 0;
        pos->index = pos->x + pos->y * info->cols;
        return;
    case 2:
        pos->x = 0;
        pos->y = 0;
        pos->index = pos->y * info->cols;
        break;
    }
    return;
}
