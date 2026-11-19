#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"
#include "unk_0208B1AC.h"

void sub_0208A7F8(PokemonSummary *summary, u32 move);
#include "unk_0208C3E4.h"

void sub_0208BBDC(PokemonSummary *arg0);

void sub_0208A79C(PokemonSummary *arg0);

void sub_0208A79C(PokemonSummary *arg0) {
    u16 temp_r1_2;
    u32 temp_r1;

    sub_0208BBDC(arg0);
    temp_r1 = arg0->moveSlot;
    if (temp_r1 == 4) {
        temp_r1_2 = arg0->args->moveToLearn;
        if (temp_r1_2 != 0) {
            sub_0208A7F8(arg0, (u32)temp_r1_2);
            return;
        }
        sub_0208A7F8(arg0, -1U);
        return;
    }
    if (temp_r1 == 5) {
        sub_0208A7F8(arg0, -1U);
        return;
    }
    sub_0208A7F8(arg0, (u32)arg0->mon.moves[temp_r1]);
}
