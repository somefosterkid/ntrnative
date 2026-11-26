#include "global.h"

#include "frontier/frontier.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "overlay_80_02235900.h"
#include "overlay_80_wfcfacility_selector.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov80_02236040(void *, void *, u16);
#else
s32 ov80_02236040(void *, void *, u32);
#endif

s32 FrtCmd_175(FrontierScriptContext *ctx) {
    u16 *v1;
    u32 v2;
    void *v0;

    FrontierScript_ReadVar(ctx);
    v2 = FrontierScript_ReadVar(ctx);
    v1 = FrontierScript_ReadVarPtr(ctx);
    v0 = Frontier_GetData(ctx->frontierSystem->unk0);
    *((void **)&((WFCFacilitySelector *)v0)->fieldSystem) = Frontier_GetLaunchArgs(ctx->frontierSystem->unk0)->fieldSystem;
    ((WFCFacilitySelector *)v0)->subAppResult = v1;
    ov80_02236040(ctx->frontierSystem->unk0, v0, v2);
    return 1;
}
