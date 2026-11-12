// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/types.h.
#ifndef BWPORT_TYPES_H
#define BWPORT_TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#ifdef __clang__
// The tools' clang (struct layouts for brief.py, check_layouts.py) aligns a 64-bit integer to 8 in a struct, as the
// ARM EABI does; MWCC aligns it to 4 (GdsRapWork's union at 0x3a8 puts `response` at 0x3ac). MWCC never sees this.
typedef unsigned long long u64 __attribute__((aligned(4)));
#else
typedef unsigned long long u64;
#endif
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
#ifdef __clang__
typedef signed long long s64 __attribute__((aligned(4)));
#else
typedef signed long long s64;
#endif

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float f32;
typedef double f64;

// NitroSDK's u32 as its nitro/types.h has it, unsigned long, which MWCC indexes with otherwise than unsigned int (32
// bits on the DS; a PC target's types.h makes it 32 bits there too)
typedef unsigned long sdk_u32;

// NitroSDK's types for the hardware registers (NitroSDK has them in nitro/types.h, whose other types are these above;
// a header of ours by that name would answer for this one in lib/nitro/include/nitro's own #include "types.h")
typedef u8 REGType8;
typedef u16 REGType16;
typedef u32 REGType32;
typedef u64 REGType64;
typedef vu8 REGType8v;
typedef vu16 REGType16v;
typedef vu32 REGType32v;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define NULL ((void *)0)

#ifndef __MWERKS__
// MWCC's intrinsic memcpy, which builds a copy of a constant size in place and needs no declaration (the C calls it
// by name: dpw, GameSpy, poke_net): another compiler's own builtin
#define __memcpy __builtin_memcpy
#endif

#if !defined(__MWERKS__) && !defined(__INTELLISENSE__) && !defined(__arm__)
// A PC target (the port): the host's uintptr_t and intptr_t, for a pointer that carries a number (an OSMessage that
// is a result code) and back. MWCC and the tools' clang for ARM take MSL's stddef.h's (nitro/os.h includes it)
#include <stdint.h>
#endif

#define NELEMS(array) (sizeof(array) / sizeof((array)[0]))

// NitroSDK's alignment attribute, as in `static u8 buffer[64] ATTRIBUTE_ALIGN(32);`
#define ATTRIBUTE_ALIGN(num) __attribute__((aligned(num)))

#endif // BWPORT_TYPES_H
