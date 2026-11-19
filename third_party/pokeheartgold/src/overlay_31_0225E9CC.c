#include "global.h"

#include "msgdata.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "options.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

String *ov31_0225E9CC(MartData *param_1, MsgData *param_2, u32 param_3, s32 param_4);
String *ov31_0225E95C(MartData *, MsgData *);

String *ov31_0225E9CC(MartData *param_1, MsgData *param_2, u32 param_3, s32 param_4) {
    u8 cVar1;

    cVar1 = param_1->martType;
    if (cVar1 == 0) {
        param_4 = 0x10;
    } else if (cVar1 == 1) {
        param_4 = 0x16;
    } else if (cVar1 == 2) {
        param_4 = 0x18;
    } else if (cVar1 == 3) {
        param_4 = 0x19;
    } else if (cVar1 == 4) {
        param_4 = 0x1a;
    }
    return NewString_ReadMsgData(param_2, param_4);
}

void ov31_0225EA08(MartSubScreen *param_1) {
    u8 bVar1;
    u8 uVar2;
    String *src;
    u32 uVar3;

    src = ov31_0225E95C(param_1->martData, param_1->msgData);
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
