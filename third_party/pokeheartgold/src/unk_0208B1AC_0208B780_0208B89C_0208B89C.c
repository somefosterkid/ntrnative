#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208B714(PokemonSummary *arg0);

void sub_0208B780(Sprite **, s32, s32, s32, s32, s32, s32);

void sub_0208B89C(PokemonSummary *arg0);

void sub_0208B89C(PokemonSummary *arg0) {
    PartyExtraSub juice;
    struct PokeathlonPerformanceStars stars;
    struct PokeathlonTodayPerformance perf;
    BoxPokemon *boxMon;
    PokemonSummaryArgs *data;

    if (arg0->page == 2 && arg0->args->isFlag982Set != 0) {
        boxMon = sub_0208A520(arg0);
        MI_CpuFill8(&juice, 0, sizeof(PartyExtraSub));
        CalcBoxMonPokeathlonPerformance(boxMon, &perf);
        data = arg0->args;
        if (data->unk11 == 1) {
            Party_GetMonAprijuiceModifiers(data->party, &juice, data->partySlot);
        }
        CalcBoxmonPokeathlonStars(&stars, boxMon, (s8 *)&juice, HEAP_ID_19);
        sub_0208B780(arg0->sprites, 0x4F, perf.stats[4].hi, (u32)(stars.stars << 17) >> 29, stars.color[4], (s8)juice.unk_00[4], 0x68);
        sub_0208B780(arg0->sprites, 0x54, perf.stats[0].hi, (u32)(stars.stars << 29) >> 29, stars.color[0], (s8)juice.unk_00[0], 0x69);
        sub_0208B780(arg0->sprites, 0x59, perf.stats[2].hi, (u32)(stars.stars << 23) >> 29, stars.color[2], (s8)juice.unk_00[2], 0x6A);
        sub_0208B780(arg0->sprites, 0x5E, perf.stats[1].hi, (u32)(stars.stars << 26) >> 29, stars.color[1], (s8)juice.unk_00[1], 0x6B);
        sub_0208B780(arg0->sprites, 0x63, perf.stats[3].hi, (u32)(stars.stars << 20) >> 29, stars.color[3], (s8)juice.unk_00[3], 0x6C);
    } else {
        sub_0208B714(arg0);
    }
}
