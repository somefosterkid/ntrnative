#include "global.h"

#include "frontier/frontier_map.h"
#include "frontier/frontier_system.h"

#include "bg_window.h"
#include "render_window.h"

void ov80_0222E2B8(FrontierSystem *param_1);

void ov80_0222E2B8(FrontierSystem *param_1) {
    FrontierMap *pFVar1;

    pFVar1 = FrontierSystem_GetFrontierMap(param_1);
    if (param_1[1].unk4[0xe] == 0) {
        AddWindowParameterized((BgConfig *)pFVar1->unk0, (Window *)(param_1[1].unk4 + 0x18), 1, 2, 0x13, 0x1b, 4, 0xd, 0x36d);
        FillWindowPixelBuffer((Window *)(param_1[1].unk4 + 0x18), 0xf);
        DrawFrameAndWindow2((Window *)(param_1[1].unk4 + 0x18), 0, 0x3e2, 0xb);
        param_1[1].unk4[0xe] = 1;
        return;
    }
    FillWindowPixelBuffer((Window *)(param_1[1].unk4 + 0x18), 0xf);
    return;
}
