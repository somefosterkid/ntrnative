#include "global.h"
#include "bg_window.h"
#include "heap.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "render_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_text.h"
#include "overlay_59_app_0223b68c.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223B554(Ov59JuiceFeedApp *arg0);

void ov59_0223B554(Ov59JuiceFeedApp *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
    FreeBgTilemapBuffer(arg0->bgConfig, 7U);
    Heap_Free(arg0->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

void ov59_0223B590(Ov59JuiceFeedApp *arg0);

void ov59_0223B590(Ov59JuiceFeedApp *arg0) {
    NARC *temp_r0;

    temp_r0 = NARC_New(NARC_a_2_4_2, arg0->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 7, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 9, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 6, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 8, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->heapId);
    arg0->narc = temp_r0;
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_SUB_0_F, 1U, 0xDU, (u8) (s32) *&arg0->frame, arg0->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, arg0->heapId);
    G2_SetBlendAlpha(1, 0xE, 6, 0x1A);
}

void ov59_0223B66C(Ov59JuiceFeedApp *arg0);

void ov59_0223B66C(Ov59JuiceFeedApp *arg0) {
    NARC_Delete(arg0->narc);
    G2_SetBlendAlpha(0, 0, 0x1F, 0);
}

void ov59_0223B68C(Ov59JuiceFeedApp *arg0);

void ov59_0223B68C(Ov59JuiceFeedApp *arg0) {
    s32 var_r4;

    FontID_Alloc(FONT_UNK_4, arg0->heapId);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x16, arg0->heapId);
    arg0->msgFormat = MessageFormat_New_Custom(0x10U, 0x10U, arg0->heapId);
    arg0->unk_64 = String_New(0x100U, arg0->heapId);
    arg0->unk_68 = String_New(0x100U, arg0->heapId);
    arg0->unk_6C = NewString_ReadMsgData(arg0->msgData, 4);
    var_r4 = 0;
    do {
        ((Ov59App_0223B68C *)arg0)->unk_70[var_r4] = NewString_ReadMsgData(arg0->msgData, var_r4 + 0xA);
        var_r4 += 1;
    } while (var_r4 < 2);
}

void ov59_0223B6FC(Ov59JuiceFeedApp *arg0);

void ov59_0223B6FC(Ov59JuiceFeedApp *arg0) {
    String_Delete(arg0->genderStrings[1]);
    String_Delete(arg0->genderStrings[0]);
    String_Delete(arg0->unk_6C);
    String_Delete(arg0->unk_68);
    String_Delete(arg0->unk_64);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    TextFlags_SetCanABSpeedUpPrint(0);
    FontID_Release(FONT_UNK_4);
}
