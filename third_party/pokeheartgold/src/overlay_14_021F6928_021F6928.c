#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_14_box_funcs.h"

MsgData *ov14_021F6628(MsgData *);
void ov14_021F6928(MsgData *param_1, s32 param_2, s32 param_3);

void ov14_021F6928(MsgData *param_1, s32 param_2, s32 param_3) {
    MsgData *msgData;
    String *src;
    msgData = ov14_021F6628(param_1);
    switch (param_3) {
    case 0:
        param_3 = 0x24;
        break;

    case 1:
        param_3 = 0x25;
        break;

    case 2:
        param_3 = 0x26;
        break;

    case 3:
        param_3 = 0x27;
        break;

    case 4:
        param_3 = 0x1c;
        break;

    case 5:
        param_3 = 0x19;
        BufferItemName(*((MessageFormat **)((*((u8 **)(&param_1[4].narc_id))) + 0x24)), 0, (u32)(*((u16 *)((*((u8 **)(&param_1[4].narc_id))) + 0x88c8))));
        break;

    case 6:
        param_3 = 0x38;
        break;

    case 7:
        param_3 = 0x39;
        break;

    case 8:
        param_3 = 0x3a;
        break;

    case 9:
        param_3 = 0x3b;
        break;

    case 10:
        param_3 = 0x3c;
    }

    src = NewString_ReadMsgData(msgData, param_3);
    StringExpandPlaceholders(*((MessageFormat **)((*((u8 **)(&param_1[4].narc_id))) + 0x24)), *((String **)((*((u8 **)(&param_1[4].narc_id))) + 0x28)), src);
    String_Delete(src);
    FillWindowPixelBuffer((Window *)((((*((void **)(&param_1[4].narc_id))) + 0x30)) + (param_2 * sizeof(Window))), 0xf);
    AddTextPrinterParameterizedWithColor((Window *)(((*((u8 **)(&param_1[4].narc_id))) + 0x30) + (param_2 * sizeof(Window))), FONT_MESSAGE, *((String **)((*((u8 **)(&param_1[4].narc_id))) + 0x28)), 0, 0, 0xff, 0x1020f, (PrinterCallback_t)0x0);
    DestroyMsgData(msgData);
    return;
}
