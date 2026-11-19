#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_34.h"

void ov34_0225DC18(Ov34Work *work, int arg1, Ov34ChatEntry *entry) {
    String *string;

    work->rows[arg1].unk30 = entry->gender;
    CopyToBgTilemapRect(work->bgConfig, 7, 0, (7 * arg1) + 2, 0x20, 7, work->scrnData->rawData, 0, 0x18 * work->rows[arg1].unk30, 0x20, 0x30);
    FillWindowPixelBuffer(&work->rows[arg1].window0, 0);
    FillWindowPixelBuffer(&work->rows[arg1].window1, 0);
    FillWindowPixelBuffer(&work->rows[arg1].window2, 0);
    AddTextPrinterParameterizedWithColor(&work->rows[arg1].window0, FONT_MESSAGE, entry->name, 0, 1, 0xFF, 0xF0200, NULL);
    AddTextPrinterParameterizedWithColor(&work->rows[arg1].window1, FONT_MESSAGE, entry->sentence, 0, 0, 0xFF, 0x10200, NULL);
    ScheduleWindowCopyToVram(&work->rows[arg1].window0);
    ScheduleWindowCopyToVram(&work->rows[arg1].window1);
    string = entry->friendName;
    if (string != NULL) {
        AddTextPrinterParameterizedWithColor(&work->rows[arg1].window2, FONT_MESSAGE, string, 0, 1, 0xFF, 0xF0200, NULL);
    }
    ScheduleWindowCopyToVram(&work->rows[arg1].window2);
}

void ov34_0225DD04(Ov34Work *work) {
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;

    var_r6 = ov34_0225DC0C(work->chatLog->start, work->scroll);
    var_r7 = work->chatLog->count;
    if (var_r7 > 3) {
        var_r7 = 3;
    }
    if (work->scroll != work->prevScroll) {
        ov34_0225E560(work);
        work->prevScroll = (s32) work->scroll;
    }
    if (work->needsRedraw == 1) {
        var_r4 = 0;
        if (var_r7 > 0) {
            do {
                ov34_0225DC18(work, var_r4, &work->chatLog->entries[var_r6]);
                var_r6 = ov34_0225DC00(work->chatLog, var_r6);
                var_r4 += 1;
            } while (var_r4 < var_r7);
        }
        DC_FlushRange(GetBgTilemapBuffer(work->bgConfig, 7U), 0x600U);
        BgCopyOrUncompressTilemapBufferRangeToVram(work->bgConfig, 7U, GetBgTilemapBuffer(work->bgConfig, 7U), 0x600U, 0U);
        work->needsRedraw = 0;
    }
}
