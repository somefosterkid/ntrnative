// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dpc_obj.h: the lines in use so far.
#ifndef BWPORT_APP_DPC_OBJ_H
#define BWPORT_APP_DPC_OBJ_H

#include "types.h"
#include "app/dpc_main.h"

// pokebw2's DpcObj_GetPokeBase
// The first actor of the set that is shown
u32 func_ov171_021de5f4(DendouPCWork *wk);
// pokebw2's DpcObj_GetPos
void func_ov171_021de0e4(DendouPCWork *wk, u32 id, s16 *x, s16 *y);

// pokebw2's DpcObj_Init
void func_ov171_021ddfe8(DendouPCWork *wk);
// pokebw2's DpcObj_Animate
void func_ov171_021de030(DendouPCWork *wk);
// pokebw2's DpcObj_SetVisible
void func_ov171_021de064(DendouPCWork *wk, u32 id, BOOL visible);
// pokebw2's DpcObj_SetAnime
void func_ov171_021de080(DendouPCWork *wk, u32 id, u32 sequence);
// pokebw2's DpcObj_IsAnimating
BOOL func_ov171_021de0ac(DendouPCWork *wk, u32 id);
// pokebw2's DpcObj_SetPos
void func_ov171_021de0c0(DendouPCWork *wk, u32 id, s16 x, s16 y);
// pokebw2's DpcObj_LoadPokemon
// Loads the page's Pokémon into the free set of actors and frees the other set
void func_ov171_021de388(DendouPCWork *wk);
// pokebw2's DpcObj_SetPokePos
// Puts a Pokémon's actor on the ring at an angle in degrees
void func_ov171_021de608(DendouPCWork *wk, u32 id, s32 angle);
// pokebw2's DpcObj_SortPriority
// Draws the Pokémon at the front of the ring over those behind
void func_ov171_021de684(DendouPCWork *wk);
// pokebw2's DpcObj_SetPokeFade
// Sets how dark each Pokémon is by its place behind the front one, or lights them all
void func_ov171_021de76c(DendouPCWork *wk, BOOL on);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DpcObj_Exit
void func_ov171_021de014(DendouPCWork *wk);

#endif // BWPORT_APP_DPC_OBJ_H
