// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/ddemo_obj.h: the lines in use so far.
#ifndef BWPORT_DEMO_DDEMO_OBJ_H
#define BWPORT_DEMO_DDEMO_OBJ_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's DDemoObj_Main1st
void func_ov180_021b7d5c(DDemoWork *wk);
// pokebw2's DDemoObj_Init
void func_ov180_021b7318(DDemoWork *wk, u32 scene);
// pokebw2's DDemoObj_Init1st
void func_ov180_021b7cf0(DDemoWork *wk);
// pokebw2's DDemoObj_Exit
void func_ov180_021b7390(DDemoWork *wk);
// pokebw2's DDemoObj_AddPoke
// Loads the Pokémon being shown, in place of the last one
void func_ov180_021b761c(DDemoWork *wk);
// pokebw2's DDemoObj_PrintPokeInfo
void func_ov180_021b78e0(DDemoWork *wk);
// pokebw2's DDemoObj_MoveFontOam
// Keeps the first scene's texts on their plates
void func_ov180_021b7c08(DDemoWork *wk);
// pokebw2's DDemoObj_Move
void func_ov180_021b7438(DDemoWork *wk, u32 id, s16 x, s16 y);
// pokebw2's DDemoObj_GetPos
void func_ov180_021b7408(DDemoWork *wk, u32 id, s16 *x, s16 *y);
// pokebw2's DDemoObj_SetVisible
void func_ov180_021b746c(DDemoWork *wk, u32 id, BOOL visible);
// pokebw2's DDemoObj_SetAnime
void func_ov180_021b7488(DDemoWork *wk, u32 id, u32 sequence);
// pokebw2's DDemoObj_IsAnimating
BOOL func_ov180_021b74b4(DDemoWork *wk, u32 id);
// pokebw2's DDemoObj_Exit2nd
void func_ov180_021b7f0c(DDemoWork *wk);
// pokebw2's DDemoObj_SetFontOamVisible
void func_ov180_021b7c60(DDemoWork *wk, u32 id, BOOL visible);
// pokebw2's DDemoObj_MainConfetti
// Removes the confetti that have fallen, and drops a new one every other frame or so
void func_ov180_021b7f38(DDemoWork *wk);
// pokebw2's DDemoObj_SetPos
void func_ov180_021b73e4(DDemoWork *wk, u32 id, s16 x, s16 y);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DDemoObj_Main2nd
void func_ov180_021b7f30(DDemoWork *wk);
// pokebw2's DDemoObj_Exit1st
void func_ov180_021b7d44(DDemoWork *wk);
// pokebw2's DDemoObj_LoadTypePalette
// Gives the actors' palette the colors of the Pokémon's type
void func_ov180_021b7750(DDemoWork *wk);
// pokebw2's DDemoObj_Init2nd
void func_ov180_021b7d80(DDemoWork *wk);

#endif // BWPORT_DEMO_DDEMO_OBJ_H
