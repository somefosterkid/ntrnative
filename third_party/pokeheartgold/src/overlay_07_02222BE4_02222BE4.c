#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

void ov07_02222B30(VBlankDMAController *controller, void *param, VBlankDMAFunc dmaFunc, VBlankDMAFunc swapBufferFunc);

BufferManager *sub_02014A08(enum HeapID, void *, void *);

void ov07_02222BC8(void *param);

void ov07_02222BDC(void *param);

CustomBgScrollContext *ov07_02222BE4(void *offsetReg, u32 data, enum HeapID heapID);

CustomBgScrollContext *ov07_02222BE4(void *offsetReg, u32 data, enum HeapID heapID) {
    CustomBgScrollContext *ctx;
    BufferManager *bufferManager;

    ctx = Heap_Alloc(heapID, sizeof(CustomBgScrollContext));
    memset(ctx, 0, sizeof(CustomBgScrollContext));
    if (ctx == NULL) {
        GF_AssertFail();
    }
    bufferManager = sub_02014A08(heapID, ctx->buffer1, ctx->buffer2);
    ctx->bufferManager = bufferManager;
    if (bufferManager == NULL) {
        GF_AssertFail();
    }
    ctx->offsetReg = offsetReg;
    MIi_CpuClear32(data, ctx->buffer1, sizeof(ctx->buffer1));
    MIi_CpuClear32(data, ctx->buffer2, sizeof(ctx->buffer2));
    ov07_02222B30(&ctx->dmaController, ctx, ov07_02222BC8, ov07_02222BDC);
    return ctx;
}

s32 ov07_02222B70(void *);

void sub_02014A38(BufferManager *bufferManager);

void ov07_02222C60(CustomBgScrollContext *ctx);

void ov07_02222C60(CustomBgScrollContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    ov07_02222B70(ctx);
    if (ctx->bufferManager != NULL) {
        sub_02014A38(ctx->bufferManager);
    }
    Heap_Free(ctx);
}

void *sub_02014A4C(BufferManager *);

void ov07_02222C84(CustomBgScrollContext *ctx);

void ov07_02222C84(CustomBgScrollContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    sub_02014A4C(ctx->bufferManager);
}

s32 ov07_02222B94(void *);

void ov07_02222C98(void *arg0);

void ov07_02222C98(void *arg0) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    ov07_02222B94(arg0);
}

s32 sub_02014C08(ScreenScrollManager *);

s32 sub_02014C40(ScreenScrollManager *);

void ov07_02222CAC(BgScrollContext *ctx);

void ov07_02222CAC(BgScrollContext *ctx) {
    sub_02014C08(ctx->screenScrollMgr);
    sub_02014C40(ctx->screenScrollMgr);
}
