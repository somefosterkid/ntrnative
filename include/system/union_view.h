// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/union_view.h, the lines in use so
// far.
#ifndef BWPORT_SYSTEM_UNION_VIEW_H
#define BWPORT_SYSTEM_UNION_VIEW_H

#include "types.h"

// The looks a player can have in the Union Room (union_view.c, a guessed name). A view is 0 to 7 for a male player and
// 8 to 15 for a female one

// The trainer type of a view, the first view's for one out of range
u16 func_02025c74(u32 view);

u32 func_02025cb0(u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 func_02025c88(u32 view);

// union_view.c: the message of a view's looks, in Black
u8 func_02025c9c(u32 view);

#endif // BWPORT_SYSTEM_UNION_VIEW_H
