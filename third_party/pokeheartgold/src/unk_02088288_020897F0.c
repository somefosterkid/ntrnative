#include "global.h"

#include "pokemon_summary_app.h"
#include "pokemon_types_def.h"

#include "constants/heap.h"

#include "heap.h"


Pokemon *AllocMonZeroed(enum HeapID heapID);
void CopyBoxPokemonToPokemon(BoxPokemon *boxMon, Pokemon *mon);

void sub_0208981C(PokemonSummary *summaryScreen, Pokemon *mon, PokemonSummaryMonData *monData);

void sub_020897F0(PokemonSummary *summaryScreen, BoxPokemon *boxMon, PokemonSummaryMonData *monData);

void sub_020897F0(PokemonSummary *summaryScreen, BoxPokemon *boxMon, PokemonSummaryMonData *monData) {
    Pokemon *mon = AllocMonZeroed(HEAP_ID_19);

    CopyBoxPokemonToPokemon(boxMon, mon);
    sub_0208981C(summaryScreen, mon, monData);
    Heap_Free(mon);
}
