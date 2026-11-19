#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "overlay_01_021EDAFC.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

Window *FieldSystem_ShowMoneyBox(FieldSystem *param_1, u8 param_2, u8 param_3);

Window *FieldSystem_ShowMoneyBox(FieldSystem *param_1, u8 param_2, u8 param_3) {
    Window *window;
    MsgData *msgData;
    String *text;

    window = AllocWindows(HEAP_ID_FIELD1, 1);
    AddWindowParameterized(param_1->bgConfig, window, 3, param_2, param_3, 0xa, 4, 0xd, 1);
    LoadUserFrameGfx1(param_1->bgConfig, GF_BG_LYR_MAIN_3, 0x3d9, 0xb, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, 1, 0x3d9, 0xb);
    FillWindowPixelBuffer(window, 0xf);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1b3, HEAP_ID_FIELD1);
    text = NewString_ReadMsgData(msgData, 0x1e);
    AddTextPrinterParameterized(window, FONT_SYSTEM, text, 0, 0, 0xff, (PrinterCallback_t)0x0);
    DestroyMsgData(msgData);
    String_Delete(text);
    MoneyBoxSys_Update(param_1, window);
    return window;
}

void MoneyBoxSys_Delete(Window *window) {
    ClearWindowPixelBuffer(window, 0);
    WindowArray_Delete(window, 1);
}
