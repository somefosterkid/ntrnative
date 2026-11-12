// The PC build's prefix header (port/meson.build: -include port_prefix.h before every file's own lines): libntr's
// and libntrsystem's umbrella headers, as libntr's include/pch/nitro_pch.h has its users do (the decomp's files rely
// on names their own includes bring with our lib/nitro headers and not with libntr's), the game's types (port's
// types.h in place of include/types.h), then port_compat.h: the pokebw2 SDK spellings libntr spells otherwise or lacks.
#ifndef BWPORT_PORT_PREFIX_H
#define BWPORT_PORT_PREFIX_H

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
