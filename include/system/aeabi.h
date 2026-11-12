// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/aeabi.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_AEABI_H
#define BWPORT_SYSTEM_AEABI_H

#include "types.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Quotient is returned in r0 and remainder in r1. The u64 declaration exposes both registers to C.
u64 _u32_div_f(u32 dividend, u32 divisor);
// The quotient is returned in r0:r1 and the remainder in r2:r3
u64 _ll_udiv(u64 dividend, u64 divisor);
// Quotient is returned in r0 and remainder in r1. The u64 declaration exposes both registers to C.
u64 __aeabi_uidivmod(u32 dividend, u32 divisor);

#endif // BWPORT_SYSTEM_AEABI_H
