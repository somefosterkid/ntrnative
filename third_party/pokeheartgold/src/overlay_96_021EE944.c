#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_96_text_box.h"

void ov96_021EE944(Ov96TextBox *arg0);

void ov96_021EE944(Ov96TextBox *arg0) {
    s32 printerId;

    printerId = arg0->printerId;
    if (printerId != -1) {
        RemoveTextPrinter((u8)printerId);
    }
    if (arg0->window.bgConfig != NULL) {
        ClearFrameAndWindow2(&arg0->window, 0);
        ClearWindowTilemapAndCopyToVram(&arg0->window);
        RemoveWindow(&arg0->window);
    }
}

MessageFormat *ov96_021EE97C(Ov96TextBox *arg0);

MessageFormat *ov96_021EE97C(Ov96TextBox *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->msgFormat == NULL) {
        GF_AssertFail();
    }
    return arg0->msgFormat;
}

void ov96_021EE994(Ov96TextBox *arg0);

void ov96_021EE994(Ov96TextBox *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->waitingIcon != NULL) {
        GF_AssertFail();
    }
    arg0->waitingIcon = WaitingIcon_New(&arg0->window, 0x3D2U);
}

void ov96_021EE9BC(Ov96TextBox *arg0);

void ov96_021EE9BC(Ov96TextBox *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->waitingIcon == NULL) {
        GF_AssertFail();
    }
    sub_0200F450(arg0->waitingIcon);
}

void ov96_021EE9D8(BgConfig *arg0, enum GFBgLayer arg1, enum HeapID arg2);

void ov96_021EE9D8(BgConfig *arg0, enum GFBgLayer layer, enum HeapID heapID) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, heapID);
    LoadUserFrameGfx2(arg0, layer, 0x3D2U, 0xDU, 0U, heapID);
}

u8 ov96_021EEA08(BgConfig *param_1, Window *param_2, String **param_3, u32 param_4, MessageFormat *param_5, MsgData *param_6, u32 param_7, u8 param_8, enum HeapID param_9);

u8 ov96_021EEA08(BgConfig *param_1, Window *param_2, String **param_3, u32 param_4, MessageFormat *param_5, MsgData *param_6, u32 param_7, u8 param_8, enum HeapID param_9) {
    u8 uVar1;
    String *pSVar2;

    if (param_2->bgConfig == NULL) {
        AddWindowParameterized(param_1, param_2, param_8, 2, 0x13, 0x1b, 4, 0xf, 1);
    }
    FillWindowPixelBuffer(param_2, 0xf);
    pSVar2 = ReadMsgData_ExpandPlaceholders(param_5, param_6, param_7, param_9);
    *param_3 = pSVar2;
    uVar1 = AddTextPrinterParameterizedWithColor(param_2, FONT_MESSAGE, *param_3, 0, 0, param_4, 0x1020f, (PrinterCallback_t)0x0);
    DrawFrameAndWindow2(param_2, 0, 0x3d2, 0xd);
    return uVar1;
}

u32 ov96_021EEA80(Ov96TextBox *arg0);

u32 ov96_021EEA80(Ov96TextBox *arg0) {
    return arg0->busy;
}
