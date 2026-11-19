#include "global.h"

#include "overlay_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02016EDC.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "unk_02088288.h"
#include "unk_0208B1AC.h"
#include "unk_0208C3E4.h"
#include "unk_0208DE40.h"

u32 sub_02088288(SaveData *saveData) {
    return 1;
}

BOOL sub_0208828C(SaveData *saveData) {
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(saveData);
    return CheckFlag982(varsFlags);
}

BOOL PokemonSummary_Init(OverlayManager *man, int *state) {
    NARC *narc;
    NARC *nARC;
    PokemonSummary *arg;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFFE0FF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFFE0FF & reg_GXS_DB_DISPCNT);
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFF1FFF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFF1FFF & reg_GXS_DB_DISPCNT);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    SetKeyRepeatTimers(4, 8);
    Heap_Create(HEAP_ID_3, HEAP_ID_19, 0x45000U);
    nARC = NARC_New(NARC_a_0_3_9, HEAP_ID_19);
    narc = NARC_New(NARC_a_1_6_2, HEAP_ID_19);
    arg = OverlayManager_CreateAndGetData(man, sizeof(PokemonSummary), HEAP_ID_19);
    memset(arg, 0, sizeof(PokemonSummary));
    arg->args = OverlayManager_GetArgs(man);
    arg->bgConfig = BgConfig_Alloc(HEAP_ID_19);
    arg->args->unk38 = 0;
    arg->pokepicAnimManager = sub_02016EDC(HEAP_ID_19, 1, 1);
    arg->pokepicAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_19);
    FontID_SetAccessDirect(FONT_SYSTEM, HEAP_ID_19);
    EnableTouchPad();
    InitializeTouchPad(4);
    sub_02088610();
    sub_02088630(arg->bgConfig);
    sub_020887C4(arg, nARC, narc);
    sub_0208887C();
    sub_0208DE40(arg);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_19);
    sub_02088894(arg);
    sub_020889D0(arg, narc);
    sub_020897C0(arg);
    sub_0208B1AC(arg);
    sub_0208B2C0(arg);
    sub_0208E3AC(arg);
    sub_0208B48C(arg);
    sub_0208B4EC(arg);
    sub_0208BECC(arg);
    sub_0208C3E4(arg);
    sub_02089CB4(arg);
    sub_0208DF2C(arg);
    Main_SetVBlankIntrCB(sub_020885DC, arg);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    GfGfx_BothDispOn();
    Sound_SetSceneAndPlayBGM(0x3DU, 0U, 0);
    sub_0203A964();
    NARC_Delete(narc);
    NARC_Delete(nARC);
    return 1;
}
