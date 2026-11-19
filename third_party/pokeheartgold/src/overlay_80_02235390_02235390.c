#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "unk_02005D10.h"

u32 FrtCmd_085(FrontierScriptContext *param_1);

u32 FrtCmd_085(FrontierScriptContext *param_1) {
    u16 uVar1;

    uVar1 = FrontierScript_ReadVar(param_1);
    PlaySE(uVar1);
    return 0;
}
