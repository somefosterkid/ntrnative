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


Window *ov01_021EED60(FieldSystem *fieldSystem, u8 arg1, u8 arg2) {
    Window *window;
    MsgData *msgData;
    String *pSVar1;

    window = AllocWindows(HEAP_ID_FIELD1, 1);
    AddWindowParameterized(fieldSystem->bgConfig, window, 3, arg1, arg2, 0x10, 8, 0xd, 1);
    LoadUserFrameGfx1(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3d9, 0xb, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, 1, 0x3d9, 0xb);
    FillWindowPixelBuffer(window, 0xf);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xbf, HEAP_ID_FIELD1);
    pSVar1 = NewString_ReadMsgData(msgData, 0x14a);
    AddTextPrinterParameterized(window, FONT_SYSTEM, pSVar1, 0, 0, 0xff, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    pSVar1 = NewString_ReadMsgData(msgData, 0x14b);
    AddTextPrinterParameterized(window, FONT_SYSTEM, pSVar1, 0, 0x20, 0xff, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    DestroyMsgData(msgData);
    ov01_021EEE44(fieldSystem, window);
    return window;
}

void ov01_021EEE30(Window *window) {
    ClearWindowPixelBuffer(window, 0);
    WindowArray_Delete(window, 1);
}
