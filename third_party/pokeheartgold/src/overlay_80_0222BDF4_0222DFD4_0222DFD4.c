#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/frontier_system.h"

s32 ov80_0222F1D0(FrontierMap *, int);

s32 FrtCmd_070(FrontierScriptContext *arg0);

s32 FrtCmd_070(FrontierScriptContext *ctx) {
    FrontierMap *frontierMap;

    frontierMap = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    ov80_0222F1D0(frontierMap, FrontierScriptContext_ReadHalfWord(ctx));
    return 0;
}
