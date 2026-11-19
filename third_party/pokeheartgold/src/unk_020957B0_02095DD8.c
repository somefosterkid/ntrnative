#include "global.h"

#include "unk_020957B0.h"

u8 sub_02095DD8(CursorNav *nav) {
    return nav->areas[nav->area].info->id;
}

u8 sub_02095DE8(CursorNav *nav) {
    return nav->positions[nav->area].index;
}
