#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

u16 FrontierScript_ReadVar(FrontierScriptContext *ctx) {
    return ov80_0222BE9C(ctx, FrontierScriptContext_ReadHalfWord(ctx));
}
