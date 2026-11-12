// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_bag_anm.h: the lines in use so far.
#ifndef BWPORT_BATTLE_B_BAG_ANM_H
#define BWPORT_BATTLE_B_BAG_ANM_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BBagAnm_DeleteButtons
void func_ov197_0220bd14(BBagWork *work);
// pokebw2's BBagAnm_PutPageButtons
void func_ov197_0220bd24(BBagWork *work, u8 page);
// pokebw2's BBagAnm_StartButtonAnm
void func_ov197_0220bf20(BBagWork *work, u8 button);
// pokebw2's BBagAnm_MainButtonAnm
void func_ov197_0220bf40(BBagWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BBagAnm_CreateButtons
void func_ov197_0220bbe4(BBagWork *work);

#endif // BWPORT_BATTLE_B_BAG_ANM_H
