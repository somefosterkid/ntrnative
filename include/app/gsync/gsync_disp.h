// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync/gsync_disp.h: the lines in use so
// far.
#ifndef BWPORT_APP_GSYNC_GSYNC_DISP_H
#define BWPORT_APP_GSYNC_GSYNC_DISP_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The floating items, and after them the floating Pokémon
#define GSYNC_DISP_FLOAT_ITEM_COUNT 20

// Black 2's, not checked against Black
#define GSYNC_DISP_FLOAT_POKEMON_COUNT 10

// pokebw2's GSyncDisp_CreateSubActor
// The actor on the touch screen
void func_ov123_021d62a8(GSyncDisp *disp);
// pokebw2's GSyncDisp_SetActorSequence
void func_ov123_021d62bc(GSyncDisp *disp, int index, int sequence);
// pokebw2's GSyncDisp_SetActorCallback
void func_ov123_021d62d0(GSyncDisp *disp, int index, const ClActorCallback *callback);
// pokebw2's GSyncDisp_DeleteActor
void func_ov123_021d62e0(GSyncDisp *disp, int index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GSyncDisp_Create
GSyncDisp *func_ov123_021d5d94(HeapID heapId);
// pokebw2's GSyncDisp_Main
void func_ov123_021d5e60(GSyncDisp *disp);
// pokebw2's GSyncDisp_Free
void func_ov123_021d5e8c(GSyncDisp *disp);
// pokebw2's GSyncDisp_CreateActor
// The actors of the top screen, one for each of their sequences
void func_ov123_021d62b4(GSyncDisp *disp, int index);
// pokebw2's GSyncDisp_PokeIconSequence2
// The icon of the Pokémon sent to the Dream World, and its animations
void func_ov123_021d63d4(GSyncDisp *disp);
// pokebw2's GSyncDisp_CreatePokeIcon
void func_ov123_021d6458(GSyncDisp *disp, BoxPkm *pkm, u32 surface);
// pokebw2's GSyncDisp_StartPokeIcon
void func_ov123_021d651c(GSyncDisp *disp);
// pokebw2's GSyncDisp_PokeIconSequence1
void func_ov123_021d652c(GSyncDisp *disp);
// pokebw2's GSyncDisp_StartWave
// The top screen's wave, and its fade in or out
void func_ov123_021d65f4(GSyncDisp *disp);
// pokebw2's GSyncDisp_StartFade
void func_ov123_021d6650(GSyncDisp *disp, BOOL fadeIn);
// pokebw2's GSyncDisp_SetProgress
// Fills the progress gauge to percent
void func_ov123_021d66d0(GSyncDisp *disp, int percent);
// pokebw2's GSyncDisp_UpdateFloats
void func_ov123_021d6934(GSyncDisp *disp);
// pokebw2's GSyncDisp_SetFloatPokemon
// The Pokémon and items that drift down the screen, whose graphics GSyncDisp_LoadFloatGraphics then loads
void func_ov123_021d6a70(GSyncDisp *disp, int index, int species, int form, int sex);
// pokebw2's GSyncDisp_SetFloatItem
void func_ov123_021d6aa8(GSyncDisp *disp, int index, u16 item);
// pokebw2's GSyncDisp_LoadFloatGraphics
void func_ov123_021d6ab8(GSyncDisp *disp);

#endif // BWPORT_APP_GSYNC_GSYNC_DISP_H
