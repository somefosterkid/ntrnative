#include "global.h"

#include "frontier/frontier_map.h"
#include "frontier/frontier_script_manager.h"
#include "frontier/frontier_system.h"

#include "bg_window.h"
#include "render_window.h"

void ov80_0222E2B8(FrontierSystem *param_1);

void ov80_0222E2B8(FrontierSystem *param_1) {
    FrontierMap *pFVar1;

#if defined(SDK_PORT) || defined(NONMATCHING)
    // param_1 is really the FrontierScriptManager; param_1[1].unk4 strides by the 64-bit
    // sizeof(FrontierSystem) and lands on string/fmtString instead of msgWindow.
    FrontierScriptManager *scriptMan = (FrontierScriptManager *)param_1;

    pFVar1 = FrontierSystem_GetFrontierMap(param_1);
    if (scriptMan->isMsgBoxOpen == 0) {
        AddWindowParameterized(pFVar1->bgConfig, &scriptMan->msgWindow, 1, 2, 0x13, 0x1b, 4, 0xd, 0x36d);
        FillWindowPixelBuffer(&scriptMan->msgWindow, 0xf);
        DrawFrameAndWindow2(&scriptMan->msgWindow, 0, 0x3e2, 0xb);
        scriptMan->isMsgBoxOpen = 1;
        return;
    }
    FillWindowPixelBuffer(&scriptMan->msgWindow, 0xf);
    return;
#else
    pFVar1 = FrontierSystem_GetFrontierMap(param_1);
    if (param_1[1].unk4[0xe] == 0) {
        AddWindowParameterized(pFVar1->bgConfig, (Window *)(param_1[1].unk4 + 0x18), 1, 2, 0x13, 0x1b, 4, 0xd, 0x36d);
        FillWindowPixelBuffer((Window *)(param_1[1].unk4 + 0x18), 0xf);
        DrawFrameAndWindow2((Window *)(param_1[1].unk4 + 0x18), 0, 0x3e2, 0xb);
        param_1[1].unk4[0xe] = 1;
        return;
    }
    FillWindowPixelBuffer((Window *)(param_1[1].unk4 + 0x18), 0xf);
    return;
#endif
}
