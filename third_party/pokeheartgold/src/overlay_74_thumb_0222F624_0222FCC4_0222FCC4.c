#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov74_0222FCC4(BgConfig *param0);

extern const GraphicsModes _0223B430;

extern const BgTemplate _0223B45C;

extern const BgTemplate _0223B494;

extern const BgTemplate _0223B478;

extern const BgTemplate _0223B440;

void ov74_0222FCC4(BgConfig *param0) {
    {
        GraphicsModes v0 = _0223B430;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = _0223B45C;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
    }

    {
        BgTemplate v2 = _0223B494;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v3 = _0223B478;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v3, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v4 = _0223B440;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v4, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
    }
}
