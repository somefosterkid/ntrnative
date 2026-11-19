#include "global.h"

#include "library/spl_emitter.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"

typedef struct SPLConvergenceBehavior {
    VecFx32 target;
    fx16 force;
    u16 padding;
} SPLConvergenceBehavior;
enum SPLBehaviorType {
    SPL_BEHAVIOR_GRAVITY = 0,
    SPL_BEHAVIOR_RANDOM,
    SPL_BEHAVIOR_MAGNET,
    SPL_BEHAVIOR_SPIN,
    SPL_BEHAVIOR_COLLISION_PLANE,
    SPL_BEHAVIOR_CONVERGENCE,
};
const void *sub_02015550(SPLEmitter *emitter, int behaviorType);

void sub_02015720(SPLEmitter *emitter, VecFx32 *target);

void sub_02015720(SPLEmitter *emitter, VecFx32 *target) {
    SPLConvergenceBehavior *behavior = sub_02015550(emitter, SPL_BEHAVIOR_CONVERGENCE);
    if (behavior == 0L) {
        *target = (VecFx32) { 0, 0, 0 };
        return;
    }

    *target = behavior->target;
}
