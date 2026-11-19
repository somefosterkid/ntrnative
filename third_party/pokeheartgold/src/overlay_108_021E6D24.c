#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "unk_0208805C.h"
#include "overlay_108_safari_area_customizer.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6D24(SafariAreaCustomizer *param_1);

void ov108_021E7ADC(SafariAreaCustomizer *arg0);
void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);
void ov108_021E6D80(SafariAreaCustomizer *arg0);
void ov108_021E6F58(SafariAreaCustomizer *arg0);
void ov108_021E6F74(SafariAreaCustomizer *arg0);

void ov108_021E7014(SafariAreaCustomizer *arg0);

void ov108_021E6D24(SafariAreaCustomizer *param_1) {
    s32 iVar1;

    G2_SetBlendAlpha(0, 0, 0x1f, 0);
    G2S_SetBlendAlpha(0, 0, 0x1f, 0);
    iVar1 = 0;
    do {
        FreeBgTilemapBuffer(param_1->bgConfig, (u8)iVar1);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 8);
    Heap_Free(param_1->bgConfig);
    reg_GX_POWCNT = reg_GX_POWCNT | 0x8000;
    return;
}

void ov108_021E6D80(SafariAreaCustomizer *arg0) {
    NARC *narc;
    s32 v0;

    narc = NARC_New(NARC_a_1_6_6, arg0->heapID);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 6, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_CHAR, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 6, GF_BG_LYR_SUB_2_F, GF_BG_GFX_TYPE_CHAR, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 1, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_CHAR, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 5, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_PLTT, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 0, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_PLTT, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 0xA, GF_BG_LYR_SUB_1_F, GF_BG_GFX_TYPE_SCRN, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 7, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_SCRN, 0U, 0U);
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, 4, GF_BG_LYR_SUB_2, GF_BG_GFX_TYPE_SCRN, 0U, 0U);
    if (arg0->unk_184E3 == 0) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    BgConfig_LoadAssetFromOpenNarc(arg0->bgConfig, arg0->heapID, narc, NARC_a_1_6_6, v0 + 2, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_SCRN, 0U, 0U);
    arg0->unk_4D0 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, 0, &arg0->unk_4D8, arg0->heapID);
    arg0->unk_4CC = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, 0, &arg0->unk_4D4, arg0->heapID);
    NARC_Delete(narc);
    ov108_021E7ADC(arg0);
    ov108_021E7BB4(arg0, 0xFF, 0);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 0U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 3U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 7U);
}

void ov108_021E6F58(SafariAreaCustomizer *arg0) {
    Heap_Free(arg0->unk_4D0);
    Heap_Free(arg0->unk_4CC);
}

void ov108_021E6F74(SafariAreaCustomizer *arg0) {
    s32 v0;

    FontID_Alloc(FONT_UNK_4, arg0->heapID);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1AD, arg0->heapID);
    arg0->msgFormat = MessageFormat_New_Custom(1U, 0x11U, arg0->heapID);
    arg0->string = String_New(0x51U, arg0->heapID);
    arg0->unk_334 = NewString_ReadMsgData(arg0->msgData, 9);
    arg0->unk_338 = NewString_ReadMsgData(arg0->msgData, 0xF);
    v0 = 0;
    do {
        arg0->unk_310[v0] = NewString_ReadMsgData(arg0->msgData, v0 + 4);
        v0 += 1;
    } while (v0 < 5);
    v0 = 0;
    do {
        arg0->unk_324[v0] = NewString_ReadMsgData(arg0->msgData, v0);
        v0 += 1;
    } while (v0 < 4);
}

void ov108_021E7014(SafariAreaCustomizer *arg0) {
    s32 v0;

    v0 = 0;
    do {
        String_Delete(arg0->unk_324[v0]);
        v0 += 1;
    } while (v0 < 4);
    v0 = 0;
    do {
        String_Delete(arg0->unk_310[v0]);
        v0 += 1;
    } while (v0 < 5);
    String_Delete(arg0->unk_338);
    String_Delete(arg0->unk_334);
    String_Delete(arg0->string);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    FontID_Release(FONT_UNK_4);
}
