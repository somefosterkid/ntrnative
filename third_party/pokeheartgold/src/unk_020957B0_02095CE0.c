#include "global.h"

#include "heap.h"
#include "unk_020957B0.h"
#include "error_handling.h"

u8 _s32_div_f(u8, u8);

CursorNav *sub_02095CE0(enum HeapID heapID, const u8 *screenMode) {
    u32 v2;
    s8 *v3;
    CursorNav *nav;

    nav = Heap_Alloc(heapID, sizeof(CursorNav));
    v3 = (s8 *)nav;
    v2 = sizeof(CursorNav);
    do {
        *v3 = 0;
        v3 += 1;
        v2 -= 1;
    } while (v2 != 0);
    nav->areaCount = 6U;
    nav->positions = Heap_Alloc(heapID, nav->areaCount * sizeof(CursorNavPos));
    memset(nav->positions, 0, nav->areaCount * sizeof(CursorNavPos));
    nav->screenMode = screenMode;
    return nav;
}

void sub_02095D1C(CursorNav *nav, u8 mode) {
    nav->area = 0;
    nav->areas = _02108434;
    nav->mode = mode;
}

void sub_02095D2C(CursorNav *nav) {
    Heap_Free(nav->positions);
    Heap_Free(nav);
}

void sub_02095D40(CursorNav *nav, u32 areaId, u32 index) {
    u8 uVar1;
    u8 remainder;
    CursorNavPos *pos;
    const CursorNavAreaInfo *info;
    sub_02095C90(nav, areaId);
    pos = nav->positions + nav->area;
    info = nav->areas[nav->area].info;
    if (index >= info->count) {
        GF_AssertFail();
    }
    pos->index = (s8)index;
    if (pos->index >= info->count) {
        pos->index = 0;
    }
    remainder = (s32)pos->index;
    remainder = remainder % ((s32)(info->cols));
    pos->x = remainder;
    uVar1 = _s32_div_f(pos->index, info->cols);
    pos->y = uVar1;
    return;
}
