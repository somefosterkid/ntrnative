#include "global.h"

#include "camera.h"
#include "particle_system.h"

Camera *sub_02015524(ParticleSystem *particleSystem);

Camera *sub_02015524(ParticleSystem *particleSystem) {
    return particleSystem->camera;
}
