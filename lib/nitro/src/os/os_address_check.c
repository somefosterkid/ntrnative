// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/os/os_address_check.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK OS: address range checks (the SDK's file name is not known; this one is a guess from what the two functions
// do). Thumb code.
#include "nitro/os.h"

// Whether an address is in the extended main memory: 0x0d000000 to 0x0e000000 on a DSi (the 16 MB that its 32 MB main
// memory has beyond the DS's), 0x02400000 to 0x02800000 otherwise. Only the WM library's setup calls it
// pokebw2's OSi_IsAddressInMainMemoryEx
BOOL func_02086c70(u32 addr) {
    if (OS_IsRunOnTwl()) {
        return addr >= 0x0d000000 && addr < 0x0e000000;
    }
    return addr >= 0x02400000 && addr < 0x02800000;
}

// Whether an address is in the video memory: the palettes, VRAM and OAM, 0x05000000 to 0x07000800
// pokebw2's OSi_IsAddressInVideoMemory
BOOL func_02086cbc(u32 addr) {
    return addr >= 0x05000000 && addr < 0x07000800;
}
