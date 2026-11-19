#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

void ov18_021F0D7C(PokedexAppData *param_1);

void ov18_021F0D7C(PokedexAppData *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[2]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[4]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[10]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[11]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[12]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[13]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[14]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[15]);
    return;
}

void ov18_021EE3AC(PokedexAppData *, MsgData *, u32, u32, u32, u32, u32, u32, u32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021F0DD0(PokedexAppData *pokedexApp) {
    String *string;

    ov18_021F0D7C(pokedexApp);
    FillWindowPixelBuffer(pokedexApp->windows, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 3, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 5, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 8, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 9, 0);
    ov18_021F9648(pokedexApp->windows, pokedexApp->msgData, 0xaa, 0, 0, 4, 0x20100, 0);
    string = ov18_021E590C(pokedexApp->curSpecies, 2, HEAP_ID_POKEDEX_APP);
    BufferString(pokedexApp->msgFormat, 0, string, 2, 1, 2);
    String_Delete(string);
    ov18_021EE3AC(pokedexApp, pokedexApp->msgData, 3, 0xa9, 0x48, 0, 0, 0x20100, 2);
    ov18_021F9648(pokedexApp->windows + 5, pokedexApp->msgData, 0xaa, 0x18, 0, 0, 0x50900, 2);
    ov18_021F9648(pokedexApp->windows + 8, pokedexApp->msgData, 0xab, 0x30, 4, 4, 0xf0c00, 2);
    ov18_021F9648(pokedexApp->windows + 9, pokedexApp->msgData, 0xac, 0x30, 4, 4, 0xf0c00, 2);
    ScheduleWindowCopyToVram(pokedexApp->windows);
    ScheduleWindowCopyToVram(pokedexApp->windows + 3);
    ScheduleWindowCopyToVram(pokedexApp->windows + 5);
    ScheduleWindowCopyToVram(pokedexApp->windows + 8);
    ScheduleWindowCopyToVram(pokedexApp->windows + 9);
    ov18_021F0F68(pokedexApp, 6, (s32)pokedexApp->unk_18C5);
    ov18_021F0F68(pokedexApp, 7, (s32)pokedexApp->unk_18C6);
    return;
}
