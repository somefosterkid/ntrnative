#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_67_app.h"

extern const GraphicsModes ov67_021E6D5C;

extern const BgTemplate ov67_021E6DA8;

extern const BgTemplate ov67_021E6DFC;

extern const BgTemplate ov67_021E6DC4;

extern const BgTemplate ov67_021E6DE0;

void ov67_021E5DA0(Ov67App *param0) {
    param0->bgConfig = BgConfig_Alloc(param0->heapID);

    GraphicsModes graphicsModes = ov67_021E6D5C;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate1 = ov67_021E6DA8;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, param0->heapID);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate2 = ov67_021E6DFC;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_1, &bgTemplate2, GF_BG_TYPE_TEXT);

    BgTemplate bgTemplate3 = ov67_021E6DC4;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_0, &bgTemplate3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, param0->heapID);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplate4 = ov67_021E6DE0;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_1, &bgTemplate4, GF_BG_TYPE_TEXT);
}
