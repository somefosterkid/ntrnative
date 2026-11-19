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
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_0220E890(s32 *param_1, s32 param_2);

void ov96_0220E960(VecFx32 *param_1, s32 param_2, s32 param_3);

void ov96_0220E960(VecFx32 *param_1, s32 param_2, s32 param_3) {
    ov96_0220E890(&param_1->x, param_2);
    ov96_0220E890(&param_1->y, param_3);
    return;
}

void ov96_0220E974(VecFx32 *arg0, s32 arg1);

void ov96_0220E974(VecFx32 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = arg0->x;
    v2 = 0 - arg1;
    if (v0 > 0) {
        if (v0 > arg1) {
            arg0->x = arg1;
        }
    } else if (v0 < v2) {
        arg0->x = v2;
    }
    v1 = arg0->y;
    if (v1 > 0) {
        if (v1 > arg1) {
            arg0->y = arg1;
        }
    } else if (v1 < v2) {
        arg0->y = v2;
    }
}

void ov96_0220E9A0(PokeathlonCourseData *param_1, s32 param_2);

void ov96_0220E9A0(PokeathlonCourseData *param_1, s32 param_2) {
    UnkStruct_ov96_0220E3F0 *pHVar1;
    String *string;
    MessageFormat *messageFormat;
    MsgData *msgData;

    pHVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    messageFormat = pHVar1->msgFormat;
    msgData = pHVar1->msgData;
    FillWindowPixelBuffer(&pHVar1->window, 0);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0xa0, pHVar1->heapId);
    AddTextPrinterParameterizedWithColor(&pHVar1->window, FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&pHVar1->window);
    return;
}
