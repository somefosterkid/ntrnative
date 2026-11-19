#include "global.h"

#include "particle_system_globals.h"

s32 sub_02015420(void);

s32 sub_02015420(void) {
    s32 iVar1;
    s32 iVar2;
    struct ParticleSystem **piVar3;

    iVar1 = 0;
    piVar3 = _021D10A0.systems;
    iVar2 = 0;
    do {
        if (*piVar3 != NULL) {
            iVar1 = iVar1 + 1;
        }
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 1;
    } while (iVar2 < 0x10);
    return iVar1;
}
