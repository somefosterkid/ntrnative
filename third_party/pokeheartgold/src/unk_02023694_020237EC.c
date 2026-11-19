#include "global.h"
#include "billboard.h"

BillboardListVRAMTransfer *sub_0202068C(int maxElements, enum HeapID heapID);
void sub_020236BC(Billboard *billboard);
BillboardList *sub_0202391C(void);
void sub_02024248(BillboardList *list);
void sub_020206C8(BillboardListVRAMTransfer *listTransfer);
static BOOL sub_020238BC(BillboardList *list);
void sub_02023694(BillboardList *list);
BOOL sub_02023DA4(Billboard *billboard);

BillboardList *sub_020237EC(const BillboardListParams *params);
BOOL sub_02023874(BillboardList *list);
static BOOL sub_020238BC(BillboardList *list);
BOOL sub_020238F8(BillboardList *list);
void sub_02023910(BillboardList *list);

BillboardList *sub_020237EC(const BillboardListParams *params) {
    BillboardList *list = sub_0202391C();

    if (list == 0L) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0L;
    }

    list->active = 1;
    list->draw = 1;
    list->billboards = Heap_Alloc(params->heapID, sizeof(Billboard) * params->maxElements);
    list->capacity = params->maxElements;

    sub_020236BC(&list->sentinelData);

    list->sentinelData.next = &list->sentinelData;
    list->sentinelData.prev = &list->sentinelData;
    list->freeBillboards = Heap_Alloc(params->heapID, sizeof(Billboard *) * params->maxElements);

    sub_02024248(list);
    list->allocator = Heap_Alloc(params->heapID, sizeof(NNSFndAllocator));

    HeapExp_FndInitAllocator(list->allocator, params->heapID, 4);
    list->vramTransfer = sub_0202068C(params->maxElements, params->heapID);

    return list;
}

BOOL sub_02023874(BillboardList *list) {
    if (list == 0L) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    if (list->active) {
        sub_020238BC(list);
        Heap_Free(list->billboards);
        Heap_Free(list->freeBillboards);
        Heap_Free(list->allocator);
        sub_020206C8(list->vramTransfer);
        sub_02023694(list);
    }

    return 1;
}

static BOOL sub_020238BC(BillboardList *list) {
    Billboard *billboard, *next;

    if (list == 0L) {
        ((list) ? (void)0 : GF_AssertFail());
        return 0;
    }

    if (list->active) {
        billboard = list->sentinelData.next;

        while (billboard != &list->sentinelData) {
            next = billboard->next;
            sub_02023DA4(billboard);
            billboard = next;
        }
    }

    return 1;
}

BOOL sub_020238F8(BillboardList *list) {
    ((list) ? (void)0 : GF_AssertFail());

    if (list->redraw == BILLBOARD_REDRAW_NONE) {
        return 0;
    }

    return 1;
}

void sub_02023910(BillboardList *list) {
    if (list->redraw == BILLBOARD_REDRAW_FINISHED) {
        list->redraw = BILLBOARD_REDRAW_NONE;
    }
}
