#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

struct YesNoPrompt;

typedef struct YesNoPrompt YesNoPrompt;

typedef struct YesNoPromptTemplate {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    u32 baseTile;
    u32 palette;
    u8 tilemapLeft;
    u8 tilemapTop;
} YesNoPromptTemplate;

void YesNoPrompt_InitFromTemplate(YesNoPrompt *menu, const YesNoPromptTemplate *params);

void ov52_021E8D64(BgConfig *bgConfig, YesNoPrompt *menu);

void ov52_021E8D64(BgConfig *bgConfig, YesNoPrompt *menu) {
    YesNoPromptTemplate params;

    params.bgConfig = bgConfig;
    params.bgLayer = GF_BG_LYR_MAIN_0;
    params.baseTile = ((1 + ((6 * 3) + ((2 * 2) * 3))) + (3 * 3)) + (27 * 4) + 32;
    params.palette = 8;
    params.tilemapLeft = 25;
    params.tilemapTop = 6;

    YesNoPrompt_InitFromTemplate(menu, &params);
}
