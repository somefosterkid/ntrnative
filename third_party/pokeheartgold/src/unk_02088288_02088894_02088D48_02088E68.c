#include "global.h"
#include "pokemon_summary_app.h"

s32 sub_0208AFA0(void *, s32);

s32 sub_0208BC78(void *);

s32 sub_02088E68(PokemonSummary *arg0);

s32 sub_02088E68(PokemonSummary *arg0) {
    sub_0208AFA0(arg0, 0);
    sub_0208BC78(arg0);
    arg0->swapMoveSlot = arg0->moveSlot;
    return 6;
}
