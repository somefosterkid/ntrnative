#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "pokemon_summary_app.h"

void sub_0208DB58(PokemonSummary *param0);

void sub_0208DB58(PokemonSummary *param0) {
    ScheduleBgTilemapBufferTransfer(param0->bgConfig, GF_BG_LYR_MAIN_1);
}
