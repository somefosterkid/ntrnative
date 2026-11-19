// port/include_plat/port_plat.h: the PC build's prefix header for Pokemon Platinum (pret pokeplatinum's C,
// third_party/pokeplatinum, tools/pokeplat_cc.py: -include port_plat.h before every file's own lines). Platinum's
// SDK spellings that libntr (TwlSDK 5) and lib/nnsys (Black's NitroSystem) spell otherwise or lack go here, as
// port/include_b2w2/port_b2w2.h does for pokebw2. docs/platinum.md has the failure table this answers.
#ifndef BWPORT_PORT_PLAT_H
#define BWPORT_PORT_PLAT_H

#include <nitro.h>
#include <nnsys.h>
#include "nnsys_gen4.h"

// Platinum's V-blank waits: the hooks Black's PORT_WaitIrq runs there (BWPORT_BENCH_SEED's pin, BWPORT_PILOT's pilot before the
// field), without the game lock (Platinum's module has no PORT_ thread model: port_runtime.h is not its prefix). port/src/main_threads.c
void PortPlat_WaitIrq(BOOL clear, OSIrqMask irqFlags);
#define OS_WaitIrq(clear, irqFlags) PortPlat_WaitIrq(clear, irqFlags)

// The game's resets (the end of the credits, the soft reset, an error reset) go to the port's (port/src/reset.c: the game starts
// again from its boot in this process, as Black's): libntr's own OS_ResetSystem reloads the ROM through DS addresses and crashed
// (SIGSEGV in OSi_ReloadRomData after the Hall of Fame)
void PORT_ResetSystem(u32 parameter, const char *caller);
#define OS_ResetSystem(parameter) PORT_ResetSystem(parameter, __func__)

// The online side is stubbed on the PC, as Black's: pret's DWC/NitroWiFi includes resolve to the port's own
// lib/dwc and lib/nitrowifi headers (tools/pokeplat_cc.py), whose socket structs are C++-style untagged names
typedef struct SOCInAddr SOCInAddr;
typedef struct SOCSockAddrIn SOCSockAddrIn;
typedef struct SOCHostEnt SOCHostEnt;
typedef struct SOCSockAddr SOCSockAddr;


// pret's prefix header (meson.build c_pch: include/pch/global_pch.h): GF_ASSERT, ALIGN_4, NELEMS, the game config
#include "pch/global_pch.h"
// its two pointer-as-u32 macros, at the host's pointer size
#undef ALIGN_PTR
#define ALIGN_PTR(ptr, alignment) ((void *)(((uintptr_t)(ptr) + (alignment - 1)) / (alignment) * (alignment)))
#undef XtOffset
#define XtOffset(pointer_type, field) ((unsigned int)(uintptr_t)&(((pointer_type)NULL)->field))
// pret's FS_OVERLAY_ID(name): libntr's SDK_PORT turns it into (0) (Black names overlays by number), which made every
// Overlay_LoadByID load overlay 0 (none was reset or started: a Poketch app's NitroStaticInit never ran). Here it is the
// overlay's id as a constant (PORT_OVID_<name>: tools/port_overlays.py plat_id_symbols writes port_ovids.h).
#include "port_ovids.h"
#undef FS_OVERLAY_ID
#define FS_OVERLAY_ID(name) ((u32)(PORT_OVID_ ## name))

#endif // BWPORT_PORT_PLAT_H
