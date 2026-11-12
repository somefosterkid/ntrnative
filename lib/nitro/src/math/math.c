// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/math/src/math.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/math.h>

    #pragma thumb off

    #pragma thumb reset

u8 MATH_CountPopulation (u32 x)
{
	x -= ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x += (x >> 4);

	x &= 0x0f0f0f0f;

	x += (x >> 8);
	x += (x >> 16);

	return (u8)x;
}
