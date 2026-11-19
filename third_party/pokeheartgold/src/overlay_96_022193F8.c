#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

void ov96_022193F8(PokeathlonCourseData *param_1, s32 param_2);

void ov96_022193F8(PokeathlonCourseData *param_1, s32 param_2) {
    enum HeapID *pHVar1;
    String *string;
    MessageFormat *messageFormat;
    MsgData *msgData;

    pHVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    messageFormat = (MessageFormat *)pHVar1[4];
    msgData = (MsgData *)pHVar1[5];
    FillWindowPixelBuffer((Window *)(pHVar1 + 8), 0);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa3, *pHVar1);
    AddTextPrinterParameterizedWithColor((Window *)(pHVar1 + 8), FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram((Window *)(pHVar1 + 8));
    return;
}
