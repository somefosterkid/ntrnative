#include "global.h"

#include "unk_02014DA0.h"

#include "particle_system_globals.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/unk_02014DA0.s: 0x48 bytes on the DS, 4-aligned; its slot array
// would run past it with 8-byte pointers and sit misaligned). The asm label stays for the DS build.
ParticleSystemGlobals _021D10A0;
#endif

void sub_02014DA0(void) {
    s32 iVar1;
    struct ParticleSystem **puVar2;

    iVar1 = 0;
    puVar2 = _021D10A0.systems;
    do {
        iVar1 = iVar1 + 1;
        *puVar2 = NULL;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 0x10);
    return;
}
