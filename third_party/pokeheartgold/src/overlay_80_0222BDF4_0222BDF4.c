#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "error_handling.h"

u16 *ov80_0222BE24(FrontierScriptContext *, u16);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx) {
    u16 *v0;

    v0 = ov80_0222BE24(ctx, FrontierScriptContext_ReadHalfWord(ctx));
    if (v0 == NULL) {
        GF_AssertFail();
    }
    return v0;
}
