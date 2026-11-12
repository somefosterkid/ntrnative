// NitroSDK's nitro/os/common/interrupt.h: the static inline OS_DisableIrq and OS_EnableIrq.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_OS_COMMON_INTERRUPT_H
#define BWPORT_NITRO_OS_COMMON_INTERRUPT_H

#include "types.h"
#include "nitro/ioreg.h"

static inline BOOL OS_DisableIrq(void) {
    u16 prev = reg_OS_IME;
    reg_OS_IME = 0;
    return prev;
}
static inline BOOL OS_EnableIrq(void) {
    u16 prev = reg_OS_IME;
    reg_OS_IME = 1;
    return prev;
}

#endif // BWPORT_NITRO_OS_COMMON_INTERRUPT_H
