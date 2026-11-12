// Black's code at main 0x02004000-0x02004800: NitroSDK's system call library (libsyscall.a) as Black has it, the 2 KB
// at the start of ARM9 main that the card's secure area holds (pokebw2's lib/nitro/src/syscall/secure_area.c). Nintendo
// gives each game its own build of the library: the BIOS call stubs (Thumb, each one SWI; the SDK wrote them in
// assembly, so they are MWCC asm functions here) lie at places of their own among filler bytes, so Black, Black 2 and
// White 2 have them in different orders. The file name is descriptive, since the library's object name isn't known.
// The names are Black's (dsd's, from the GBA BIOS's), with NitroSDK's in the comments.
//
// The file isn't linked: the filler bytes are the base ROM's, which no source gives and which aren't written here, and
// MWCC starts each function at a multiple of 4 bytes, where most of the stubs start 2 bytes past one.
#include "nitro/os.h"
#include "nitro/os/common/systemCall.h"

#pragma thumb on

// clang-format off
// NitroSDK's SVC_UncompressLZ16FromDevice: BIOS call 0x12 (0x0200407e)
asm s32 LZ77UnCompReadByCallbackWrite16bit(const void *src, void *dest, const void *param, const struct MIReadStreamCallbacks *callbacks) {
    swi 0x12
    bx lr
}

// NitroSDK's SVC_Halt: BIOS call 0x06 (0x020040e2)
asm void Halt(void) {
    swi 0x6
    bx lr
}

// NitroSDK's SVC_CpuSetFast: BIOS call 0x0c (0x02004136)
asm void CpuFastSet(const void *src, void *dest, u32 mode) {
    swi 0xc
    bx lr
}

// NitroSDK's SVC_WaitIntr: BIOS call 0x04 (0x020041b2)
asm void IntrWait(BOOL clear, u32 irqFlags) {
    swi 0x4
    bx lr
}

// NitroSDK's SVC_CpuSet: BIOS call 0x0b (0x0200421a)
asm void CpuSet(const void *src, void *dest, u32 mode) {
    swi 0xb
    bx lr
}

// NitroSDK's SVC_IsMmemExpanded: BIOS call 0x0f (0x02004278)
asm BOOL IsDebugger(void) {
    swi 0xf
    bx lr
}

// NitroSDK's SVC_Div: BIOS call 0x09, which leaves the quotient in r0 (0x020042e2)
asm s32 Div(s32 number, s32 denom) {
    swi 0x9
    bx lr
}

// NitroSDK's SVC_DivRem: BIOS call 0x09, returning the remainder that it leaves in r1 (0x02004366)
asm s32 Mod(s32 number, s32 denom) {
    swi 0x9
    mov r0, r1
    bx lr
}

// NitroSDK's SVC_UnpackBits: BIOS call 0x10 (0x020043d0)
asm void BitUnPack(const void *src, void *dest, const struct MIUnpackBitsParam *param) {
    swi 0x10
    bx lr
}

// NitroSDK's SVC_SoftReset: BIOS call 0x00 (0x02004434)
asm void SoftReset(void) {
    swi 0x0
    bx lr
}

// NitroSDK's SVC_WaitByLoop: BIOS call 0x03 (0x02004490)
asm void SVC_WaitByLoop(u32 cycles) {
    swi 0x3
    bx lr
}

// NitroSDK's SVC_UncompressLZ8: BIOS call 0x11 (0x020044fe)
asm void LZ77UnCompReadNormalWrite8bit(const void *src, void *dest) {
    swi 0x11
    bx lr
}

// NitroSDK's SVC_UncompressHuffmanFromDevice: BIOS call 0x13 (0x02004560)
asm s32 HuffUnCompReadByCallback(const void *src, void *dest, u8 *tree, const struct MIReadStreamCallbacks *callbacks) {
    swi 0x13
    bx lr
}

// NitroSDK's SVC_UncompressRL16FromDevice: BIOS call 0x15 (0x020045cc)
asm s32 RLUnCompReadByCallbackWrite16bit(const void *src, void *dest, const void *param, const struct MIReadStreamCallbacks *callbacks) {
    swi 0x15
    bx lr
}

// NitroSDK's SVC_GetCRC16: BIOS call 0x0e (0x02004632)
asm u16 GetCRC16(u32 init, void *data, u32 size) {
    swi 0xe
    bx lr
}

// NitroSDK's SVC_UncompressRL8: BIOS call 0x14 (0x02004694)
asm void RLUnCompReadNormalWrite8bit(const void *src, void *dest) {
    swi 0x14
    bx lr
}

// NitroSDK's SVC_Sqrt: BIOS call 0x0d (0x0200471c)
asm u16 Sqrt(u32 value) {
    swi 0xd
    bx lr
}

// NitroSDK's SVC_WaitVBlankIntr: BIOS call 0x05, with r2 = 0 (0x020047a4)
asm void VBlankIntrWait(void) {
    mov r2, #0
    swi 0x5
    bx lr
}

// clang-format on

#pragma thumb off
