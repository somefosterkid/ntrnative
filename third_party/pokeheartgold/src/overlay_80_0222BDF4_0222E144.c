#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "save_arrays.h"
#include "unk_02035900.h"

u32 FrtCmd_200(FrontierScriptContext *param_1);

s32 FrtCmd_201(FrontierScriptContext *arg0);

u32 FrtCmd_200(FrontierScriptContext *param_1) {
    FrontierLaunchArgs *pFVar1;

    pFVar1 = Frontier_GetLaunchArgs(param_1->frontierSystem->unk0);
    sub_020270C4(pFVar1->saveData);
    FrontierScript_ReadVar(param_1);
    return 0;
}

s32 FrtCmd_201(FrontierScriptContext *ctx) {
    sub_020378E4((s32)FrontierScript_ReadVar(ctx));
    return 0;
}
