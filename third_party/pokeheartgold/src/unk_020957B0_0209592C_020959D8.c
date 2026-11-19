#include "global.h"

#include "unk_020957B0.h"

void sub_020959D8(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event) {
    s32 v0;
    u8 v1;
    u8 v2;

    v0 = event->kind;
    switch (v0) {
    case 3:
        v2 = info->cols;
        pos->index = (u8)(((s32)(info->count - 1) / (s32)v2) * v2);
        pos->x = (s8)((s32)pos->index % (s32)info->cols);
        pos->y = (u8)((s32)pos->index / (s32)info->cols);
        return;
    case 1:
    case 2:
        v1 = event->value;
        if ((u32)v1 < 2U) {
            pos->index = 0U;
        } else if ((u32)v1 > 2U) {
            pos->index = 2U;
        } else {
            pos->index = 1U;
        }
        pos->x = 0;
        pos->y = (u8)pos->index;
        return;
    }
}
