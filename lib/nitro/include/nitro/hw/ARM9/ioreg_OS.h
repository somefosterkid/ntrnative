// NitroSDK's nitro/hw/ARM9/ioreg_OS.h, the OS's registers (the interrupts, the timers).
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_HW_ARM9_IOREG_OS_H
#define BWPORT_NITRO_HW_ARM9_IOREG_OS_H

#include "types.h"

#define reg_OS_IME (*(vu16 *)0x04000208)
#define reg_OS_IE (*(vu32 *)0x04000210)

// The interrupt request flags
#define reg_OS_IF (*(vu32 *)0x04000214)

// The counters and controls of the timers that the OS uses, the system timer (0) and the alarms' (1). A control
// register's bits: running, interrupting when it wraps and counting every 64 cycles
#define reg_OS_TM0CNT_L (*(vu16 *)0x04000100)
#define reg_OS_TM1CNT_L (*(vu16 *)0x04000104)

#endif // BWPORT_NITRO_HW_ARM9_IOREG_OS_H
