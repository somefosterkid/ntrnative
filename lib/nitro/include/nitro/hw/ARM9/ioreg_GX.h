// NitroSDK's nitro/hw/ARM9/ioreg_GX.h, the display's registers.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_HW_ARM9_IOREG_GX_H
#define BWPORT_NITRO_HW_ARM9_IOREG_GX_H

#include "types.h"

#define reg_GX_POWCNT (*(vu16 *)0x04000304)

// Swaps the screens, so that the main engine drives the top screen
#define REG_GX_POWCNT_DSEL_MASK 0x8000
#define REG_MASTER_BRIGHT_ADDR ((vu16 *)0x0400006c)
#define REG_GX_DISPSTAT_HBLK_MASK 0x0002
#define reg_GX_VCOUNT (*(vu16 *)0x04000006)

#endif // BWPORT_NITRO_HW_ARM9_IOREG_GX_H
