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
#include "unk_0208C3E4.h"
#include "unk_02088288.h"

void sub_020897C0(PokemonSummary *arg0);

void sub_02089C50(PokemonSummary *arg0);

void sub_0208B4EC(PokemonSummary *arg0);

void sub_0208B5A8(PokemonSummary *arg0);

void sub_0208B9C8(PokemonSummary *arg0);

void sub_0208BA60(void *arg0);

void sub_0208BCD4(PokemonSummary *arg0);

void sub_0208BE70(void *arg0);

void sub_0208C2A0(PokemonSummary *arg0);

void sub_0208E4B4(PokemonSummary *arg0);

void sub_02089F98(PokemonSummary *);
s32 sub_0208B400(PokemonSummary *);
s32 sub_0208B448(PokemonSummary *);
s32 sub_0208B48C(PokemonSummary *);
s32 sub_0208B89C(PokemonSummary *);
s32 sub_0208BD38(PokemonSummary *);
s32 sub_0208BFD0(PokemonSummary *);

void sub_0208A234(PokemonSummary *arg0, s8 arg1);

void sub_0208A234(PokemonSummary *arg0, s8 arg1) {
    arg0->args->partySlot = arg1;
    sub_020897C0(arg0);
    sub_02089C50(arg0);
    sub_0208C57C(arg0);
    sub_0208C614(arg0);
    sub_0208C6B4(arg0);
    sub_02089F98(arg0);
    sub_0208E4B4(arg0);
    sub_0208B448(arg0);
    sub_0208BA60(arg0);
    sub_0208B5A8(arg0);
    sub_0208B89C(arg0);
    sub_0208B9C8(arg0);
    sub_0208C2A0(arg0);
    sub_0208BE70(arg0);
    sub_0208B48C(arg0);
    sub_0208B4EC(arg0);
    sub_0208BCD4(arg0);
    sub_0208BD38(arg0);
    sub_0208CBD4(arg0);
    sub_0208B400(arg0);
    sub_0208BFD0(arg0);
}
