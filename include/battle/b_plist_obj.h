// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_plist_obj.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_PLIST_OBJ_H
#define BWPORT_BATTLE_B_PLIST_OBJ_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BPlistObj_UpdatePokeIconAnims
void func_ov198_0220f1a4(BPlistWork *wk);
// pokebw2's BPlistObj_Init
void func_ov198_0220e2b4(BPlistWork *wk);
// pokebw2's BPlistObj_ShowMoveTypes
void func_ov198_0220f060(BPlistWork *wk);
// pokebw2's BPlistObj_SetPage
void func_ov198_0220ea68(BPlistWork *wk, u8 page);
// pokebw2's BPlistObj_Exit
void func_ov198_0220e74c(BPlistWork *wk);
// pokebw2's BPlistObj_MovePlate
void func_ov198_0220f22c(BPlistWork *wk, int pos, s16 dx);
// pokebw2's BPlistObj_SwapPlates
void func_ov198_0220f2dc(BPlistWork *wk, int posA, int posB);

#endif // BWPORT_BATTLE_B_PLIST_OBJ_H
