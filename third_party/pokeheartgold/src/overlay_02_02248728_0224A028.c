#include "global.h"
struct HMCutIn;

#include "dialog_box.h"
#include "gf_gfx_planes.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_02_hmcut_in.h"
#include "overworld_anim_manager.h"

void ov02_0224A288(HMCutIn *);

void ov02_0224A028(struct HMCutIn *arg0);

NARC *ov02_0224A074(void);

void ov02_0224A028(HMCutIn *arg0) {
    GfGfx_EngineATogglePlanes(8U, 0U);
    sub_020689F8(arg0->animManagerList);
    ov02_0224A288(arg0);
    reg_G2_BG0CNT = (u16)((reg_G2_BG0CNT & ~3) | arg0->bg0Priority);
    reg_G2_BG3CNT = (u16)((reg_G2_BG3CNT & ~3) | arg0->bg3Priority);
    FieldMessage_LoadTextPalettes(GF_PAL_LOCATION_MAIN_BG, 1);
    GfGfx_EngineATogglePlanes(8U, 1U);
}

NARC *ov02_0224A074(void) {
    return NARC_New(NARC_application_choose_starter_choose_starter_sub_res, HEAP_ID_FIELD1);
}
