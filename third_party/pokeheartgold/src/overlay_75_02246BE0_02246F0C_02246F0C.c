#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_75.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "render_text.h"
#include "save.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_020210A0.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

void ov75_02246BE0(UnkStruct_ov98_02246E88 *arg0, s8 arg1);

void ov75_0224725C(BgConfig *param0);

void ov75_02247450(UnkStruct_ov98_02247704 *param0);

void ov75_02247790(UnkStruct_ov98_02247704 *param0);

void ov75_02247234(void *param0);

BOOL ov75_02246F0C(OverlayManager *man, int *state);

BOOL ov75_02246F0C(OverlayManager *man, int *state) {
    UnkStruct_ov98_02247704 *work;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();

    Heap_Create(HEAP_ID_3, HEAP_ID_116, 0x70000);
    work = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov98_02247704), HEAP_ID_116);
    MI_CpuFill8(work, 0, sizeof(UnkStruct_ov98_02247704));
    work->unk_00 = OverlayManager_GetArgs(man);
    work->unk_04 = BgConfig_Alloc(HEAP_ID_116);
    GF_CreateVramTransferManager(64, HEAP_ID_116);
    SetKeyRepeatTimers(4, 8);
    ov75_0224725C(work->unk_04);
    EnableTouchPad();
    InitializeTouchPad(4);

    work->unk_20 = MessageFormat_New_Custom(11, 64, HEAP_ID_116);
    work->unk_24 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x307, HEAP_ID_116);
    work->unk_2C = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x30A, HEAP_ID_116);
    work->unk_30 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x320, HEAP_ID_116);
    work->unk_28 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xED, HEAP_ID_116);
    work->unk_34 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xBC, HEAP_ID_116);
    work->unk_38 = String_New(90 * 3, HEAP_ID_116);
    work->unk_40 = String_New(16 * 8 * 2, HEAP_ID_116);
    work->unk_3C = NewString_ReadMsgData(work->unk_24, 31);

    ov75_02247450(work);
    ov75_02247790(work);

    switch (ov75_02246BE8(work->unk_00)) {
    case 12:
        if (!DWC_CheckInet() && sub_0203A05C(work->unk_00->saveData) == TRUE) {
            work->unk_08 = 12;
            sub_02039528(work->unk_00->saveData);
            sub_0203A880();
        } else {
            work->unk_08 = 0;
        }
        break;
    case 22:
        work->unk_E8 = 0;
        sub_0203A880();
        work->unk_08 = ov75_02246BE8(work->unk_00);
        break;
    default:
        work->unk_08 = ov75_02246BE8(work->unk_00);
        break;
    }

    ov75_02246BE0(work->unk_00, 0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_116);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    Main_SetVBlankIntrCB(ov75_02247234, work);
    return TRUE;
}
