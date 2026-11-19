#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "menu_input_state.h"
#include "render_text.h"
#include "system.h"
#include "unk_0208805C.h"
#include "vram_transfer_manager.h"
#include "overlay_68_move_relearner.h"

void ov68_021E5B6C(void *);

s32 ov68_021E5BA0(void);

s32 ov68_021E5BC0(BgConfig *);

void ov68_021E5D24(MoveRelearner *, NARC *);

s32 ov68_021E5E48(MoveRelearner *);

s32 ov68_021E5EBC(MoveRelearner *);

s32 ov68_021E6204(MoveRelearner *);

s32 ov68_021E6320(MoveRelearner *);

s32 ov68_021E6820(MoveRelearner *);

void ov68_021E7178(MoveRelearner *, NARC *);

s32 ov68_021E7288(MoveRelearner *);

s32 ov68_021E75C0(MoveRelearner *);

void ov68_021E5A58(MoveRelearner *arg0);

void ov68_021E5A58(MoveRelearner *arg) {
    NARC *narc;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    TextFlags_SetCanTouchSpeedUpPrint(1);
    arg->menuInputState = MenuInputStateMgr_GetState(arg->args->menuInputStatePtr);
    arg->bgConfig = BgConfig_Alloc(HEAP_ID_66);
    narc = NARC_New(NARC_a_1_1_0, HEAP_ID_66);
    ov68_021E5BA0();
    ov68_021E5BC0(arg->bgConfig);
    ov68_021E5D24(arg, narc);
    ov68_021E7178(arg, narc);
    ov68_021E7288(arg);
    ov68_021E5EBC(arg);
    ov68_021E5E48(arg);
    ov68_021E6820(arg);
    ov68_021E6204(arg);
    ov68_021E6320(arg);
    ov68_021E75C0(arg);
    App_StartScreenFade(0U, HEAP_ID_66);
    Main_SetVBlankIntrCB(ov68_021E5B6C, arg);
    NARC_Delete(narc);
}

s32 ov68_021E5CD8(BgConfig *);

s32 ov68_021E5E38(MoveRelearner *);

s32 ov68_021E5E94(MoveRelearner *);

s32 ov68_021E5F18(MoveRelearner *);

s32 ov68_021E68C4(MoveRelearner *);

s32 ov68_021E6E84(MoveRelearner *);

void ov68_021E7388(MoveRelearnerMonPic *);

s32 ov68_021E7604(MoveRelearner *arg0);

void ov68_021E5B14(MoveRelearner *arg0);

void ov68_021E5B14(MoveRelearner *arg0) {
    ov68_021E7604(arg0);
    ov68_021E68C4(arg0);
    ov68_021E5E94(arg0);
    ov68_021E5E38(arg0);
    ov68_021E5CD8(arg0->bgConfig);
    ov68_021E5F18(arg0);
    ov68_021E7388(&arg0->monPic);
    ov68_021E6E84(arg0);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, arg0->menuInputState);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    GF_DestroyVramTransferManager();
    Main_SetVBlankIntrCB(NULL, NULL);
}
