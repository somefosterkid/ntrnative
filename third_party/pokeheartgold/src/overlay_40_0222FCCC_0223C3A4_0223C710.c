#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_40.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222C6C8(void *, s32);

MessageFormat *ov40_0222DAB0(s32);

s32 ov40_02230DCC(void *, String *);

String *sub_020315B8(u16 *, s32);

void ov40_0223C710(void *arg0, s32 strno) {
    String *string;
    void *v0;
    void *new_var;
    MessageFormat *messageFormat;
    String *v2;
    String *v3;
    String *v5;
    void *v1;
    void *v4;
    new_var = arg0 + 0x8A4;
    v4 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    ov40_0222C6C8(arg0, 2);
    GfGfx_EngineATogglePlanes(4U, 1U);
    InitWindow(arg0 + 0x8A4);
    AddWindowParameterized(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, new_var, 2U, 1U, 0x13U, 0x1EU, 4U, 0xEU, 0x12CU);
    if (strno == 0x64) {
        v1 = *((void **)(((u8 *)v4) + 0x194));
        v0 = v1;
        messageFormat = ov40_0222DAB0(0x6D);
        v5 = String_New(0xFFU, HEAP_ID_109);
        v2 = sub_020315B8(v1, 0x6D);
        string = v2;
        ov40_02230DCC(arg0, string);
        v3 = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, strno);
        BufferString(messageFormat, 0U, string, 0, 1, 2);
        StringExpandPlaceholders(messageFormat, v5, v3);
        String_Delete(v2);
        String_Delete(v3);
        MessageFormat_Delete(messageFormat);
    } else {
        v5 = NewString_ReadMsgData(((UnkStruct_0208C06C *)arg0)->unk_14.unk_34, strno);
    }
    FillWindowPixelBuffer(new_var, 0xCCU);
    AddTextPrinterParameterizedWithColor(new_var, FONT_SYSTEM, v5, 0U, 0U, 0xFFU, 0xF0D0CU, 0L);
    ScheduleWindowCopyToVram(new_var);
    String_Delete(v5);
}
