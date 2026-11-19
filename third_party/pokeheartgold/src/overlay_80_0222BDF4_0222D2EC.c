#include "global.h"

#include "system.h"
#include "touchscreen.h"
#include "frontier/overlay_80_0222BDF4.h"

s32 ov80_0222D2EC(void *arg0);

s32 FrtCmd_060(FrontierScriptContext *arg0);

s32 ov80_0222D2EC(void *arg0) {
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() != 0)) {
        return 1;
    }
    *(u16 *)((u8 *)arg0 + 0x78) = (u16)(*(u16 *)((u8 *)arg0 + 0x78) - 1);
    if (*(u16 *)((u8 *)arg0 + 0x78) == 0) {
        return 1;
    }
    return 0;
}

s32 FrtCmd_060(FrontierScriptContext *ctx) {
    FrontierScript_ReadVarPtr(ctx);
    return 1;
}
