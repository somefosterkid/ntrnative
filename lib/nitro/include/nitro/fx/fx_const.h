// NitroSDK's nitro/fx/fx_const.h, the fixed point constants (libntr generates it from fx_const.csv).
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_CONST_H
#define BWPORT_NITRO_FX_FX_CONST_H

#include "nitro/fx/fx.h"

#define FX16_SQRT1_3 ((fx16)0x093d)
#define FX16_ONE (1 << 12)
#define FX32_ONE (1 << FX32_SHIFT)
#define FX32_HALF (FX32_ONE / 2)

// An angle in fixed point degrees as a 16-bit angle
#define FX64C_65536_360 ((s64)0x000000b60b60b60bLL)

#endif // BWPORT_NITRO_FX_FX_CONST_H
