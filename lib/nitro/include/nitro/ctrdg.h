// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/ctrdg.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_CTRDG_H
#define BWPORT_NITRO_CTRDG_H

#include "types.h"

#define HW_CTRDG_MODULE_INFO_BUF 0x02fffc30

typedef struct {
    u16 romCheck; // The halfword that the ROM had at the address that the library checks
    u8 unk_02[3];
    u8 isAgbCartridge : 1; // The ROM has the header of a GBA cartridge, so its game code can be compared
    u8 pulledOut : 1;      // The library found that the cartridge was pulled out or exchanged
    u8 unk_05 : 6;
    u8 unk_06[2];
    u32 gameCode; // The cartridge's game code
} CTRDGModuleInfo;

#define CTRDG_MODULE_INFO ((CTRDGModuleInfo *)HW_CTRDG_MODULE_INFO_BUF)

#endif // BWPORT_NITRO_CTRDG_H
