#include "global.h"

#include "heap.h"
#include "unk_02023694.h"
#include "billboard.h"

void sub_02023874(BillboardList *list);

void BillboardLists_Delete(void) {
    s32 i;

    i = 0;
    if (0 < _021D2208.count) {
        do {
            sub_02023874(&_021D2208.lists[i]);
            i = i + 1;
        } while (i < _021D2208.count);
    }
    Heap_Free(_021D2208.lists);
    _021D2208.lists = NULL;
    _021D2208.count = 0;
    return;
}
