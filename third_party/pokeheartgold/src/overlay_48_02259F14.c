#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task.h"
#include "text.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"

struct YesNoPrompt;
void YesNoPrompt_Destroy(YesNoPrompt *menu);

void ov48_02259F14(UnkStruct_ov69_0225DDC8 *param0);
void ov48_02259F48(UnkStruct_ov69_0225DDC8 *param0);

void ov48_02259F14(UnkStruct_ov69_0225DDC8 *param0) {
    if (TextPrinterCheckActive(param0->unk_04) != 0) {
        RemoveTextPrinter(param0->unk_04);
    }

    String_Delete(param0->unk_0C);
    YesNoPrompt_Destroy(param0->unk_20);
    RemoveWindow(&param0->unk_10);
}

void ov48_02259F48(UnkStruct_ov69_0225DDC8 *param0) {
    FillWindowPixelBuffer(&param0->unk_10, 15);
    DrawFrameAndWindow2(&param0->unk_10, 1, 1, 2);
    ScheduleWindowCopyToVram(&param0->unk_10);

    param0->unk_04 = AddTextPrinterParameterized(&param0->unk_10, FONT_MESSAGE, param0->unk_0C, 0, 0, param0->unk_08, 0L);
    param0->unk_00 = 0;
}
