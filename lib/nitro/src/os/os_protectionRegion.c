// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/os/os_protectionRegion.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// NitroSDK OS: the memory protection unit's regions (os_protectionRegion.c). ARM code. Each region's register is
// reached through its own function, since only an instruction can name a coprocessor register; the functions are in
// tables that the region numbers index.
#include "nitro/os.h"

static void func_02086d14(int regionNo, u32 param);
static asm void func_02086d30(u32 param); // swan's mprotect_region0
static asm void func_02086d38(u32 param);
static asm void func_02086d40(u32 param);
static asm void func_02086d48(u32 param);
static asm void func_02086d50(u32 param);
static asm void func_02086d58(u32 param);
static asm void func_02086d60(u32 param);
static asm void func_02086d68(u32 param);
static asm u32 func_02086d70(void);
static asm u32 func_02086d78(void);
static asm u32 func_02086d80(void);
static asm u32 func_02086d88(void);
static asm u32 func_02086d90(void);
static asm u32 func_02086d98(void);
static asm u32 func_02086da0(void);
static asm u32 func_02086da8(void);

// The setters of the eight regions' registers (swan's MEMORY_PROTECTION_SET_FUNCS), then their getters (the ones for
// NitroSDK's OS_GetProtectionRegionParam, which nothing links)
// pokebw2's OSi_SetPRFuncs
static void (*data_020a86b4[8])(u32 param) = {
    func_02086d30, func_02086d38, func_02086d40, func_02086d48,
    func_02086d50, func_02086d58, func_02086d60, func_02086d68,
};
static u32 (*OSi_GetPRFuncs[8])(void) = {
    func_02086d70, func_02086d78, func_02086d80, func_02086d88,
    func_02086d90, func_02086d98, func_02086da0, func_02086da8,
};

// NitroSDK's func_02086d14: writes the register of a region (address, size and enable bit) through the
// function for it
// pokebw2's OS_SetProtectionRegionParam
static void func_02086d14(int regionNo, u32 param) {
    data_020a86b4[regionNo](param);
}

// clang-format off
// The setters and getters of coprocessor 15's registers c6,c0 to c6,c7, the regions' registers. Each is assembly in
// the SDK, since only an instruction can name a coprocessor register

// Assembly in the SDK: writes region 0's register, c6,c0
// pokebw2's OSi_SetPRParam0
static asm void func_02086d30(u32 param) {
    mcr p15, 0, r0, c6, c0, 0
    bx lr
}

// Assembly in the SDK: writes region 1's register, c6,c1
// pokebw2's OSi_SetPRParam1
static asm void func_02086d38(u32 param) {
    mcr p15, 0, r0, c6, c1, 0
    bx lr
}

// Assembly in the SDK: writes region 2's register, c6,c2
// pokebw2's OSi_SetPRParam2
static asm void func_02086d40(u32 param) {
    mcr p15, 0, r0, c6, c2, 0
    bx lr
}

// Assembly in the SDK: writes region 3's register, c6,c3
// pokebw2's OSi_SetPRParam3
static asm void func_02086d48(u32 param) {
    mcr p15, 0, r0, c6, c3, 0
    bx lr
}

// Assembly in the SDK: writes region 4's register, c6,c4
// pokebw2's OSi_SetPRParam4
static asm void func_02086d50(u32 param) {
    mcr p15, 0, r0, c6, c4, 0
    bx lr
}

// Assembly in the SDK: writes region 5's register, c6,c5
// pokebw2's OSi_SetPRParam5
static asm void func_02086d58(u32 param) {
    mcr p15, 0, r0, c6, c5, 0
    bx lr
}

// Assembly in the SDK: writes region 6's register, c6,c6
// pokebw2's OSi_SetPRParam6
static asm void func_02086d60(u32 param) {
    mcr p15, 0, r0, c6, c6, 0
    bx lr
}

// Assembly in the SDK: writes region 7's register, c6,c7
// pokebw2's OSi_SetPRParam7
static asm void func_02086d68(u32 param) {
    mcr p15, 0, r0, c6, c7, 0
    bx lr
}

// Assembly in the SDK: reads region 0's register, c6,c0
// pokebw2's OSi_GetPRParam0
static asm u32 func_02086d70(void) {
    mrc p15, 0, r0, c6, c0, 0
    bx lr
}

// Assembly in the SDK: reads region 1's register, c6,c1
// pokebw2's OSi_GetPRParam1
static asm u32 func_02086d78(void) {
    mrc p15, 0, r0, c6, c1, 0
    bx lr
}

// Assembly in the SDK: reads region 2's register, c6,c2
// pokebw2's OSi_GetPRParam2
static asm u32 func_02086d80(void) {
    mrc p15, 0, r0, c6, c2, 0
    bx lr
}

// Assembly in the SDK: reads region 3's register, c6,c3
// pokebw2's OSi_GetPRParam3
static asm u32 func_02086d88(void) {
    mrc p15, 0, r0, c6, c3, 0
    bx lr
}

// Assembly in the SDK: reads region 4's register, c6,c4
// pokebw2's OSi_GetPRParam4
static asm u32 func_02086d90(void) {
    mrc p15, 0, r0, c6, c4, 0
    bx lr
}

// Assembly in the SDK: reads region 5's register, c6,c5
// pokebw2's OSi_GetPRParam5
static asm u32 func_02086d98(void) {
    mrc p15, 0, r0, c6, c5, 0
    bx lr
}

// Assembly in the SDK: reads region 6's register, c6,c6
// pokebw2's OSi_GetPRParam6
static asm u32 func_02086da0(void) {
    mrc p15, 0, r0, c6, c6, 0
    bx lr
}

// Assembly in the SDK: reads region 7's register, c6,c7
// pokebw2's OSi_GetPRParam7
static asm u32 func_02086da8(void) {
    mrc p15, 0, r0, c6, c7, 0
    bx lr
}

void OS_SetProtectionRegion2(int regionNo, u32 address, u32 size) {
    func_02086d14(regionNo,
                                (address & (0xfffff000 << ((size - OS_PROTECTION_SIZE_4KB) >> 1))) | size | 1);
}

// clang-format on

