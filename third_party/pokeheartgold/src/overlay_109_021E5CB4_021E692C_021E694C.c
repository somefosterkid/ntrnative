#include "global.h"

#include "bg_window.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "overlay_109_photo_album_app.h"

extern const GraphicsModes ov109_021E78B8;
extern const BgTemplate ov109_021E7944;
extern const BgTemplate ov109_021E797C;
extern const BgTemplate ov109_021E7998;
extern const BgTemplate ov109_021E790C;
extern const BgTemplate ov109_021E7928;
extern const BgTemplate ov109_021E7960;
extern const BgTemplate ov109_021E79B4;

void ov109_021E694C(PhotoAlbumApp *app) {
    ov109_021E692C();
    app->bgConfig = BgConfig_Alloc(app->heapID);

    GraphicsModes gfxModes = ov109_021E78B8;

    SetBothScreensModesAndDisable(&gfxModes);
    reg_GX_POWCNT &= ~0x8000;

    BgTemplate bgSub0 = ov109_021E7944;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_0, &bgSub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgSub3 = ov109_021E797C;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &bgSub3, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);

    BgTemplate bgSub2 = ov109_021E7998;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &bgSub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate bgMain0 = ov109_021E790C;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_0, &bgMain0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgMain1 = ov109_021E7928;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &bgMain1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgMain2 = ov109_021E7960;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &bgMain2, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate bgMain3 = ov109_021E79B4;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &bgMain3, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);

    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x40, 0, app->heapID);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, -4);
}
