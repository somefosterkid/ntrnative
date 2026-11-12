// The PC build's overlay model (port/src/ovl.c, tools/port_overlays.py): helpers for debugging; the game itself goes
// through the SDK's FS_LoadOverlay and FS_UnloadOverlay (lib/gfl/src/gf_overlay.c).
#ifndef BWPORT_PORT_OVL_H
#define BWPORT_PORT_OVL_H

#include <nitro/types.h>

// Takes the .data snapshot (once; a constructor calls it before NitroMain)
void PortOvl_Init(void);
// Whether FS_LoadOverlay loaded the overlay and no FS_UnloadOverlay came after
BOOL PortOvl_IsLoaded(u32 id);
// The overlay whose .data or .bss holds the address, or -1 (from gdb: `p PortOvl_Owner(ptr)`)
int PortOvl_Owner(const void *p);
// The overlays in the table (0 without tools/port_overlays.py's script)
u32 PortOvl_Count(void);

#endif // BWPORT_PORT_OVL_H
