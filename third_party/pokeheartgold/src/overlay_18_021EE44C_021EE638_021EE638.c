#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "pm_string.h"
#include "pokedex.h"
#include "wild_encounter.h"

void ov18_021F95FC(Window *, String *, u32, u32, u32, u32, u32);

void ov18_021EE638(PokedexAppData *pokedexApp, u32 species, int a2) {
    String *string;
    BOOL BVar1;

    if (species != 0) {
        FillWindowPixelBuffer(pokedexApp->windows + a2, 0);
        string = ov18_021E590C((s32)species, (u32)pokedexApp->unk_185C, HEAP_ID_POKEDEX_APP);
        BVar1 = Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex);
        if (BVar1 == 1) {
            ov18_021F95FC(pokedexApp->windows + a2, string, 0x38, 0, 0, 0x20100, 2);
        } else {
            ov18_021F95FC(pokedexApp->windows + a2, string, 0x2c, 0, 0, 0x20100, 2);
        }
        String_Delete(string);
        ScheduleWindowCopyToVram(pokedexApp->windows + a2);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(pokedexApp->windows + a2);
    return;
}
