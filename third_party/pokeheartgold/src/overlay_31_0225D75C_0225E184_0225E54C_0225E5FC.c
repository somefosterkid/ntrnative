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
#include "string_util.h"
#include "text.h"





void ov31_0225E5FC(MartSubScreen *param_1) {
    u8 bVar1;
    String *src;
    u32 uVar2;
    MartData *iVar3;
    FillWindowPixelBuffer(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 0xf);
    iVar3 = param_1->martData;
    if (((u8)(iVar3->martType + 0xFD)) <= (2 - 1)) {
        ov31_0225E51C(iVar3->martType, param_1->messageFormat, iVar3->item, 0);
        src = NewString_ReadMsgData(param_1->msgData, 0x2e);
    } else {
        BufferIntegerAsString(param_1->messageFormat, 0, (s32)iVar3->quantity, 2, PRINTING_MODE_LEFT_ALIGN, 1);
        BufferIntegerAsString(param_1->messageFormat, 1, (param_1->martData->cost) * ((s32)(param_1->martData->quantity)), 6, PRINTING_MODE_LEFT_ALIGN, 1);
        src = NewString_ReadMsgData(param_1->msgData, 0xe);
    }
    StringExpandPlaceholders(param_1->messageFormat, param_1->string, src);
    String_Delete(src);
    uVar2 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(param_1->bgConfig, GF_BG_LYR_MAIN_CNT, 0x1b5, 5, (u8)uVar2, HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], 1, 0x1b5, 5);
    bVar1 = Options_GetTextFrameDelay(param_1->options);
    bVar1 = AddTextPrinterParameterized(&param_1->windows[MART_SUBSCREEN_WIN_MESSAGE_BOX], FONT_MESSAGE, param_1->string, 0, 0, (u32)bVar1, (PrinterCallback_t)0x0);
    param_1->unk_184 = (u32)bVar1;
    return;
}
