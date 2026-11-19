#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223B414(void);
extern const GraphicsModes ov59_0223C978;
extern const BgTemplate ov59_0223C9B0;
extern const BgTemplate ov59_0223C9CC;
extern const BgTemplate ov59_0223C9E8;
extern const BgTemplate ov59_0223CA04;

void ov59_0223B434(Ov59JuiceFeedApp *param_1);
void ov59_0223B434(Ov59JuiceFeedApp *param_1) {
    ov59_0223B414();
    param_1->bgConfig = BgConfig_Alloc(param_1->heapId);
    GraphicsModes graphicsModes = ov59_0223C978;
    SetBothScreensModesAndDisable(&graphicsModes);
    reg_GX_POWCNT &= ~0x8000;
    BgTemplate main0Template = ov59_0223C9B0;
    InitBgFromTemplate(param_1->bgConfig, GF_BG_LYR_SUB_3, &main0Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param_1->bgConfig, GF_BG_LYR_SUB_3);
    BgTemplate main1Template = ov59_0223C9CC;
    InitBgFromTemplate(param_1->bgConfig, GF_BG_LYR_MAIN_1, &main1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param_1->bgConfig, GF_BG_LYR_MAIN_1);
    BgTemplate main2Template = ov59_0223C9E8;
    InitBgFromTemplate(param_1->bgConfig, GF_BG_LYR_MAIN_2, &main2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param_1->bgConfig, GF_BG_LYR_MAIN_2);
    BgTemplate main3Template = ov59_0223CA04;
    InitBgFromTemplate(param_1->bgConfig, GF_BG_LYR_MAIN_3, &main3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(param_1->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, (8 * (8 / 2)), 0, param_1->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, (8 * (8 / 2)), 0, param_1->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, (8 * (8 / 2)), 0, param_1->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, (8 * (8 / 2)), 0, param_1->heapId);
}
