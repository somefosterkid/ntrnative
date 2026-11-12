// NitroSDK's nitro/fx/fx_cp.h, the divisions and square roots by the coprocessor.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fx.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_FX_FX_CP_H
#define BWPORT_NITRO_FX_FX_CP_H

#include "nitro/fx/fx.h"

fx32 FX_Div(fx32 numer, fx32 denom);
fx32 FX_Inv(fx32 x);
s32 FX_ModS32(s32 numer, s32 denom);
fx32 FX_Sqrt(fx32 x);
fx64c FX_InvFx64c(fx32 denom);
fx32 FX_InvSqrt(fx32 x);
fx64c FX_GetDivResultFx64c(void);
fx32 FX_GetDivResult(void);
void FX_InvAsync(fx32 denom);
void FX_DivAsync(fx32 numer, fx32 denom);
s32 FX_DivS32(s32 numer, s32 denom);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fx/fx_cp.h
static inline fx64c FX_GetInvResultFx64c (void)
{
	return FX_GetDivResultFx64c();
}
static inline fx32 FX_GetInvResult (void)
{
	return FX_GetDivResult();
}
static inline fx32 FX_Mod (fx32 numer, fx32 denom)
{
	return (fx32)FX_ModS32(numer, denom);
}

#endif // BWPORT_NITRO_FX_FX_CP_H
