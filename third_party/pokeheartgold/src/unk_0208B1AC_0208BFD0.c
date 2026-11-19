#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208BE00(PokemonSummary *, void *, s32);

void sub_0208BFD0(PokemonSummary *arg0);

s16 sub_0208C0A4(u32 statValue, s16 endCoord, s16 startCoord);

void sub_0208BFD0(PokemonSummary *arg0) {
    BoxPokemon *boxMon;

    if (arg0->args->unk11 == 2) {
        boxMon = sub_0208A520(arg0);
        if (arg0->boxIconSprite == 0) {
            thunk_Sprite_SetDrawFlag(arg0->sprites[0x49], 1);
            thunk_Sprite_SetDrawFlag(arg0->sprites[0x4A], 0);
            sub_0208BE00(arg0, boxMon, 0x49);
            thunk_Sprite_SetPaletteOverride(arg0->sprites[0x49], Boxmon_GetIconPalette(boxMon) + 0xC);
        } else {
            thunk_Sprite_SetDrawFlag(arg0->sprites[0x49], 0);
            thunk_Sprite_SetDrawFlag(arg0->sprites[0x4A], 1);
            sub_0208BE00(arg0, boxMon, 0x4A);
            thunk_Sprite_SetPaletteOverride(arg0->sprites[0x4A], Boxmon_GetIconPalette(boxMon) + 0xC);
        }
        arg0->boxIconSprite = (s32)(1 ^ arg0->boxIconSprite);
    }
}

void sub_0208C068(PokemonSummary *arg0) {
    if (arg0->page == 1) {
        Sprite_SetPositionXY(arg0->sprites[19], 0xA0, 0x10);
    } else {
        Sprite_SetPositionXY(arg0->sprites[19], 0x20, 0x44);
    }
    thunk_Sprite_SetDrawFlag(arg0->sprites[19], 1);
}

s16 sub_0208C0A4(u32 statValue, s16 endCoord, s16 startCoord) {
    u32 finalCoord;

    statValue += 44;

    if (startCoord > endCoord) {
        finalCoord = ((startCoord - endCoord) * statValue) << 16;
        finalCoord = (finalCoord / 300) >> 16;

        return startCoord + (s16)finalCoord * -1;
    }

    finalCoord = ((endCoord - startCoord) * statValue) << 16;
    finalCoord = (finalCoord / 300) >> 16;

    return startCoord + (s16)finalCoord;
}
