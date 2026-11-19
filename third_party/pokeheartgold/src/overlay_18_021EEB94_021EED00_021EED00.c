#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "pm_string.h"

void ov18_021F95FC(void *, String *, u32, u32, u32, u32, u32);

void ov18_021EED00(PokedexAppData *param_1, u16 param_2, s32 param_3, s32 param_4);

void ov18_021EED00(PokedexAppData *param_1, u16 param_2, s32 param_3, s32 param_4) {
    String *string;
    FillWindowPixelBuffer(&param_1->windows[param_4], 0);
    if (param_1->unk_1030[param_3].unk_2 == 2) {
        string = ov18_021E595C((u16)param_2, (u16)param_1->unk_185C, HEAP_ID_POKEDEX_APP);
    } else {
        string = ov18_021E595C(0, (u16)param_1->unk_185C, HEAP_ID_POKEDEX_APP);
    }
    ov18_021F95FC(&param_1->windows[param_4], string, 0x7c, 0, 0, 0x20100, 1);
    String_Delete(string);
    ScheduleWindowCopyToVram(&param_1->windows[param_4]);
    return;
}
