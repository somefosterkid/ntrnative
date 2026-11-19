#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "message_format.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "string_util.h"

s32 ov18_021EE3AC(void *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

void ov18_021EE75C(PokedexAppData *arg0, u32 arg1, s32 arg2);

void ov18_021EE75C(PokedexAppData *arg0, u32 arg1, s32 arg2) {
    FillWindowPixelBuffer(&arg0->windows[arg2], 0U);
    if (arg1 != 0) {
        BufferIntegerAsString(arg0->msgFormat, 0U, (s32)Pokedex_ConvertToCurrentDexNo((s32)arg0->unk_1858, arg1), 3U, PRINTING_MODE_LEADING_ZEROS, 1);
        ov18_021EE3AC(arg0, arg0->msgData, arg2, 9, 1, 0, 4, 0x20100, 0);
        ScheduleWindowCopyToVram(&arg0->windows[arg2]);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[arg2]);
}

s32 ov18_021F95FC(void *, String *, s32, s32, s32, s32, s32);

void ov18_021EE7DC(PokedexAppData *arg0, u16 arg1, s32 arg2);

void ov18_021EE7DC(PokedexAppData *arg0, u16 arg1, s32 arg2) {
    String *temp_r0;

    FillWindowPixelBuffer(&arg0->windows[arg2], 0U);
    if (arg1 != 0) {
        temp_r0 = ov18_021E590C(arg1, 2, HEAP_ID_POKEDEX_APP);
        ov18_021F95FC(&arg0->windows[arg2], temp_r0, 0, 0, 4, 0x20100, 0);
        String_Delete(temp_r0);
        ScheduleWindowCopyToVram(&arg0->windows[arg2]);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[arg2]);
}
