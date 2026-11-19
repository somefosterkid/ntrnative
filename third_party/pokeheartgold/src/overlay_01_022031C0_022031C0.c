#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_01_022031C0.h"

void *ov01_022031C0(FieldEffectManager *fieldEffMan) {
    UnkOv01_022031C0 *ctx;

    ctx = ov01_021F1430(fieldEffMan, sizeof(UnkOv01_022031C0), 0, 0);
    ctx->fxMgr = fieldEffMan;
    HeapExp_FndInitAllocator(&ctx->alloc, HEAP_ID_FIELD1, 0x20);
    ov01_022031F8(ctx);
    return ctx;
}

void ov01_022031E8(void *a0) {
    UnkOv01_022031C0 *ctx = a0;

    ov01_02203270(ctx);
    ov01_021F1448(ctx);
}
