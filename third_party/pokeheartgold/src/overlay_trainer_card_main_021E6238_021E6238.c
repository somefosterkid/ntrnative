#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov51_021E6D44(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 *signature);

void ov51_021E6238(BgConfig *bgConfig);

extern const GraphicsModes ov51_021E7DC8;

extern const BgTemplate ov51_021E7E8C;

typedef struct {
    BgTemplate a[2];
} ov51_021E7ED0_Array;

extern const ov51_021E7ED0_Array ov51_021E7ED0;

extern const BgTemplate ov51_021E7E70;

extern const BgTemplate ov51_021E7E38;

extern const BgTemplate ov51_021E7E54;

void ov51_021E6238(BgConfig *bgConfig) {
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;

    GraphicsModes graphicsModes = ov51_021E7DC8;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate sub3Template = ov51_021E7E8C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3Template, BG_TYPE_STATIC_WITH_AFFINE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);

    ov51_021E7ED0_Array sub1Templates = ov51_021E7ED0;

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)4, &sub1Templates.a[0], BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)4);

    InitBgFromTemplate(bgConfig, (enum GFBgLayer)5, &sub1Templates.a[1], BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum GFBgLayer)5);

    BgTemplate sub2Template = ov51_021E7E70;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2Template, (enum BgType)2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate main2Template = ov51_021E7E38;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2Template, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate main3Template = ov51_021E7E54;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3Template, (enum BgType)0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
}
