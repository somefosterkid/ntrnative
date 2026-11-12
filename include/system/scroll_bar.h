// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/scroll_bar.h.
#ifndef BWPORT_SYSTEM_SCROLL_BAR_H
#define BWPORT_SYSTEM_SCROLL_BAR_H

#include "types.h"

// A scroll bar between top and bottom, whose bar is barSize long: the value from 0 to max where the bar's center is
// (ScrollBar_GetValue), and where the bar's center is for a value (ScrollBar_GetPos). The file's name and the
// functions' in parentheses are pokebw2's guesses
u32 func_0202ede4(u32 max, u32 pos, u32 top, u32 bottom, u32 barSize);
u32 func_0202ee0c(u32 max, u32 value, u32 top, u32 bottom, u32 barSize);

#endif // BWPORT_SYSTEM_SCROLL_BAR_H
