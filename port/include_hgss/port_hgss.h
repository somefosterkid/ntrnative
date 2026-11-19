// port/include_hgss/port_hgss.h: the PC build's prefix header for Pokemon HeartGold/SoulSilver (our
// hgss-decomp, third_party/pokeheartgold, tools/pokehg_cc.py: -include port_hgss.h before every file's own lines),
// as port/include_plat/port_plat.h is for Platinum. HGSS's SDK spellings that libntr (TwlSDK 5) and lib/nnsys (Black's
// NitroSystem) spell otherwise or lack go here or, when Platinum shares them, in port/include_gen4. docs/hgss.md has
// the failure table this answers.
#ifndef BWPORT_PORT_HGSS_H
#define BWPORT_PORT_HGSS_H

#include <nitro.h>
#include <nnsys.h>
#include "nnsys_gen4.h"

// u64/s64 are 4-aligned in the game's structs, as MWCC lays them out on the DS (Pokemon 0xEC, SysInfo 0x5C with
// SysInfo_RTC's s64 at 0x24, the save blocks): port/include/types.h's typedefs, which lib/nnsys's nnsys/fnd.h includes
// inside nnsys.h above, after libntr's own headers (nitro.h: OSTick, OSAlarm keep libntr's layout). The same rule
// as Black's (port_prefix.h); this keeps the HGSS module from losing it if the prefix's order changes
_Static_assert(_Alignof(u64) == 4 && _Alignof(s64) == 4 && sizeof(u64) == 8, "u64/s64: the DS's 4-byte alignment");

// HW_DTCM is the address of libntr's SDK_AUTOLOAD_DTCM_START, cut to 32 bits by nitro/hw/X86/mmap_global.h's (u32) (twl's
// has the SDK_PORT u64 form): the whole address (poke_overlay.c's GetOverlayLoadDestination compares it with pointers)
#include <stdint.h>
#undef HW_DTCM
#define HW_DTCM ((uintptr_t)SDK_AUTOLOAD_DTCM_START)

// HGSS's V-blank waits (main.c, the intro movie scenes, the reset screen): the hooks Black's PORT_WaitIrq runs there (the
// game's frame hook, frame-timed keys, BWPORT_BENCH_SEED's pin, BWPORT_PILOT), as Platinum's port_plat.h. port/src/main_threads.c
void PortHg_WaitIrq(BOOL clear, OSIrqMask irqFlags);
#define OS_WaitIrq(clear, irqFlags) PortHg_WaitIrq(clear, irqFlags)

// HGSS's OS_ResetSystem (16 callers: main.c's L+R+START+SELECT soft reset, game_clear.c, error_message_reset.c, the Wi-Fi settings' return) is the port's in-process restart
// (port/src/reset.c, machine.c: back to the title with save.bin as it is), as port/include/port_runtime.h makes it for
// Black and White (only this one of its defines: that header is not the Gen 4 prefix). libntr's reloads the ROM
// images through 32-bit DS addresses and crashed in OSi_ReloadRomData
void PORT_ResetSystem(u32 parameter, const char *caller);
#define OS_ResetSystem(parameter) PORT_ResetSystem(parameter, __func__)

// The online side is stubbed on the PC, as Black's and Platinum's: the DWC/NitroWiFi includes resolve to the port's
// own lib/dwc and lib/nitrowifi headers, whose socket structs are untagged names (port_plat.h has the same lines)
typedef struct SOCInAddr SOCInAddr;
typedef struct SOCSockAddrIn SOCSockAddrIn;
typedef struct SOCHostEnt SOCHostEnt;
typedef struct SOCSockAddr SOCSockAddr;

// A struct's pad array sized from the DS size of its members (`u8 pad[0x148 - 4 - 3 * sizeof(T)]`) goes negative, a huge
// size_t, once they hold 8-byte pointers: PORT_PAD makes it one byte then
#define PORT_PAD(n) ((n) > 0xFFFF ? 1 : (n))

// tools/hg_assets.py's blanks (zeros the ROM fills at start) stay in plain writable data, not .bss, which an overlay load zeroes:
// `__attribute__((section(BWPORT_DATA_SECTION)))`. Mach-O names a section with its segment
#ifdef __APPLE__
#define BWPORT_DATA_SECTION "__DATA,__data"
#else
#define BWPORT_DATA_SECTION ".data"
#endif

// HGSS's SDK has a second 4 Mbit flash type; libntr's card (the save file) has the one
enum { CARD_BACKUP_TYPE_FLASH_4MBITS_EX = CARD_BACKUP_TYPE_FLASH_4MBITS };

// HGSS's spelling of the SDK's all-planes mask (libntr's GXPlaneMask has the five planes; Black's port_compat.h has this)
#ifndef GX_PLANEMASK_ALL
#define GX_PLANEMASK_ALL 0x1f
#endif

// FS_OVERLAY_ID(name): the DS's is the address of the linker's SDK_OVERLAY_<name>_ID, no u32 constant on a 64-bit host (and
// libntr's under SDK_PORT is 0); here the overlay's number from main.lsf.in (tools/pokehg_gen.py's gen/hg_overlay_ids.h).
// The C that takes the symbol's address itself gets tools/pokehg_asm.py's absolute symbols of the same numbers
#include "hg_overlay_ids.h"
#undef FS_OVERLAY_ID
#define FS_OVERLAY_ID(name) ((u32)HG_OVERLAY_##name)

// NitroSystem's texture / palette image of a .nsbtx (lib/nnsys/src/g3d/util_gen4.c): HGSS calls them with no prototype
// (certificates_app.c, register_hall_of_fame.c, unk_02025534.c), and an implicit int return cuts the pointer
const void *NNS_G3dGetTexData(const NNSG3dResTex *tex);
const void *NNS_G3dGetPlttData(const NNSG3dResTex *tex);

// The particle systems' globals are one object (include/particle_system_globals.h, upstream since bc7fa4386): the code
// that names the slot array by its own DS label _021D10A8 reads _021D10A0.systems (the header asks a native build to
// alias it). Included first so its own `extern ... _021D10A8[16];` is read before the name becomes the alias.
#include "particle_system_globals.h"
#define _021D10A8 (_021D10A0.systems)

#endif // BWPORT_PORT_HGSS_H
