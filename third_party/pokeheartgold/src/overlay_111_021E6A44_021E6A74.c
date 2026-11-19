#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_111.h"
#include "text.h"

u8 ov111_021E6A74(BgConfig *param_1, Window *param_2, String **param_3, MessageFormat *param_4, MsgData *param_5, u32 param_6, u8 param_7, enum HeapID param_8, Options *param_9) {
    u8 bVar1;
    u8 uVar2;
    u32 uVar3;
    String *pSVar4;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_9 == NULL) {
        GF_AssertFail();
    }
    if (param_2->bgConfig == NULL) {
        AddWindowParameterized(param_1, param_2, param_7, 2, 0x13, 0x1b, 4, 0xf, 1);
        bVar1 = GetWindowBgId(param_2);
        uVar3 = Options_GetFrame(param_9);
        LoadUserFrameGfx2(param_1, (GFBgLayer)((u32)bVar1), 0x3d2, 0xd, (u8)uVar3, param_8);
    }
    bVar1 = Options_GetTextFrameDelay(param_9);
    FillWindowPixelBuffer(param_2, 0xf);
    pSVar4 = ReadMsgData_ExpandPlaceholders(param_4, param_5, param_6, param_8);
    *param_3 = pSVar4;
    uVar2 = AddTextPrinterParameterizedWithColor(param_2, FONT_MESSAGE, *param_3, 0, 0, (u32)bVar1, 0x1020f, (PrinterCallback_t)0x0);
    DrawFrameAndWindow2(param_2, 0, 0x3d2, 0xd);
    return uVar2;
}

void ov111_021E6B30(BgConfig *param_1, Window *param_2, MessageFormat *param_3, MsgData *param_4, u32 param_5, u8 param_6, u8 param_7, u8 param_8, u32 param_9, enum HeapID param_10) {
    String *string;

    FillWindowPixelBuffer(param_2, 0);
    string = ReadMsgData_ExpandPlaceholders(param_3, param_4, param_5, param_10);
    AddTextPrinterParameterizedWithColor(param_2, FONT_SYSTEM, string, (u32)param_7, (u32)param_8, 0, param_9, (PrinterCallback_t)0x0);
    String_Delete(string);
    return;
}
