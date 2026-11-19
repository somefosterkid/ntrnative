#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ov74_02232974(BgConfig *param0);

extern const GraphicsModes _0223B4F8;

extern const BgTemplate _0223B508;

extern const BgTemplate _0223B524;

extern const BgTemplate _0223B540;

extern const BgTemplate _0223B55C;

void ov74_02232974(BgConfig *param0) {
    {
        GraphicsModes v0 = _0223B4F8;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = _0223B508;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v1, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
    }

    {
        BgTemplate v2 = _0223B524;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v2, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v3 = _0223B540;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v3, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v4 = _0223B55C;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v4, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
    }
}
