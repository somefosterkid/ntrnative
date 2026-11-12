// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/hp_gauge.h.
#ifndef BWPORT_SYSTEM_HP_GAUGE_H
#define BWPORT_SYSTEM_HP_GAUGE_H

#include "types.h"

// The HP gauges' size and color. The ROM doesn't name the file that holds these. The names are Black's dsd names;
// pokebw2's guesses are in the comments

// The color of the gauge for HP out of a maximum: above half, above a fifth, below, and none left
#define HP_GAUGE_COLOR_GREEN 0
#define HP_GAUGE_COLOR_YELLOW 1
#define HP_GAUGE_COLOR_RED 2
#define HP_GAUGE_COLOR_NONE 3

// The pixels of a gauge width pixels long to fill for HP out of a maximum, at least 1 while any HP is left
// (HPGauge_GetFill)
u8 func_0202d01c(u32 hp, u32 maxHp, u32 width);
// HPGauge_GetColor
u8 func_0202d034(u32 hp, u32 maxHp);

#endif // BWPORT_SYSTEM_HP_GAUGE_H
