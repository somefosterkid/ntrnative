#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "text.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"

void ov90_02259314(void *arg0, s32 arg1, Window *arg2, String *arg3, u32 arg4);

void ov90_02259314(void *arg0, s32 strno, Window *window, String *arg3, u32 textSpeed) {
    ReadMsgDataIntoString(*(MsgData **)arg0, strno, ((UnkStruct_ov114_0225D084 *)arg0)->unk_0C);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov114_0225D084 *)arg0)->unk_04), arg3, ((UnkStruct_ov114_0225D084 *)arg0)->unk_0C);
    AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, arg3, 0U, 0U, textSpeed, 0x1020FU, NULL);
}
