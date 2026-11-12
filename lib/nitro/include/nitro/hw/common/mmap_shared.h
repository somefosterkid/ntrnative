// NitroSDK's nitro/hw/common/mmap_shared.h, the main memory that the processors share.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_HW_COMMON_MMAP_SHARED_H
#define BWPORT_NITRO_HW_COMMON_MMAP_SHARED_H

// VBlank counter in shared memory, from the DSi mirror of main memory
#define HW_VBLANK_COUNT_BUF 0x02fffc3c

// The ROM header that the card's boot leaves at the end of main memory, the copy of it that the system keeps, and the
// header of a DSi game, which the system leaves in the memory the processors share
#define HW_CARD_ROM_HEADER 0x02fffa80
#define HW_CARD_ROM_HEADER_SIZE 0x160
#define HW_ROM_HEADER_BUF 0x02fffe00

#endif // BWPORT_NITRO_HW_COMMON_MMAP_SHARED_H
