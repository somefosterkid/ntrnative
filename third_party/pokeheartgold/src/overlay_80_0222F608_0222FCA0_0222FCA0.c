#include "global.h"

#include "frontier/frontier.h"
#include "frontier/overlay_80_0222BDF4.h"

s32 ov80_0222FCA0(FrontierScriptContext *arg0);

s32 ov80_0222FCA0(FrontierScriptContext *ctx) {
    void *v0;

    ov80_0222BE9C(ctx, ctx->data[0]);
    v0 = Frontier_GetData(ctx->frontierSystem->unk0);
    if ((u32) * (u8 *)((u8 *)v0 + 0x702) >= 2U) {
        *(u8 *)((u8 *)v0 + 0x702) = 0U;
        return 1;
    }
    return 0;
}
