#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "message_format.h"
#include "pm_string.h"

void ov18_021EE3AC(PokedexAppData *, MsgData *, u32, u32, u32, u32, u32, u32, u32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021F0C50(PokedexAppData *param_1);

void ov18_021F0C50(PokedexAppData *param_1) {
    String *string;

    FillWindowPixelBuffer(&param_1->windows[0], 0);
    FillWindowPixelBuffer(&param_1->windows[4], 0);
    FillWindowPixelBuffer(&param_1->windows[1], 0);
    ov18_021F9648(&param_1->windows[0], param_1->msgData, 0xaa, 0, 0, 4, 0x20100, 0);
    if (param_1->numSeenForms != 1) {
        ov18_021F9648(&param_1->windows[4], param_1->msgData, 0xa8, 0, 0, 4, 0xf0c00, 0);
    }
    string = ov18_021E590C(param_1->curSpecies, 2, HEAP_ID_POKEDEX_APP);
    BufferString(param_1->msgFormat, 0, string, 2, 1, 2);
    String_Delete(string);
    ov18_021EE3AC(param_1, param_1->msgData, 1, 0xa7, 0x48, 0, 0, 0x20100, 1);
    ScheduleWindowCopyToVram(&param_1->windows[0]);
    ScheduleWindowCopyToVram(&param_1->windows[4]);
    ScheduleWindowCopyToVram(&param_1->windows[1]);
    return;
}
