#include "global.h"

#include "bg_window.h"
#include "overlay_113.h"

void ov113_021E62E0(void);

extern const GraphicsModes ov113_021E6B94;

extern const BgTemplate ov113_021E6C3C;

extern const BgTemplate ov113_021E6C74;

extern const BgTemplate ov113_021E6BE8;

extern const BgTemplate ov113_021E6C04;

extern const BgTemplate ov113_021E6C20;

extern const BgTemplate ov113_021E6C58;

void ov113_021E6300(UnownReportData *param);

void ov113_021E6300(UnownReportData *param) {
    ov113_021E62E0();
    param->bgConfig = BgConfig_Alloc(param->heapID);

    GraphicsModes graphicsModes = ov113_021E6B94;

    SetBothScreensModesAndDisable(&graphicsModes);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    BgTemplate sub3Template = ov113_021E6C3C;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_SUB_1, &sub3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate sub1Template = ov113_021E6C74;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_SUB_2, &sub1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate sub2Template = ov113_021E6BE8;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_SUB_3, &sub2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_SUB_3);

    BgTemplate main1Template = ov113_021E6C04;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_MAIN_1, &main1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate main2Template = ov113_021E6C20;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_MAIN_2, &main2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate main3Template = ov113_021E6C58;

    InitBgFromTemplate(param->bgConfig, GF_BG_LYR_MAIN_3, &main3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param->bgConfig, GF_BG_LYR_MAIN_3);

    BG_ClearCharDataRange(5, 0x20, 0, param->heapID);
    BG_ClearCharDataRange(6, 0x20, 0, param->heapID);
    BG_ClearCharDataRange(7, 0x20, 0, param->heapID);
    BG_ClearCharDataRange(1, 0x20, 0, param->heapID);
    BG_ClearCharDataRange(2, 0x20, 0, param->heapID);
    BG_ClearCharDataRange(3, 0x20, 0, param->heapID);
}
