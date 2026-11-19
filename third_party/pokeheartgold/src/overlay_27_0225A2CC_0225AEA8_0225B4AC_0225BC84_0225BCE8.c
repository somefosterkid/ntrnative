#include "global.h"

#include "overlay_27_ov27task.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"

void ov27_0225BCE8(struct Ov27Task *arg0);

void ov27_0225BCE8(struct Ov27Task *task) {
    s32 i;
    s32 margin;

    for (i = 0; i < 7; i++) {
        if (task->unk_470[i].unk_4 != NULL) {
            margin = 72 - FontID_String_GetWidth(FONT_SYSTEM, task->unk_470[i].unk_4, 0);
            AddTextPrinterParameterizedWithColor(&task->unk_3F0[i], FONT_SYSTEM, task->unk_470[i].unk_4, margin / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 0), NULL);
        }
    }
}
