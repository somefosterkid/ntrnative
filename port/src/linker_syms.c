// The DS link's linker-script symbols on PC (docs/port-build.md, The link). The ROM build's objects reference 162 that
// Black's linker script defines (the maintainers' notes); the PC build needs a definition of none of them:
// - OVERLAY_<n>_ID (146), the overlay numbers as addresses: include/gfl/overlay.h's OVERLAY_ID(n) is the number
//   itself on a PC compiler, so nothing references them;
// - SDK_AUTOLOAD_DTCM_START (the game's HW_DTCM, 18 files): libntr defines it (libraries/sim/src/sim_vars.c);
// - the other 15 (ARM9_BSS_START/END, ARM9_DATA_END, ARM9_CTOR_START, SDK_AUTOLOAD_LIST..., SDK_LTDAUTOLOAD_*,
//   SDK_IRQ_STACKSIZE, SDK_SYS_STACKSIZE, SDK_SECTION_ARENA_*, SDK_OVERLAY_DIGEST_END, SDK_OVERLAY_NUMBER): only the
//   crt0, OS and FS of lib/nitro and MSL's runtime use them, which the PC build replaces with libntr.
// The overlay table (SDK_OVERLAY_*, FS_LoadOverlay) is port/src/ovl.c's.
//
// What is left are the arena bounds. libntr defines the linker's arena symbols on PC as empty functions
// (SDK_SECTION_ARENA_DTCM_START, SDK_SECTION_ARENA_ITCM_START, SDK_IRQ_STACKSIZE, SDK_SYS_STACKSIZE in
// libraries/os/src/os_thread.c and os_arena.c) and takes their addresses, cut to 32 bits, as the bounds: the DTCM
// arena starts on a function's code and ends wherever the stack sizes' "values" put it, and the ITCM arena ends at
// the DS's 0x02000000. lib/gfl/src/heap_dtcm.c builds an OS heap at the DTCM arena's start (GFLAppInit's
// GFL_HeapDTCMInit(0x2000), the first thing after SystemInit; GFL_G3DSys takes the geometry buffer from it), which
// writes into .text. Here both arenas get memory of the DS's sizes before NitroMain: OS_InitArena (libntr's, which
// OS_Init calls again and then returns at once) and then their bounds set to the buffers.
#include <nitro.h>

#define PORT_DTCM_SIZE 0x4000 // HW_DTCM_SIZE
#define PORT_ITCM_SIZE 0x8000 // HW_ITCM_SIZE

// The DS's TCMs as RAM: aligned as the 32-byte cache lines of the DS's (the game's DMA and cache calls)
static u8 sPortDtcm[PORT_DTCM_SIZE] __attribute__((aligned(32)));
static u8 sPortItcm[PORT_ITCM_SIZE] __attribute__((aligned(32)));

void *PortArena_Dtcm(void) {
    return sPortDtcm;
}

__attribute__((constructor)) static void PortArena_Init(void) {
    OS_InitArena();
    OS_SetArenaLo(OS_ARENA_DTCM, sPortDtcm);
    OS_SetArenaHi(OS_ARENA_DTCM, sPortDtcm + PORT_DTCM_SIZE);
    OS_SetArenaLo(OS_ARENA_ITCM, sPortItcm);
    OS_SetArenaHi(OS_ARENA_ITCM, sPortItcm + PORT_ITCM_SIZE);
}
