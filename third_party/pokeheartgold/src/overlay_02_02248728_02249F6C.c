#include "global.h"
struct HMCutIn;

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "dialog_box.h"
#include "overlay_02_hmcut_in.h"
#include "overworld_anim_manager.h"

NARC *ov02_0224A074(void);

void ov02_0224A080(HMCutIn *, NARC *);

void ov02_02249F6C(struct HMCutIn *arg0);

void ov02_0224A288(HMCutIn *);
void ov02_0224A63C(BgConfig *);
void ov02_02249FD4(struct HMCutIn *arg0);

void ov02_02249F6C(HMCutIn *arg0) {
    NARC *narc;

    narc = ov02_0224A074();
    arg0->bg0Priority = GetBgPriority(arg0->fieldSystem->bgConfig, 0U);
    arg0->bg3Priority = GetBgPriority(arg0->fieldSystem->bgConfig, 3U);
    reg_G2_BG1CNT = (u16)(1 | (reg_G2_BG1CNT & ~3));
    reg_G2_BG3CNT = (u16)(reg_G2_BG3CNT & ~3);
    GfGfx_EngineATogglePlanes(8U, 0U);
    ov02_0224A080(arg0, narc);
    NARC_Delete(narc);
    arg0->animManagerList = sub_020689C8(HEAP_ID_FIELD1, 0x20);
    GfGfx_EngineATogglePlanes(8U, 1U);
}

void ov02_02249FD4(HMCutIn *arg0) {
    GfGfx_EngineATogglePlanes(8U, 0U);
    sub_020689F8(arg0->animManagerList);
    ov02_0224A63C(arg0->fieldSystem->bgConfig);
    ov02_0224A288(arg0);
    reg_G2_BG0CNT = (u16)((reg_G2_BG0CNT & ~3) | arg0->bg0Priority);
    reg_G2_BG3CNT = (u16)((reg_G2_BG3CNT & ~3) | arg0->bg3Priority);
    FieldMessage_LoadTextPalettes(GF_PAL_LOCATION_MAIN_BG, 1);
    GfGfx_EngineATogglePlanes(8U, 1U);
}
