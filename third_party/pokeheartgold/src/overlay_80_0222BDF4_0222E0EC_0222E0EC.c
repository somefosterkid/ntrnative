#include "global.h"

#include "frontier/frontier.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "player_avatar.h"

s32 FrtCmd_075(FrontierScriptContext *arg0);

s32 FrtCmd_075(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 = (u16)sub_0205C298(Frontier_GetLaunchArgs(ctx->frontierSystem->unk0)->saveData);
    return 0;
}

BOOL sub_020304B4(void);

s32 FrtCmd_113(FrontierScriptContext *arg0);

s32 FrtCmd_113(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = FrontierScript_ReadVarPtr(ctx);
    *v0 = sub_020304B4();
    return 0;
}
