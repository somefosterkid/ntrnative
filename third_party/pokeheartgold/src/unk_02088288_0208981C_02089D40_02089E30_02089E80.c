#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02088288.h"

BOOL sub_02089E80(PokemonSummary *summaryScreen, s32 badge);

BOOL sub_02089E80(PokemonSummary *summaryScreen, s32 badge) {
    return (u32)((((u32)(summaryScreen->args->unk15)) & (1 << (badge & badge))) != 0);
    badge = 0xffU;
}
