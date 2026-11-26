// The PC build's prefix header (port/meson.build: -include port_prefix.h before every file's own lines): libntr's
// and libntrsystem's umbrella headers, as libntr's include/pch/nitro_pch.h has its users do (the decomp's files rely
// on names their own includes bring with our lib/nitro headers and not with libntr's), the game's types (port's
// types.h in place of include/types.h), then port_compat.h: the pokebw2 SDK spellings libntr spells otherwise or lacks.
#ifndef BWPORT_PORT_PREFIX_H
#define BWPORT_PORT_PREFIX_H

// MATHRandContext32 with the DS's layout: its u64 members aligned to 4, as MWCC aligns them (types.h does the same for
// the game's own u64 and s64, but only after <nitro.h>, whose rand.h is read first). With x86_64's and arm64's 8 the
// struct is 8-aligned and every game struct holding one is padded: BattleRecSetup 0x38 bytes where the DS has 0x34, so
// Black 2's Pokestar Studios movie (ARCID_POKEWOOD_MOVIE file 41, a PokewoodBlock with the DS's layout) was read with
// every field after it 4 bytes off: dataSize took a word of the data and MI_CpuCopy8 asserted on a 3.7 GB copy. The
// battle videos' save blocks (BattleRecBody) had the same shift. Its functions are all inline in rand.h (libntr's own
// code passes none of it across), so the game's view of the layout is the only one.
#include <nitro/types.h>
typedef unsigned long long bwport_rand_u64 __attribute__((aligned(4)));
#define u64 bwport_rand_u64
#include <nitro/math/rand.h>
#undef u64
_Static_assert(sizeof(MATHRandContext32) == 24 && _Alignof(MATHRandContext32) == 4, "MATHRandContext32: the DS's layout");
// MWCC's plain char is signed: the game is built with -fsigned-char (port/meson.build game_args), on Android arm64 too
_Static_assert((char)-1 < 0, "plain char must be signed (-fsigned-char)");

#include <nitro.h>
#include <nnsys.h>
#include "types.h"
// our nitro/os.h includes the game's forward typedefs (SOCInAddr ... that lib/nitrowifi's soc.h uses)
#include "struct_decls.h"
#include "port_compat.h"

// SDL_main.h (libntr's headers include SDL2) makes main SDL_main on Windows: the game's main is not an SDL program's.
// The game's entry, src/system/main.c's main, is the DS's NitroMain, which libntr's main() calls (port/meson.build
// builds that one file with BWPORT_ENTRY).
#ifdef main
#undef main
#endif
#ifdef BWPORT_ENTRY
#define main NitroMain
#endif

#include "port_runtime.h"

// The game's exit() is MSL's, which returns (port/src/boot_exit.c), not the host's: the heap's debug dump calls it
// where Black goes on. An object-like macro, after the host's <stdlib.h> (libntr's headers include it): the struct
// members named exit (procs' function tables) are renamed alike in every game file.
void PORT_MslExit(int status);
#define exit PORT_MslExit

#endif // BWPORT_PORT_PREFIX_H
