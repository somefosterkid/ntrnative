#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"
#include "bg_window.h"
#include "pm_string.h"

void ov18_021F95FC(void *, String *, u32, u32, u32, u32, u32);

void ov18_021EECB0(PokedexAppData *pokedexApp, u16 param_2, s32 param_3);

void ov18_021EECB0(PokedexAppData *pokedexApp, u16 param_2, s32 param_3) {
    String *string;

    FillWindowPixelBuffer(&pokedexApp->windows[param_3], 0);
    string = ov18_021E590C(param_2, pokedexApp->unk_185C, HEAP_ID_POKEDEX_APP);
    ov18_021F95FC(&pokedexApp->windows[param_3], string, 0, 0, 0, 0x20100, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(&pokedexApp->windows[param_3]);
    return;
}
