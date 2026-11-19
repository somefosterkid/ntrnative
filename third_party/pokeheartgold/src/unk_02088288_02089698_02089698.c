#include "global.h"

#include "bg_window.h"
#include "pokemon_summary_app.h"

u8 sub_02089698(PokemonSummary *arg0);

u8 sub_02089698(PokemonSummary *arg0) {
    u8 temp_r0;

    temp_r0 = arg0->unk7BE;
    switch (temp_r0) {
    case 0:
        BgTilemapRectChangePalette(arg0->bgConfig, arg0->flashBgId, arg0->flashX, arg0->flashY, (s32) arg0->flashWidth, (s32) arg0->flashHeight, arg0->flashPalette);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, arg0->flashBgId);
        arg0->unk7BE = (arg0->unk7BE + 1);
        break;
    case 1:
        arg0->flashTimer = (arg0->flashTimer + 1);
        if (arg0->flashTimer == 4) {
            BgTilemapRectChangePalette(arg0->bgConfig, arg0->flashBgId, arg0->flashX, arg0->flashY, (s32) arg0->flashWidth, (s32) arg0->flashHeight, arg0->restorePalette);
            ScheduleBgTilemapBufferTransfer(arg0->bgConfig, arg0->flashBgId);
            arg0->flashTimer = 0U;
            arg0->unk7BE = (arg0->unk7BE + 1);
        }
        break;
    case 2:
        arg0->flashTimer = (arg0->flashTimer + 1);
        if (arg0->flashTimer == 2) {
            arg0->unk7BE = 0U;
            return arg0->flashNextState;
        }
        break;
    }
    return 0xFU;
}
