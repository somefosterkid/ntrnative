#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "sprite.h"
#include "overlay_72_wfc_connect.h"

void ov72_0223B1C8(NintendoWfcYesNoMenu *arg0);

void ov72_0223B2C0(NintendoWfcYesNoMenu *param_1);

s32 ov72_0223B2FC(NintendoWfcYesNoMenu *, s32);
void ov72_0223B2E4(NintendoWfcYesNoMenu *arg0);

void ov72_0223B1C8(NintendoWfcYesNoMenu *arg0) {
    MsgData *msgData;
    String *string;
    String *v0;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xBF, arg0->heapID);
    AddWindowParameterized(arg0->bgConfig, &arg0->yesWindow, arg0->textBgLayer, 0xCU, 8U, 8U, 2U, (u8)((s32)(arg0->fontPltt)), (u16)((s32)(arg0->baseTile)));
    FillWindowPixelBuffer(&arg0->yesWindow, 0U);
    AddWindowParameterized(arg0->bgConfig, &arg0->noWindow, arg0->textBgLayer, 0xCU, 0xEU, 8U, 2U, (u8)(((s32)(arg0->fontPltt)) & 0xFFu), (u16)((u16)(arg0->baseTile + 0x10)));
    FillWindowPixelBuffer(&arg0->noWindow, 0U);
    string = NewString_ReadMsgData(msgData, 0x2E);
    v0 = NewString_ReadMsgData(msgData, 0x2F);
    AddTextPrinterParameterizedWithColor(&arg0->yesWindow, FONT_UNK_4, string, 0U, 0U, 0U, 0xF0100U, 0L);
    AddTextPrinterParameterizedWithColor(&arg0->noWindow, FONT_UNK_4, v0, 0U, 0U, 0U, 0xF0100U, 0L);
    CopyWindowToVram(&arg0->yesWindow);
    CopyWindowToVram(&arg0->noWindow);
    String_Delete(v0);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void ov72_0223B2C0(NintendoWfcYesNoMenu *param_1) {
    ClearWindowTilemapAndCopyToVram(&param_1->noWindow);
    ClearWindowTilemapAndCopyToVram(&param_1->yesWindow);
    RemoveWindow(&param_1->noWindow);
    RemoveWindow(&param_1->yesWindow);
    return;
}

void ov72_0223B2E4(NintendoWfcYesNoMenu *arg0) {
    Sprite_SetDrawFlag(arg0->cursorSprite, 1);
    ov72_0223B2FC(arg0, arg0->initialChoice);
}
