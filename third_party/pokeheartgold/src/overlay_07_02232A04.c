#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "filesystem.h"
#include "seal_case.h"
#include "sys_task.h"
#include "pokemon_types_def.h"
#include "particle_system.h"
#include "overlay_07_ball_capsule_seal_effect.h"

struct ParticleSystem;
struct Pokemon;

BOOL ov07_02232A04(BallCapsuleSealEffect *ballCapSealEff);

BOOL ov07_02232A04(BallCapsuleSealEffect *ballCapSealEff) {
    if (ballCapSealEff->state == 0xFF) {
        return 1;
    }

    return 0;
}
