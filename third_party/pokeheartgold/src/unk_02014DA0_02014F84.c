#include "global.h"

#include "library/spl_emitter.h"

#include "unk_02014DA0.h"

#include "particle_system_globals.h"

void sub_02014F84(void);

void sub_02014F84(void) {
    s32 iVar1;
    struct ParticleSystem **puVar2;

    puVar2 = _021D10A0.systems;
    iVar1 = 0;
    do {
        if (*puVar2 != NULL) {
            sub_02014EBC(*puVar2);
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 0x10);
    return;
}
