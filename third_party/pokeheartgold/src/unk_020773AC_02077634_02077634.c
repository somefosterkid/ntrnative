#include "global.h"

#include "egg_hatch_particle_system.h"
#include "unk_02014DA0.h"
#include "unk_020773AC.h"

void sub_020775C4(SPLEmitter *);

void sub_02077634(void *arg, s32 res_no) {
    sub_02015494(((EggHatchParticleSystem *)arg)->ps, res_no, sub_020775C4, arg);
    sub_02015528(((EggHatchParticleSystem *)arg)->ps, 1);
}
