#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_02219BC4(Window *, MessageFormat *, MsgData *, u32, enum HeapID);

void ov96_02219BDC(Window *, MessageFormat *, MsgData *, u32, enum HeapID, s16, s16);

void ov96_02219B30(Ov96DigitWork *work);

void ov96_02219B30(Ov96DigitWork *work) {
    String *string;

    FillWindowPixelBuffer(&work->windows[0], 0U);
    string = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(work->course, (s32)work->playerIdx), work->heapId);
    AddTextPrinterParameterizedWithColor(&work->windows[0], FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    ov96_02219BDC(&work->windows[2], work->msgFormat, work->msgData, 0x130, work->heapId, 2, 2);
    String_Delete(string);
    CopyWindowToVram(&work->windows[0]);
    BufferString(work->msgFormat, 0U, work->names[1], 2, 1, 2);
    ov96_02219BC4(&work->windows[1], work->msgFormat, work->msgData, 0x9A, work->heapId);
}
