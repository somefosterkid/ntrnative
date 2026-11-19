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
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7B18(Ov112PokewalkerApp *param_1);

void ov112_021E7C18(Ov112PokewalkerApp *arg0);
void ov112_021E7C6C(Ov112PokewalkerApp *arg0);
void ov112_021E7CA4(Ov112PokewalkerApp *arg0, enum GFBgLayer arg1, s32 arg2);

void ov112_021E7B18(Ov112PokewalkerApp *param_1) {
    NARC *narc;
    u32 uVar1;

    narc = NARC_New(NARC_a_2_4_7, HEAP_ID_154);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, param_1->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, HEAP_ID_154);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, param_1->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, HEAP_ID_154);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_154);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_154);
    uVar1 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_1, 0x3e2, 0xd, (u8)uVar1, HEAP_ID_154);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_154);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_154);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, param_1->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_154);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, param_1->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_154);
    uVar1 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x3e2, 0xd, (u8)uVar1, HEAP_ID_154);
    NARC_Delete(narc);
    return;
}

void ov112_021E7C18(Ov112PokewalkerApp *arg0) {
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x112, HEAP_ID_154);
    arg0->msgData2 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xDE, HEAP_ID_154);
    arg0->msgFormat = MessageFormat_New_Custom(0x10U, 0x20U, HEAP_ID_154);
    arg0->unk_1E454 = String_New(0x190U, HEAP_ID_154);
}

void ov112_021E7C6C(Ov112PokewalkerApp *arg0) {
    String_Delete(arg0->unk_1E454);
    DestroyMsgData(arg0->msgData2);
    DestroyMsgData(arg0->msgData);
    MessageFormat_Delete(arg0->msgFormat);
}

void ov112_021E7CA4(Ov112PokewalkerApp *arg0, enum GFBgLayer layer, s32 memberNo) {
    GfGfxLoader_LoadScrnData(NARC_a_2_4_7, memberNo, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_154);
}
