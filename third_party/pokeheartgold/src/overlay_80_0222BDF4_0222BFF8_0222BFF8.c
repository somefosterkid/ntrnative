#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

u32 ov80_0222BFE4(u32, u16);

s32 FrtCmd_032(FrontierScriptContext *arg0);

s32 FrtCmd_032(FrontierScriptContext *ctx) {
    u32 v0;

    v0 = FrontierScript_ReadVar(ctx);
    ctx->unk_8C = ov80_0222BFE4(v0, FrontierScriptContext_ReadHalfWord(ctx));
    return 0;
}

s32 FrtCmd_033(FrontierScriptContext *arg0);

s32 FrtCmd_033(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    ctx->unk_8C = ov80_0222BFE4(*v0, *FrontierScript_ReadVarPtr(ctx));
    return 0;
}
