// NitroSDK's nitro/hw/ARM9/ioreg_MI.h, the memory interface's registers (the card's).
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_HW_ARM9_IOREG_MI_H
#define BWPORT_NITRO_HW_ARM9_IOREG_MI_H

#include "types.h"

// The card's SPI bus to its backup memory (and the infrared chip): control and data
#define reg_MI_MCCNT0 (*(vu16 *)0x040001a0)
#define reg_MI_MCCNT1 (*(vu32 *)0x040001a4)
#define reg_MI_MCCMD0 (*(vu32 *)0x040001a8)
#define reg_MI_MCCMD1 (*(vu32 *)0x040001ac)
#define reg_MI_MCD1 (*(vu32 *)0x04100010)
#define reg_MI_MCD0 (*(vu16 *)0x040001a2)
#define REG_MI_MCCNT0_E_MASK 0x8000
#define REG_MI_MCCNT0_I_MASK 0x4000
#define REG_MI_MCCNT0_SEL_MASK 0x2000
#define REG_MI_MCCNT0_BUSY_MASK 0x0080
#define REG_MI_MCCNT0_MODE_MASK 0x0040

// A command is running while START is set, and READY says that the card has a word for MCD1. The control word that the
// ROM header gives for the normal commands has the rest: the size of the answer (PC) and the clock cycles before it
// (L1). RESB is the card's reset line
#define REG_MI_MCCNT1_START_MASK 0x80000000
#define REG_MI_MCCNT1_READY_MASK 0x00800000
#define REG_MI_MCCNT1_PC_MASK 0x07000000
#define REG_MI_MCCNT1_PC_512B 0x01000000
#define REG_MI_MCCNT1_PC_4B 0x07000000
#define REG_MI_MCCNT1_L1_MASK 0x00001fff

#endif // BWPORT_NITRO_HW_ARM9_IOREG_MI_H
