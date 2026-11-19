#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_40.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02013534.h"

void *ov40_0222DAB0(u32);

void ov40_02230DCC(void *, String *);

String *sub_020315B8(u16 *, u32);

void ov40_0223B29C(u8 *param_1, u8 *param_2, s32 param_3) {
    String *dest;
    MessageFormat *messageFormat;
    MsgData *new_var2;
    String *src;
    u8 *new_var;
    String *string;
    Window WStack_28;
    new_var2 = *((MsgData **)(param_2 + 0x48));
    if ((*((s32 *)((param_2 + 0x88c) + (param_3 * 4)))) == 0) {
        dest = NewString_ReadMsgData(new_var2, 8);
    } else {
        messageFormat = (MessageFormat *)ov40_0222DAB0(0x6d);
        src = NewString_ReadMsgData(*((MsgData **)(param_2 + 0x48)), 7);
        string = (String *)sub_020315B8(*((void **)(new_var = (param_2 + 0x88c) + (param_3 * 4))), 0x6d);
        ov40_02230DCC(param_2, string);
        dest = String_New(0xff, HEAP_ID_109);
        BufferString(messageFormat, 0, string, 0, 1, 2);
        StringExpandPlaceholders(messageFormat, dest, src);
        String_Delete(src);
        String_Delete(string);
        MessageFormat_Delete(messageFormat);
    }
    InitWindow(&WStack_28);
    AddTextWindowTopLeftCorner(*((BgConfig **)(param_2 + 0x24)), &WStack_28, 0x14, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&WStack_28, FONT_SYSTEM, dest, 0, 0, 0xff, 0xe0d00, (PrinterCallback_t)0x0);
    TextOBJ_CopyFromBGWindow(*((TextOBJ **)(param_1 + 8)), *((UnkStruct_02013910 **)(param_1 + 0xc)), &WStack_28, HEAP_ID_109);
    String_Delete(dest);
    RemoveWindow(&WStack_28);
    return;
}
