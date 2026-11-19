#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "render_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_113.h"

s32 ov113_021E64DC(void *, NARC *, s32, enum HeapID);

void ov113_021E650C(UnownReportData *arg0);

void ov113_021E6680(UnownReportData *arg0);

void ov113_021E66E4(UnownReportData *arg0);

extern u32 ov113_021E6C90;
void ov113_021E671C(UnownReportData *param_1);
void ov113_021E6754(UnownReportData *param_1);

void ov113_021E650C(UnownReportData *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_3_9, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, arg0->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, arg0->heapID);
    ov113_021E64DC(&arg0->screens[3], narc, 0xA, arg0->heapID);
    ov113_021E64DC(&arg0->screens[4], narc, 0xC, arg0->heapID);
    ov113_021E64DC(&arg0->screens[5], narc, 0xE, arg0->heapID);
    ov113_021E64DC(arg0->screens, narc, 0xB, arg0->heapID);
    ov113_021E64DC(&arg0->screens[1], narc, 0xD, arg0->heapID);
    ov113_021E64DC(&arg0->screens[2], narc, 0xF, arg0->heapID);
    NARC_Delete(narc);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_SUB_0_F, 1U, 0xDU, (u8)(s32) * &arg0->frame, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, arg0->heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, arg0->heapID);
}

void ov113_021E663C(UnownReportData *arg0);

void ov113_021E663C(UnownReportData *arg0) {
    Heap_Free(arg0->screens[2].buf);
    Heap_Free(arg0->screens[1].buf);
    Heap_Free(arg0->screens[0].buf);
    Heap_Free(arg0->screens[5].buf);
    Heap_Free(arg0->screens[4].buf);
    Heap_Free(arg0->screens[3].buf);
}

void ov113_021E6680(UnownReportData *arg0) {
    FontID_Alloc(FONT_UNK_4, arg0->heapID);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 1, arg0->heapID);
    arg0->msgFormat = MessageFormat_New_Custom(0x10U, 0x10U, arg0->heapID);
    arg0->strBuf1 = String_New(0x200U, arg0->heapID);
    arg0->strBuf2 = String_New(0x200U, arg0->heapID);
    arg0->titleString = NewString_ReadMsgData(arg0->msgData, 0);
    arg0->pageFormatString = NewString_ReadMsgData(arg0->msgData, 5);
    arg0->playerName = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(arg0->args->saveData), arg0->heapID);
}

void ov113_021E66E4(UnownReportData *arg0) {
    String_Delete(arg0->playerName);
    String_Delete(arg0->pageFormatString);
    String_Delete(arg0->titleString);
    String_Delete(arg0->strBuf2);
    String_Delete(arg0->strBuf1);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    FontID_Release(FONT_UNK_4);
}

void ov113_021E671C(UnownReportData *param_1) {
    YesNoPrompt *pYVar1;
    WindowTemplate *template;
    Window *window;
    s32 iVar2;

    template = (WindowTemplate *)&ov113_021E6C90;
    iVar2 = 0;
    window = param_1->windows;
    do {
        AddWindow(param_1->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        iVar2 = iVar2 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar2 < 4);
    pYVar1 = YesNoPrompt_Create(param_1->heapID);
    param_1->yesNoPrompt = pYVar1;
    return;
}

void ov113_021E6754(UnownReportData *param_1) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = param_1->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 4);
    YesNoPrompt_Destroy(param_1->yesNoPrompt);
    return;
}
