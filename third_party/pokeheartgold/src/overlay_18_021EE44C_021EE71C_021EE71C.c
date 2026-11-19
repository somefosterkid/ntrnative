#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EE71C(PokedexAppData *param_1, s32 param_2);

void ov18_021EE71C(PokedexAppData *param_1, s32 param_2) {
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    ov18_021F9648(&param_1->windows[param_2], param_1->msgData, 8, 0, 0, 4, 0x20100, 0);
    ScheduleWindowCopyToVram(&param_1->windows[param_2]);
    return;
}
