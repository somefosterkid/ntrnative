#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"

void ov18_021EE35C(PokedexAppData *, u8 *, u32);

void ov18_021F95FC(Window *, String *, u32, u32, u32, u32, u32);

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

extern u32 ov18_021F9E4C;

void ov18_021F021C(PokedexAppData *pokedexApp) {
    String *string;

    ov18_021EE35C((struct PokedexAppData *)((unsigned char *)(pokedexApp)), (unsigned char *)(&ov18_021F9E4C), 0xe);
    FillWindowPixelBuffer(pokedexApp->windows, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 2, 0);
    FillWindowPixelBuffer(pokedexApp->windows + 4, 0);
    ov18_021F9648(pokedexApp->windows, pokedexApp->msgData, 0x8e, 0, 0, 4, 0x20100, 0);
    string = ov18_021E590C(pokedexApp->curSpecies, 2, HEAP_ID_POKEDEX_APP);
    ov18_021F95FC(pokedexApp->windows + 2, string, 0x24, 0, 0, 0x20100, 2);
    String_Delete(string);
    ov18_021F9648(pokedexApp->windows + 4, pokedexApp->msgData, 0x84, 0x18, 0, 0, 0x50900, 2);
    ScheduleWindowCopyToVram(pokedexApp->windows);
    ScheduleWindowCopyToVram(pokedexApp->windows + 2);
    ScheduleWindowCopyToVram(pokedexApp->windows + 4);
    if (pokedexApp->unk_1860 == 1) {
        FillWindowPixelBuffer(pokedexApp->windows + 5, 0);
        FillWindowPixelBuffer(pokedexApp->windows + 7, 0);
        ov18_021F9648(pokedexApp->windows + 5, pokedexApp->msgData, 0x41, 0x1c, 0, 4, 0xf0c00, 2);
        ov18_021F9648(pokedexApp->windows + 7, pokedexApp->msgData, 0x42, 0x1c, 0, 4, 0xf0c00, 2);
        ScheduleWindowCopyToVram(pokedexApp->windows + 5);
        ScheduleWindowCopyToVram(pokedexApp->windows + 7);
    } else {
        FillWindowPixelBuffer(pokedexApp->windows + 6, 0);
        ov18_021F9648(pokedexApp->windows + 6, pokedexApp->msgData, 0x41, 0x1c, 0, 0, 0x50900, 2);
        ScheduleWindowCopyToVram(pokedexApp->windows + 6);
    }
    FillWindowPixelBuffer(pokedexApp->windows + 3, 0);
    ov18_021F9648(pokedexApp->windows + 3, pokedexApp->msgData, 0x80, 0x38, 0, 0, 0x20100, 2);
    CopyWindowPixelsToVram_TextMode(pokedexApp->windows + 3);
    ov18_021F03E0(pokedexApp);
    ov18_021F0428(pokedexApp);
    return;
}
