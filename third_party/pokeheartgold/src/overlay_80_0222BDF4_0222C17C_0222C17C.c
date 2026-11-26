#include "global.h"

#include "frontier/frontier_script_context.h"

#include "render_text.h"
#include "screen_fade.h"
#include "text.h"
#include "frontier/frontier_script_manager.h"

s32 ov80_0222C17C(void **arg0);

s32 ov80_0222C17C(void **arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    if (TextPrinterCheckActive(((FrontierScriptManager *)(*arg0))->printerID) == 0) {
#else
    if (TextPrinterCheckActive(*(u8 *)((u8 *)(*arg0) + 0x50)) == 0) {
#endif
        TextFlags_SetCanTouchSpeedUpPrint(0);
        return 1;
    }
    return 0;
}

s32 ov80_0222E38C(void *);

s32 FrtCmd_018(void **arg0);

s32 FrtCmd_018(void **arg0) {
    ov80_0222E38C(*arg0);
    return 0;
}

s32 FrtCmd_019(FrontierScriptContext *arg0);

s32 FrtCmd_019(FrontierScriptContext *ctx) {
    u16 steps;
    u16 framesPerStep;
    u16 v0;

    steps = FrontierScriptContext_ReadHalfWord(ctx);
    framesPerStep = FrontierScriptContext_ReadHalfWord(ctx);
    v0 = FrontierScriptContext_ReadHalfWord(ctx);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)v0, (enum FadeType)v0, FrontierScriptContext_ReadHalfWord(ctx), (s32)steps, (s32)framesPerStep, HEAP_ID_FIELD2);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    return 0;
}
