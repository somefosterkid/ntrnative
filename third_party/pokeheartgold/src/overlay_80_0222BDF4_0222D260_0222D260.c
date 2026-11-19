#include "global.h"

#include "frontier/overlay_80_0222BDF4.h"

#include "math_util.h"

s32 FrtCmd_056(FrontierScriptContext *arg0);

s32 FrtCmd_056(FrontierScriptContext *ctx) {
    u16 *v1;
    u32 v0;

    v1 = FrontierScript_ReadVarPtr(ctx);
    v0 = FrontierScript_ReadVar(ctx);
    *v1 = (u16)((s32)LCRandom() % (s32)v0);
    return 1;
}
