// Platinum's overlay static initializers on PC (shadows libntr's nitro/sinit.h, which has the CodeWarrior .sinit table only).
// On the DS each overlay's .sinit table is run by FS_StartOverlay after the overlay is loaded: most are one overlay loading the
// overlays it needs, but every Poketch app's (src/applications/poketch/*/main.c) is what hands PoketchSystem its New / Exit
// functions (PoketchSystem_SetAppFunctions), so without it the first app start calls a NULL. Here the file's NitroStaticInit
// registers itself from a constructor with the address of a zeroed static of the file (which the linker script puts into that
// overlay's .ovlbss: port/src/ovl.c PortOvl_Owner finds the overlay), and port/src/ovl.c FS_StartOverlay calls those of the
// overlay it starts.
#ifndef BWPORT_PLAT_NITRO_SINIT_H
#define BWPORT_PLAT_NITRO_SINIT_H

void PortOvl_RegisterSinit(void (*fn)(void), const void *anchor);

static void NitroStaticInit(void);
static char NitroStaticInit_anchor;
__attribute__((constructor, used)) static void NitroStaticInit_register(void)
{
    PortOvl_RegisterSinit(NitroStaticInit, &NitroStaticInit_anchor);
}

#endif
