#include "global.h"

#include "library/spl_emitter.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"

typedef struct SPLMagnetBehavior {
    VecFx32 target;
    fx16 force;
    u16 padding;
} SPLMagnetBehavior;
enum SPLBehaviorType {
    SPL_BEHAVIOR_GRAVITY = 0,
    SPL_BEHAVIOR_RANDOM,
    SPL_BEHAVIOR_MAGNET,
    SPL_BEHAVIOR_SPIN,
    SPL_BEHAVIOR_COLLISION_PLANE,
    SPL_BEHAVIOR_CONVERGENCE,
};
const void *sub_02015550(SPLEmitter *emitter, int behaviorType);

void sub_02015640(SPLEmitter *emitter, VecFx32 *target);

void sub_02015640(SPLEmitter *emitter, VecFx32 *target) {
    SPLMagnetBehavior *behavior = sub_02015550(emitter, SPL_BEHAVIOR_MAGNET);
    if (behavior == 0L) {
        *target = (VecFx32) { 0, 0, 0 };
        return;
    }

    *target = behavior->target;
}
