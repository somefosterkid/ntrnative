#include "global.h"
#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "unk_0208805C.h"
#include "heap.h"
#include "overlay_68_move_relearner.h"
struct BgConfig;
struct Options;

void ov68_021E5D24(MoveRelearner *arg0, NARC *param_2);

void ov68_021E5E38(MoveRelearner *arg0);

void ov68_021E5D24(MoveRelearner *arg0, NARC *param_2) {
    void *pvVar1;
    u32 uVar2;

    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 1, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 4, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_SCRN, 0x800, 0);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 0, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 1, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 2, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_SCRN, 0x800, 0);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, HEAP_ID_66, param_2, NARC_a_1_1_0, 0, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    pvVar1 = GfGfxLoader_GetScrnDataFromOpenNarc(param_2, 3, 0, &arg0->screenData, HEAP_ID_66);
    arg0->screenFile = pvVar1;
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_66);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_66);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_66);
    uVar2 = Options_GetFrame(arg0->args->options);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_MAIN_CNT, 0x3d, 0xd, (u8)uVar2, HEAP_ID_66);
    return;
}

void ov68_021E5E38(MoveRelearner *arg0) {
    Heap_Free(arg0->screenFile);
}
