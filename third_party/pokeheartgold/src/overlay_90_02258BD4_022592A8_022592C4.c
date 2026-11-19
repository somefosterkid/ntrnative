#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "message_format.h"

struct MsgData;

struct MessageFormat;


void ReadMsgDataIntoString(const struct MsgData *loader, u32 entryID, String *string);


typedef u32 TextColor;

void ov90_022592C4(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4);

void ov90_022592C4(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4) {
    u32 v0;
    s32 v1;

    ReadMsgDataIntoString(param0->unk_00, param1, param0->unk_0C);
    StringExpandPlaceholders(param0->unk_04, param0->unk_08, param0->unk_0C);

    v0 = FontID_String_GetWidth(FONT_SYSTEM, param0->unk_08, 0);
    v1 = param3 - v0;

    if (v1 < 0) {
        v1 = 0;
    }

    AddTextPrinterParameterizedWithColor(param2, FONT_SYSTEM, param0->unk_08, v1, param4, 0xFF, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
}
