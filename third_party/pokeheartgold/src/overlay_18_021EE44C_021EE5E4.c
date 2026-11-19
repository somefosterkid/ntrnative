#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

void ov18_021EE520(PokedexAppData *param_1, s32 param_2, u32 param_3);

void ov18_021EE5E4(PokedexAppData *param_1, s32 param_2, u32 param_3);

void ov18_021EE5E4(PokedexAppData *param_1, s32 param_2, u32 param_3) {
    ov18_021EE520(param_1, param_2, param_3);
    ScheduleWindowCopyToVram(&param_1->windows[param_2]);
    return;
}
