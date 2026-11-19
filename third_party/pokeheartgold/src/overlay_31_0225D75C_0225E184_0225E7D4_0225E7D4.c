#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "item.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_03.h"
#include "overlay_31.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"



void ov31_0225E7D4(MartSubScreen *arg0) {
    String *var_r5;
    u16 temp_r2_2;
    u8 temp_r2;
    MartData *temp_r1;
    MartData *temp_r3;

    temp_r3 = arg0->martData;
    if ((s32)temp_r3->quantity > 1) {
        ov31_0225E4EC(temp_r3->martType, arg0->messageFormat, temp_r3->item, 0);
    } else {
        ov31_0225E4BC(temp_r3->martType, arg0->messageFormat, temp_r3->item, 0);
    }
    temp_r1 = arg0->martData;
    temp_r2 = temp_r1->martType;
    switch (temp_r2) {
    case 0:
        var_r5 = NewString_ReadMsgData(arg0->msgData, 0xF);
        BufferPocketName(arg0->messageFormat, 1U, (u32)GetItemAttr(arg0->martData->item, 5U, HEAP_ID_FIELD2));
        break;
    case 3:
        temp_r2_2 = temp_r1->item;
        if (((u32)temp_r2_2 >= 0x1E5U) && ((u32)temp_r2_2 <= 0x1EBU)) {
            var_r5 = NewString_ReadMsgData(arg0->msgData, 0x31);
        } else {
            var_r5 = NewString_ReadMsgData(arg0->msgData, 0xF);
            BufferPocketName(arg0->messageFormat, 1U, (u32)GetItemAttr(arg0->martData->item, 5U, HEAP_ID_FIELD2));
        }
        break;
    case 4:
        var_r5 = NewString_ReadMsgData(arg0->msgData, 0x31);
        break;
    case 1:
        var_r5 = NewString_ReadMsgData(arg0->msgData, 0x15);
        break;
    default:
        var_r5 = NewString_ReadMsgData(arg0->msgData, 0x17);
        break;
    }
    StringExpandPlaceholders(arg0->messageFormat, arg0->string, var_r5);
    String_Delete(var_r5);
    FillWindowPixelBuffer(&arg0->msgWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->msgWindow, 1, 0x1B5U, 5U);
    arg0->martData->printerID = AddTextPrinterParameterized(&arg0->msgWindow, FONT_MESSAGE, arg0->string, 0U, 0U, (u32)Options_GetTextFrameDelay(arg0->options), (PrinterCallback_t)(ov31_0225E948));
}
