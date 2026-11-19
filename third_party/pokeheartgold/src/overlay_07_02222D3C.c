#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

s32 ov07_02222B70(void *);

void sub_02014BD8(ScreenScrollManager *screenScrollMgr);

void *ov07_02222D90(int bgID);

void ov07_02222D3C(BgScrollContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    ov07_02222B70(ctx);
    if (ctx->screenScrollMgr != NULL) {
        sub_02014BD8(ctx->screenScrollMgr);
    }
    Heap_Free(ctx);
}

void *sub_02014BF8(ScreenScrollManager *);

void ov07_02222D60(BgScrollContext *ctx);

void ov07_02222D60(BgScrollContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    sub_02014BF8(ctx->screenScrollMgr);
}

s32 ov07_02222B94(void *);

void ov07_02222D74(void *arg0);

void ov07_02222D74(void *arg0) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    ov07_02222B94(arg0);
}

s32 ov07_02222D88(s32 arg0, s32 arg1);

s32 ov07_02222D88(s32 arg0, s32 arg1) {
    return arg0 | (arg1 << 0x10);
}

void *ov07_02222D90(int bgID) {
    switch (bgID) {
    case 0:
        return (void *)REG_BG0OFS_ADDR;
    case 1:
        return (void *)REG_BG1OFS_ADDR;
    case 2:
        return (void *)REG_BG2OFS_ADDR;
    case 3:
        return (void *)REG_BG3OFS_ADDR;
    }
#if defined(SDK_PORT)
    return NULL;
#endif
}
