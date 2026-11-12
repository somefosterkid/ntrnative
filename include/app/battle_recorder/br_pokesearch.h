// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_pokesearch.h: the
// lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_POKESEARCH_H
#define BWPORT_APP_BATTLE_RECORDER_BR_POKESEARCH_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of BR_POKESEARCH_SELECT_NONE and BR_POKESEARCH_SELECT_DECIDE in Black
// What BrPokeSearch_GetSelect gives
enum {
    BR_POKESEARCH_SELECT_NONE,
    BR_POKESEARCH_SELECT_CANCEL,
    BR_POKESEARCH_SELECT_DECIDE,
};

// pokebw2's BrPokeSearch_Exit
void func_ov186_021ffc6c(BrPokeSearch *p_wk);
// pokebw2's BrPokeSearch_PrintMain
// TRUE once its text is printed
BOOL func_ov186_021ffe34(BrPokeSearch *p_wk);
// pokebw2's BrPokeSearch_StartUp
void func_ov186_021ffe5c(BrPokeSearch *p_wk);
// pokebw2's BrPokeSearch_CleanUp
void func_ov186_021ffe70(BrPokeSearch *p_wk);
// pokebw2's BrPokeSearch_GetSelect
// A BR_POKESEARCH_SELECT_*, with the species in species once one is decided
u32 func_ov186_021ffe90(const BrPokeSearch *cp_wk, u16 *species);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrPokeSearch_Init
BrPokeSearch *func_ov186_021ffbd4(PokeDexSave *pokedex, BrRes *res, ClActUnit *unit, BmpOamSys *bmpoam, BrFade *fade,
                                BrBallEffect *ballMain, BrBallEffect *ballSub, HeapID heapId);
// pokebw2's BrPokeSearch_Main
void func_ov186_021ffc9c(BrPokeSearch *p_wk);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_POKESEARCH_H
