#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "render_window.h"
#include "text.h"
#include "overlay_46_unk_struct_ov67_0225d210.h"

void ov46_02259374(UnkStruct_ov67_0225D210 *arg0, s32 arg1);

void ov46_02259374(UnkStruct_ov67_0225D210 *arg0, s32 arg1) {
    if (TextPrinterCheckActive((u8)arg0->unk_2C) != 0) {
        RemoveTextPrinter((u8)arg0->unk_2C);
    }
    FillWindowPixelBuffer(&arg0->unk_08, 0xFU);
    ReadMsgDataIntoString(arg0->unk_04, arg1, arg0->unk_1C);
    StringExpandPlaceholders((MessageFormat *)arg0->unk_00, arg0->unk_18, arg0->unk_1C);
    AddTextPrinterParameterized(&arg0->unk_08, FONT_MESSAGE, arg0->unk_18, 0U, 0U, 0xFFU, NULL);
    if (arg0->unk_20 == 0) {
        DrawFrameAndWindow1(&arg0->unk_08, 1, 0x1FU, 0xCU);
    } else {
        DrawFrameAndWindow2(&arg0->unk_08, 1, 1U, 0xBU);
    }
    ScheduleWindowCopyToVram(&arg0->unk_08);
}
