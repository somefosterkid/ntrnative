#include "global.h"

#include "pokemon_summary_app.h"
#include "unk_02005D10.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02089D68(PokemonSummary *param_1, s8 param_2);
#else
void sub_02089D68(PokemonSummary *param_1, s32 param_2);
#endif

void sub_02089E14(PokemonSummary *param_1, u32 param_2);

void sub_02089E14(PokemonSummary *param_1, u32 param_2) {
    PlaySE(0x5e1);
    sub_02089D68(param_1, param_2 & 0xff);
    return;
}
