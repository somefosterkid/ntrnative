#include "global.h"
struct EvolutionData;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "evolution_data.h"

void sub_02075804(struct EvolutionData *param_1, Window *param_2, struct MsgData *param_3, u32 param_4);

void sub_02075804(EvolutionData *param_1, Window *param_2, struct MsgData *param_3, u32 param_4) {
    String *string;

    string = ReadMsgData_ExpandPlaceholders(param_1->strTemplate, param_1->msgLoader, param_4, param_1->heapID);
    FillWindowPixelBuffer(param_2, 0);
    AddTextPrinterParameterizedWithColor(param_2, FONT_UNK_4, string, 0, 0, 0, 0x10200, (PrinterCallback_t)0x0);
    CopyWindowToVram(param_2);
    String_Delete(string);
    return;
}
