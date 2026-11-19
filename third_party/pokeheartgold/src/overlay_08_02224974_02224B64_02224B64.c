#include "global.h"

#include "heap.h"
#include "overlay_08_battle_sub_menu_cursor.h"

BattleSubMenuCursor *ov08_02224B64(enum HeapID heapID);

BattleSubMenuCursor *ov08_02224B64(enum HeapID heapID) {
    void *cursor;
    u32 v0;
    s8 *v1;
    cursor = Heap_Alloc(heapID, sizeof(BattleSubMenuCursor));
    v1 = cursor;
    v0 = sizeof(BattleSubMenuCursor);
    do {
        *v1 = 0;
        v1 += 1;
        v0 -= 1;
    } while (v0 != 0);
    return cursor;
}
