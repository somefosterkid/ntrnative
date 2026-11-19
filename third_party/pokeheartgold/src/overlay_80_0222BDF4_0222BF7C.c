#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

s32 FrtCmd_006(FrontierScriptContext *arg0);

s32 FrtCmd_009(FrontierScriptContext *arg0);

u32 ov80_0222BFE4(u16 value0, u16 value1);

s32 FrtCmd_006(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 = FrontierScriptContext_ReadHalfWord(ctx);
    return 0;
}

s32 FrtCmd_007(FrontierScriptContext *arg0);

s32 FrtCmd_007(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 = *FrontierScript_ReadVarPtr(ctx);
    return 0;
}

s32 FrtCmd_008(FrontierScriptContext *arg0);

s32 FrtCmd_008(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 += FrontierScript_ReadVar(ctx);
    return 0;
}

s32 FrtCmd_009(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 -= FrontierScript_ReadVar(ctx);
    return 0;
}

u32 ov80_0222BFE4(u16 value0, u16 value1) {
    if (value0 < value1) {
        return 0;
    } else if (value0 == value1) {
        return 1;
    } else {
        return 2;
    }
}
