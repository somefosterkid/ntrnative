#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"



void ov31_0225E54C(MartSubScreen *param_1) {
    u8 bVar1;
    u8 uVar2;
    String *src;
    u32 uVar3;

    FillWindowPixelBuffer(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 0xf);
    ov31_0225E4BC(param_1->martData->martType, param_1->messageFormat, param_1->martData->item, 0);
    src = NewString_ReadMsgData(param_1->msgData, 0xc);
    StringExpandPlaceholders(param_1->messageFormat, param_1->string, src);
    String_Delete(src);
    uVar3 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x1b5, 5, (u8)uVar3, HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 1, 0x1b5, 5);
    bVar1 = Options_GetTextFrameDelay(param_1->options);
    uVar2 = AddTextPrinterParameterized(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], FONT_MESSAGE, param_1->string, 0, 0, (u32)bVar1, (PrinterCallback_t)0x0);
    param_1->martData->printerID = uVar2;
    return;
}
