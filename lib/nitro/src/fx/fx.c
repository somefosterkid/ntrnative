// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/fx/src/fx.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/fx/fx.h>
#include <nitro/fx/fx_const.h>
#include <nitro/fx/fx_trig.h>

// MWCC's alone, as the SDK's nitro.h has it (__option is MWCC's preprocessor's)
#ifdef __MWERKS__
    #if !defined(_MSL_USE_INLINE) && __option(dont_inline)
        #define _MSL_USE_INLINE    0
    #endif
#endif

#include <math.h>

#define SDK_FX_INIT_2PI      (4096 * 2)
#define SDK_FX_INIT_PI_2     (4096 * 2 / 4)
#define SDK_FX_INIT_PI       (4096 * 2 / 2)

#include <nitro/code32.h>

#include <nitro/codereset.h>


void FX_Init(void) {
}

fx32 FX_Modf (fx32 x, fx32 *iPtr)
{
	if (x >= 0) {
		*iPtr = (x & FX32_INT_MASK);
		return (x & FX32_DEC_MASK);
	} else {
		*iPtr = -((-x) & FX32_INT_MASK);
		return -((-x) & FX32_DEC_MASK);
	}
}
