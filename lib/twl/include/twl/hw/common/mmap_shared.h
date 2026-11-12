// TwlSDK's twl/hw/common/mmap_shared.h: the DSi's shared main memory.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_TWL_HW_COMMON_MMAP_SHARED_H
#define BWPORT_TWL_HW_COMMON_MMAP_SHARED_H

// The low word of the DSi's title ID, its game code, in the system area
#define HW_TWL_TITLE_ID 0x02ffe230
#define HW_TWL_ROM_HEADER_BUF 0x02ffe000

#endif // BWPORT_TWL_HW_COMMON_MMAP_SHARED_H
