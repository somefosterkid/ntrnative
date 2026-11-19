#include "global.h"

#include "frontier/frontier_script_context.h"

s32 FrtCmd_001(FrontierScriptContext *arg0);

s32 FrtCmd_001(FrontierScriptContext *ctx) {
    FrontierScriptContext_Stop(ctx);
    return 0;
}
