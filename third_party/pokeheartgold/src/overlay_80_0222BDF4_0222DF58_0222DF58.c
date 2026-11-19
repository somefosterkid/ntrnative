#include "global.h"

#include "frontier/frontier_script_context.h"

s32 FrtCmd_150(FrontierScriptContext *arg0);

s32 FrtCmd_150(FrontierScriptContext *ctx) {
    FrontierScriptContext_ReadHalfWord(ctx);
    return 0;
}
