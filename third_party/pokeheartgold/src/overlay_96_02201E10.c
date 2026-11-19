#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "overlay_96_ov96_02203468work.h"
#include "overlay_96_ov96_sprite_mgr.h"

s32 ov96_02203A64(void *, s32);

void ov96_02201E10(Ov96_02203468Work *arg0, s32 arg1);

void ov96_02201E10(Ov96_02203468Work *arg0, s32 arg1) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 1, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x4000U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 2, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, arg0->heapID);
    ov96_02203A64(arg0->unk_5E0, arg1);
}

void ov96_02201E70(Ov96SpriteMgr *param_1);

void ov96_02201E70(Ov96SpriteMgr *param_1) {
    ov96_021EB2BC(param_1, NARC_a_2_1_7, 0x15, 0x6a, 3);
    ov96_021EB2F4(param_1, NARC_a_2_1_7, 0x12, 0x6a, 3, 2);
    ov96_021EB334(param_1, NARC_a_2_1_7, 0x14, 0x6a);
    ov96_021EB36C(param_1, NARC_a_2_1_7, 0x13, 0x6a);
    ov96_021EB2BC(param_1, NARC_a_2_1_7, 0x11, 0x69, 1);
    ov96_021EB2F4(param_1, NARC_a_2_1_7, 0xe, 0x69, 1, 3);
    ov96_021EB334(param_1, NARC_a_2_1_7, 0x10, 0x69);
    ov96_021EB36C(param_1, NARC_a_2_1_7, 0xf, 0x69);
    return;
}
