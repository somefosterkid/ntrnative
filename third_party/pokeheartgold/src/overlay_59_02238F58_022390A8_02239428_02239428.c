#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "gf_gfx_planes.h"

void ov59_02239408(void);

extern const GraphicsModes ov59_0223C658;

extern const BgTemplate ov59_0223C718;

extern const BgTemplate ov59_0223C68C;

extern const BgTemplate ov59_0223C6A8;

extern const BgTemplate ov59_0223C6FC;

extern const BgTemplate ov59_0223C734;

extern const BgTemplate ov59_0223C6E0;

void ov59_02239428(Ov59ApricornApp *param0);

void ov59_02239428(Ov59ApricornApp *param0) {
    ov59_02239408();
    param0->bgConfig = BgConfig_Alloc(param0->heapId);

    GraphicsModes graphicsModes = ov59_0223C658;

    SetBothScreensModesAndDisable(&graphicsModes);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    BgTemplate sub0Template = ov59_0223C718;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_0, &sub0Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate sub3Template = ov59_0223C68C;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_SUB_3, &sub3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_SUB_3);

    BgTemplate main0Template = ov59_0223C6A8;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_0, &main0Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate main1Template = ov59_0223C6FC;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_1, &main1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate main2Template = ov59_0223C734;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_2, &main2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate main3Template = ov59_0223C6E0;

    InitBgFromTemplate(param0->bgConfig, GF_BG_LYR_MAIN_3, &main3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param0->bgConfig, GF_BG_LYR_MAIN_3);

    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, param0->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, param0->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, param0->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, param0->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, param0->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 64, 0, param0->heapId);
}
