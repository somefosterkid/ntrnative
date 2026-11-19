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
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

s8 sub_0208A2E0(PokemonSummary *summaryScreen, s8 delta);

s32 sub_0208A234(PokemonSummary *, u8);

void sub_0208A2C0(PokemonSummary *arg0, s8 arg1);

void sub_0208A2C0(PokemonSummary *arg0, s8 arg1) {
    s8 v0;

    v0 = sub_0208A2E0(arg0, arg1);
    if (v0 != -1) {
        sub_0208A234(arg0, (u8)v0);
    }
}

s8 sub_0208A4B8(PokemonSummary *summaryScreen, s8 delta);

s8 sub_0208A3F4(PokemonSummary *summaryScreen, s8 delta);

s8 sub_0208A45C();

s8 sub_0208A2E0(PokemonSummary *summaryScreen, s8 delta) {
    u8 v0;

    v0 = summaryScreen->args->unk11;
    switch (v0) {
    case 0:
        return sub_0208A3F4(summaryScreen, delta);
    case 1:
        return sub_0208A45C();
    case 2:
        return sub_0208A4B8(summaryScreen, delta);
    default:
        return -1;
    }
}
