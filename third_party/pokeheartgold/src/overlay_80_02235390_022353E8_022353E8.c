#include "global.h"

#include "frontier/frontier_script_context.h"

#include "overlay_80_02235390.h"
#include "unk_02005D10.h"

s32 FrtCmd_088(FrontierScriptContext *ctx) {
    PlayFanfare(FrontierScriptContext_ReadHalfWord(ctx));
    return 0;
}
