#include "global.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "overlay_34.h"

extern const BgTemplate ov34_0225E714;
extern const BgTemplate ov34_0225E6F8;
extern const BgTemplate ov34_0225E6C0;
extern const BgTemplate ov34_0225E6DC;

void ov34_0225D924(BgConfig *bgConfig) {
    ov34_0225D900(bgConfig);
    {
        BgTemplate template0 = ov34_0225E714;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template0, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
    }
    {
        BgTemplate template1 = ov34_0225E6F8;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template1, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 0);
    }
    {
        BgTemplate template2 = ov34_0225E6C0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template2, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
    }
    {
        BgTemplate template3 = ov34_0225E6DC;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &template3, 0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 0);
    }
    GfGfxLoader_GXLoadPal(NARC_a_0_7_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20 * 3, HEAP_ID_FIELD1);
    GfGfxLoader_LoadCharData(NARC_a_0_7_3, 2, bgConfig, GF_BG_LYR_SUB_3, 0, (32 * 5) * 0x20, 1, HEAP_ID_FIELD1);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_3, 4, bgConfig, GF_BG_LYR_SUB_3, 0, 32 * 24 * 2, 1, HEAP_ID_FIELD1);
    FieldMessage_LoadTextPalettes(GF_PAL_LOCATION_SUB_BG, 0);
}
