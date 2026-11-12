// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_bag_ui.h: the lines in use so far.
#ifndef BWPORT_BATTLE_B_BAG_UI_H
#define BWPORT_BATTLE_B_BAG_UI_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BBagUi_CreateCursor
void func_ov197_0220d8c4(BBagWork *work, u8 page, int pos);
// pokebw2's BBagUi_DeleteCursor
void func_ov197_0220d914(BBagWork *work);
// pokebw2's BBagUi_ChangeCursorPage
void func_ov197_0220d924(BBagWork *work, u8 page, int pos);

#endif // BWPORT_BATTLE_B_BAG_UI_H
