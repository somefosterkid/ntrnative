#include "global.h"
struct Ov27Task;

#include "constants/heap.h"

#include "bg_window.h"
#include "bug_contest_internal.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"
#include "pm_string.h"
#include "pokemon.h"
#include "text.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// The DS build keeps this function in asm/overlay_27_0225A2CC_0225BED8.s (the typed C below compiles to the same code except one
// ldr of textX placed after the baseTile load instead of first). Narrow stack arguments are read as the asm does: width/height
// as bytes, baseTile as u16, textY as s16.
void ov27_0225BED8(Ov27Task *task, s32 msgId, s32 x, s32 y, s32 width, s32 height, s32 baseTile, s32 textX, s32 textY) {
    Window window;
    String *str;

    AddWindowParameterized(task->bgConfig, &window, 5, x, y, (u8)width, (u8)height, 4, (u16)baseTile);
    FillWindowPixelBuffer(&window, 0);
    str = ReadMsgData_ExpandPlaceholders(task->unk_4AC, task->unk_4A8, msgId, HEAP_ID_8);
    if (textX == -1) {
        textX = (s16)((s16)((u8)width * 8 - FontID_String_GetWidth(FONT_SYSTEM, str, 0)) / 2);
    }
    AddTextPrinterParameterizedWithColor(&window, FONT_SYSTEM, str, textX, (s16)textY, 0xFF, 0xE0200, NULL);
    String_Delete(str);
    CopyWindowToVram(&window);
    RemoveWindow(&window);
}
#endif

void ov27_0225BF84(Ov27Task *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov27_0225BFCC(Ov27Task *arg0, BugContest *arg1);

void ov27_0225BF84(Ov27Task *arg0, s32 arg1, s32 arg2, s32 num) {
    BufferIntegerAsString(arg0->unk_4AC, 0U, num, 2U, PRINTING_MODE_LEFT_ALIGN, 1);
    ov27_0225BED8(arg0, 0x1F, arg1, arg2, 6, 2, 0x186, 0, 0);
}

void ov27_0225BFCC(Ov27Task *arg0, BugContest *arg1) {
    BufferIntegerAsString(arg0->unk_4AC, 0U, (s32)GetMonData(arg1->mon, 0xA1, NULL), 3U, PRINTING_MODE_LEFT_ALIGN, 1);
    ov27_0225BED8(arg0, 0x21, 0xC, 0x15, 6, 2, 0x192, 4, 0);
}
