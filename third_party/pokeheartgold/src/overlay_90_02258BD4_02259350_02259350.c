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

void ov90_02259350(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4, TextColor param5);

void ov90_02259350(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4, TextColor param5) {
    ReadMsgDataIntoString(param0->unk_00, param1, param0->unk_0C);
    StringExpandPlaceholders(param0->unk_04, param0->unk_08, param0->unk_0C);
    AddTextPrinterParameterizedWithColor(param2, FONT_SYSTEM, param0->unk_08, param3, param4, 0xFF, param5, 0L);
}
