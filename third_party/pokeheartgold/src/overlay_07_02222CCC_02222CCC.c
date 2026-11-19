#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"
#include "sys_task.h"

void ov07_02222CAC(BgScrollContext *ctx);

void ov07_02222B30(BgScrollContext *, BgScrollContext *, u8 *, u8 *);

void *ov07_02222D90(u32);

ScreenScrollManager *sub_02014AD8(enum HeapID);

void sub_02014B08(ScreenScrollManager *, u8, u8, u16, fx32, s16, void *, u32, u32);

void ov07_02222CC0();

BgScrollContext *ov07_02222CCC(u8 startY, u8 endY, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bgID, u32 priority, u32 initValue, enum HeapID heapID);

BgScrollContext *ov07_02222CCC(u8 startY, u8 endY, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bgID, u32 priority, u32 initValue, enum HeapID heapID) {
    BgScrollContext *pBVar1;
    void *uVar2;
    ScreenScrollManager *pSVar3;

    pBVar1 = Heap_Alloc(heapID, sizeof(BgScrollContext));
    if (pBVar1 == NULL) {
        GF_AssertFail();
    }
    memset(pBVar1, 0, sizeof(BgScrollContext));
    uVar2 = ov07_02222D90(bgID);
    pSVar3 = sub_02014AD8(heapID);
    pBVar1->screenScrollMgr = pSVar3;
    sub_02014B08(pBVar1->screenScrollMgr, startY, endY, angleIncrement, amplitude, (s32)speed, uVar2, initValue, priority);
    ov07_02222B30(pBVar1, pBVar1, (u8 *)(ov07_02222CAC), (u8 *)(ov07_02222CC0));
    return pBVar1;
}
