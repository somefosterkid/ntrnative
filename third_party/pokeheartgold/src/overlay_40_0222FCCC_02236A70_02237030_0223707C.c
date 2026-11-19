#include "global.h"
struct UnkStruct_0208C06C;

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

MessageFormat *ov40_0222DAB0(s32);

s32 ov40_02230DCC(void *, String *);

void ov40_0223707C(struct UnkStruct_0208C06C *arg0);

void ov40_0223707C(UnkStruct_0208C06C *arg0) {
    MessageFormat *messageFormat;
    String *src;
    MessageFormat *v0;
    void *new_var;
    String *string;
    String *v2;
    String *v3;
    void *v1;
    v1 = arg0->unk_860;
    string = NewString_ReadMsgData(arg0->unk_14.unk_34, (*((s32 *)(((u8 *)v1) + 0x380))) + 0x1C);
    src = string;
    v2 = String_New(0xFFU, HEAP_ID_109);
    v3 = String_New(0xFFU, HEAP_ID_109);
    v0 = ov40_0222DAB0(0x6D);
    new_var = v1 + 0x1D4;
    messageFormat = v0;
    CopyU16ArrayToString(v2, *((u16 **)(((u8 *)(v1 + ((arg0->unk_14.unk_48C.unk_38.unk_00) * 4))) + 0x38C)));
    ov40_02230DCC(arg0, v2);
    BufferString(v0, 0U, v2, 0, 1, 2);
    StringExpandPlaceholders(v0, v3, src);
    FillWindowPixelBuffer(new_var, 0xCCU);
    AddTextPrinterParameterizedWithColor(new_var, FONT_SYSTEM, v3, 0U, 0U, 0xFFU, 0xF0D00U, 0L);
    ScheduleWindowCopyToVram(v1 + 0x1D4);
    String_Delete(string);
    String_Delete(v2);
    String_Delete(v3);
    MessageFormat_Delete(v0);
}
