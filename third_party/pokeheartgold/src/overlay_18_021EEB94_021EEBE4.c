#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "bg_window.h"
#include "unk_02091278.h"

void ov18_021F9648(void *, void *, s32, u32, u32, u32, u32, u32);

void ov18_021EEBE4(PokedexAppData *pokedexApp, s32 param_2);

void ov18_021EEBE4(PokedexAppData *pokedexApp, s32 param_2) {
    s32 iVar1;

    FillWindowPixelBuffer(&pokedexApp->windows[param_2], 0);
    iVar1 = LanguageToDexFlag(pokedexApp->unk_185C);
    ov18_021F9648(&pokedexApp->windows[param_2], pokedexApp->msgData, iVar1 + 0x7a, 0x38, 0, 0, 0x20100, 2);
    ScheduleWindowCopyToVram(&pokedexApp->windows[param_2]);
    return;
}
