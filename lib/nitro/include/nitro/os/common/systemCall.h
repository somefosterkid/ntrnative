// NitroSDK's nitro/os/common/systemCall.h, the BIOS calls.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/svc.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_OS_COMMON_SYSTEMCALL_H
#define BWPORT_NITRO_OS_COMMON_SYSTEMCALL_H

#include "types.h"

// SVC_CpuSet: copies or fills the words or halfwords the mode gives (the count in its low bits, the flags below), from
// src to dest. A fill reads src once
#define SVC_CPUSET_FILL 0x01000000

#define SVC_CPUSET_32BIT 0x04000000

// pokebw2's SVC_GetCRC16
// SVC_GetCRC16: the BIOS's CRC-16 of the data, from the first value of the sum
u16 GetCRC16(u32 init, void *data, u32 size);

// The other BIOS call stubs of the secure area (lib/nitro/src/syscall/secure_area.c), under Black's names, with
// NitroSDK's in the comments. CpuSet and SVC_WaitByLoop are in nitro/os.h
struct MIUnpackBitsParam;
struct MIReadStreamCallbacks;

// SVC_UncompressLZ16FromDevice
s32 LZ77UnCompReadByCallbackWrite16bit(const void *src, void *dest, const void *param,
                                       const struct MIReadStreamCallbacks *callbacks);
// SVC_Halt
void Halt(void);
// SVC_CpuSetFast
void CpuFastSet(const void *src, void *dest, u32 mode);
// SVC_WaitIntr
void IntrWait(BOOL clear, u32 irqFlags);
// SVC_IsMmemExpanded
BOOL IsDebugger(void);
// SVC_Div
s32 Div(s32 number, s32 denom);
// SVC_DivRem
s32 Mod(s32 number, s32 denom);
// SVC_UnpackBits
void BitUnPack(const void *src, void *dest, const struct MIUnpackBitsParam *param);
// SVC_SoftReset
void SoftReset(void);
// SVC_UncompressLZ8
void LZ77UnCompReadNormalWrite8bit(const void *src, void *dest);
// SVC_UncompressHuffmanFromDevice
s32 HuffUnCompReadByCallback(const void *src, void *dest, u8 *tree, const struct MIReadStreamCallbacks *callbacks);
// SVC_UncompressRL16FromDevice
s32 RLUnCompReadByCallbackWrite16bit(const void *src, void *dest, const void *param,
                                     const struct MIReadStreamCallbacks *callbacks);
// SVC_UncompressRL8
void RLUnCompReadNormalWrite8bit(const void *src, void *dest);
// SVC_Sqrt
u16 Sqrt(u32 value);
// SVC_WaitVBlankIntr
void VBlankIntrWait(void);

#endif // BWPORT_NITRO_OS_COMMON_SYSTEMCALL_H
