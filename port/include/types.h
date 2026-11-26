// The PC build's include/types.h (port/include is first on the include path, and port_prefix.h includes this one
// before any game header: its guard is the game's, so the game's include/types.h is skipped). The integer types are
// libntr's (nitro/types.h): on win64 its u32 and s32 are unsigned long and long (32 bits there), which the game's
// `typedef unsigned int u32` would contradict. The rest is the game's types.h, for a PC target.
#ifndef BWPORT_TYPES_H
#define BWPORT_TYPES_H

#include <nitro/types.h>

// The game's 64-bit integers aligned to 4 in its structs, as MWCC aligns them on the DS (and include/types.h for the
// tools' clang): a save block with an s64 at a 4-byte boundary (TimeSigSave, AdventureTime, TrainerGameInfoSave) has
// the DS's layout and size, where x86_64's 8 made the save blocks overrun the DS's 0x24000-byte save area. Names
// only from here on: libntr's and libntrsystem's headers, which port_prefix.h includes before this one, keep
// theirs (none of the few it doesn't include, with a 64-bit member, is a game include).
typedef unsigned long long bwport_u64 __attribute__((aligned(4)));
typedef signed long long bwport_s64 __attribute__((aligned(4)));
#define u64 bwport_u64
#define s64 bwport_s64

// NitroSDK's u32 by its own type (include/types.h): 32 bits on a PC target, where Linux's unsigned long is 64
typedef u32 sdk_u32;

// libntr has f32 and not f64
typedef double f64;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

// MWCC's intrinsic memcpy, which the C calls by name (dpw, GameSpy, poke_net)
#define __memcpy __builtin_memcpy

// uintptr_t and intptr_t, for a pointer that carries a number and back
#include <stdint.h>

// Widths the DS's C relies on, checked once here for every game file (the PC targets are 64-bit: win64 LLP64, Linux, Mac,
// Android and iOS LP64): the integer types are libntr's (nitro/types.h asserts them too, before the u64/s64 aligned
// redefinition above), a pointer is 8 bytes, and a plain char is signed as MWCC's is (-fsigned-char: Android arm64's
// default is unsigned; the same assert is in port_prefix.h for the compiled-in setting)
#ifdef __cplusplus
static_assert(sizeof(u32) == 4 && sizeof(s32) == 4 && sizeof(u64) == 8 && sizeof(s64) == 8 && sizeof(void *) == 8, "PC target widths");
#else
_Static_assert(sizeof(u32) == 4 && sizeof(s32) == 4 && sizeof(u64) == 8 && sizeof(s64) == 8 && sizeof(void *) == 8, "PC target widths");
#endif

#define NELEMS(array) (sizeof(array) / sizeof((array)[0]))

#ifndef ATTRIBUTE_ALIGN
#define ATTRIBUTE_ALIGN(num) __attribute__((aligned(num)))
#endif

#endif // BWPORT_TYPES_H
