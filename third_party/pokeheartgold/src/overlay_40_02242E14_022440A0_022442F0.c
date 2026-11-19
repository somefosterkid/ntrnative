#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_40.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

MessageFormat *ov40_0222DAB0(s32);

s32 ov40_02230DCC(void *, String *);

String *sub_020315B8(u16 *, s32);

void ov40_022442F0(void *arg0, s32 strno, s32 arg2) {
    int new_var;
    String *string;
    s32 v0;
    void *new_var3;
    MessageFormat *messageFormat;
    String *v1;
    u8 *new_var2;
    String *v3;
    String *v4;
    void *v2;
    new_var3 = arg0 + 0x8A4;
    new_var2 = (u8 *)arg0;
    v0 = strno;
    if (strno == 0x11A) {
        v2 = *((void **)(((u8 *)(arg0 + (arg2 * 4))) + 0x88C));
        messageFormat = ov40_0222DAB0(0x6D);
        v4 = String_New(0xFFU, HEAP_ID_109);
        v1 = sub_020315B8(v2, 0x6D);
        string = v1;
        ov40_02230DCC(arg0, string);
        new_var = 2;
        v3 = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, v0);
        BufferString(messageFormat, 0U, string, 0, 1, new_var);
        StringExpandPlaceholders(messageFormat, v4, v3);
        String_Delete(v1);
        String_Delete(v3);
        MessageFormat_Delete(messageFormat);
    } else {
        v4 = NewString_ReadMsgData(((UnkStruct_0208C06C *)new_var2)->unk_14.unk_34, strno);
    }
    FillWindowPixelBuffer(new_var3, 0xCCU);
    AddTextPrinterParameterizedWithColor(arg0 + 0x8A4, FONT_SYSTEM, v4, 0U, 0U, 0xFFU, 0xF0D0CU, 0L);
    ScheduleWindowCopyToVram(arg0 + 0x8A4);
    String_Delete(v4);
}
