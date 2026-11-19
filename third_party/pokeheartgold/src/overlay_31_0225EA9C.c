#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "options.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "constants/heap.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"

String *ov31_0225E9CC(MartData *, MsgData *);

void ov31_0225EA9C(MartSubScreen *arg0) {
    String *string;
    string = ov31_0225E9CC(arg0->martData, arg0->msgData);
    StringExpandPlaceholders(arg0->messageFormat, arg0->string, string);
    String_Delete(string);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    LoadUserFrameGfx2(arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0x1B5U, 5U, (u8)((u8)Options_GetFrame(arg0->options)), HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&arg0->msgWindow, 1, 0x1B5U, 5U);
    arg0->martData->printerID = AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->string, 0U, 0U, (u32)Options_GetTextFrameDelay(arg0->options), 0L);
}

void ov31_0225EB30(MartSubScreen *param_1) {
    u8 bVar1;
    u8 uVar2;
    String *src;
    u32 uVar3;

    src = NewString_ReadMsgData(param_1->msgData, 0x10);
    StringExpandPlaceholders(param_1->messageFormat, param_1->string, src);
    String_Delete(src);
    FillWindowPixelBuffer(&param_1->msgWindow, 0xf);
    uVar3 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x1b5, 5, (u8)uVar3, HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&param_1->msgWindow, 1, 0x1b5, 5);
    bVar1 = Options_GetTextFrameDelay(param_1->options);
    uVar2 = AddTextPrinterParameterized(&param_1->msgWindow, FONT_MESSAGE, param_1->string, 0, 0, (u32)bVar1, (PrinterCallback_t)0x0);
    param_1->martData->printerID = uVar2;
    return;
}

void ov31_0225EBC4(MartSubScreen *param_1) {
    u8 bVar1;
    u8 uVar2;
    String *src;
    u32 uVar3;

    src = NewString_ReadMsgData(param_1->msgData, 0x14);
    StringExpandPlaceholders(param_1->messageFormat, param_1->string, src);
    String_Delete(src);
    FillWindowPixelBuffer(&param_1->msgWindow, 0xf);
    uVar3 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x1b5, 5, (u8)uVar3, HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&param_1->msgWindow, 1, 0x1b5, 5);
    bVar1 = Options_GetTextFrameDelay(param_1->options);
    uVar2 = AddTextPrinterParameterized(&param_1->msgWindow, FONT_MESSAGE, param_1->string, 0, 0, (u32)bVar1, (PrinterCallback_t)0x0);
    param_1->martData->printerID = uVar2;
    return;
}

void ov31_0225EC58(MartSubScreen *arg0) {
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM], 4U, 0xCU, 7U, 0xBU, 4U, 0xCU, 0xADU);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_UNK_7], 4U, 0x10U, 0xEU, 2U, 3U, 0xCU, 0xD9U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_UNK_8], 4U, 0x14U, 0xEU, 2U, 3U, 0xCU, 0xDFU);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_UNK_9], 4U, 0xEU, 0x15U, 7U, 2U, 0xCU, 0xE5U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_OWNED], 4U, 1U, 0xDU, 8U, 5U, 0xBU, 0xF3U);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_TOTAL], 4U, 0x17U, 0xEU, 8U, 3U, 0xCU, 0x11BU);
    AddWindowParameterized(arg0->bgConfig, &arg0->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 4U, 0xCU, 1U, 0x11U, 4U, 0xCU, 0x133U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_SELECTED_ITEM], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_7], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_8], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_UNK_9], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_OWNED], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_TOTAL], 0U);
    FillWindowPixelBuffer(&arg0->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 0xFU);
}
