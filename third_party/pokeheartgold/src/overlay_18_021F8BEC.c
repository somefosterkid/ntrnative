#include "global.h"
struct PokedexEntryPopup;

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "overlay_18_pokedex_entry_popup.h"



void ov18_021F8BEC(PokedexEntryPopup *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
}


void ov18_021F8C0C(PokedexEntryPopup *arg0) {
    PaletteData_BeginPaletteFade(arg0->paletteData, 5U, 0xFFFFU, 1, 0x10U, 0U, 0U);
    Pokepic_StartPaletteFade(arg0->pokepic, 0x10, 0, 0, 0);
    PaletteData_SetAutoTransparent(arg0->paletteData, 0);
}


s32 ov18_021F8C48(PokedexEntryPopup *arg0) {
    if ((PaletteData_GetSelectedBuffersBitmask(arg0->paletteData) == 0) && (Pokepic_ResumePaletteFade(arg0->pokepic) == 0)) {
        return 1;
    }
    return 0;
}

void ov18_021F8C68(PokedexEntryPopup *arg0) {
    u16 v0;

    arg0->palFlashTimer = (u16)(arg0->palFlashTimer + 1);
    v0 = arg0->palFlashTimer;
    if (v0 == 16) {
        BgTilemapRectChangePalette(arg0->bgConfig, 2U, 0U, 0U, 0x20U, 2U, 7U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
        return;
    }
    switch (v0) {
    case 32:
        BgTilemapRectChangePalette(arg0->bgConfig, 2U, 0U, 0U, 0x20U, 2U, 0U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 2U);
        arg0->palFlashTimer = 0U;
        return;
    }
}
