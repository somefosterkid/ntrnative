#include "global.h"
struct ParticleSystem;

#include "library/spl_emitter.h"
#include "particle_system.h"

void sub_02015528(ParticleSystem *particleSystem, int projection);

u8 sub_02015530(struct ParticleSystem *arg0);

void sub_02015528(ParticleSystem *particleSystem, int projection) {
    particleSystem->cameraProjection = projection;
}

u8 sub_02015530(ParticleSystem *arg0) {
    return arg0->cameraProjection;
}
