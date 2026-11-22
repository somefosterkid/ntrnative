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
// A pointer that the DS keeps in saved data (a map object's movement data, MapObjectSave.unk_30/unk_40) is a 4-byte DS address
// there. Held as a host pointer it widens the struct and writes a host address into the save: the save then differs from build
// to build (the address of libntr's s_HW_MAIN_MEM moves with the binary). Such a field is a u32 holding the DS-style address:
// the game's heaps all lie in s_HW_MAIN_MEM (OS arenas), so HW_MAIN_MEM + offset <-> 0x02000000 + offset, NULL <-> 0.
#define PORT_PLAT_DS_MAIN_MEM 0x02000000u
static inline u32 PortPlat_DsAddr(const void *p)
{
    uintptr_t a = (uintptr_t)p;

    if (p == NULL) {
        return 0;
    }
    if (a < (uintptr_t)HW_MAIN_MEM || a >= (uintptr_t)HW_MAIN_MEM + HW_MAIN_MEM_SIZE) {
        OS_Panic("PortPlat_DsAddr: %p is not in the main memory", p);
    }
    return PORT_PLAT_DS_MAIN_MEM + (u32)(a - (uintptr_t)HW_MAIN_MEM);
}

static inline void *PortPlat_HostAddr(u32 a)
{
    if (a == 0) {
        return NULL;
    }
    if (a < PORT_PLAT_DS_MAIN_MEM || a - PORT_PLAT_DS_MAIN_MEM >= HW_MAIN_MEM_SIZE) {
        OS_Panic("PortPlat_HostAddr: 0x%08x is not a main memory address", a);
    }
    return (void *)((uintptr_t)HW_MAIN_MEM + (a - PORT_PLAT_DS_MAIN_MEM));
}

#include "port_ovids.h"
#undef FS_OVERLAY_ID
#define FS_OVERLAY_ID(name) ((u32)(PORT_OVID_ ## name))

#endif // BWPORT_PORT_PLAT_H
