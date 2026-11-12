// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_plist_cursor.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_PLIST_CURSOR_H
#define BWPORT_BATTLE_B_PLIST_CURSOR_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BPlistCursor_Create
void func_ov198_02211998(BPlistWork *work, u8 page, int pos);
// pokebw2's BPlistCursor_Delete
void func_ov198_022119e8(BPlistWork *work);
// pokebw2's BPlistCursor_ChangePage
void func_ov198_022119f8(BPlistWork *work, u8 page, int pos);

#endif // BWPORT_BATTLE_B_PLIST_CURSOR_H
