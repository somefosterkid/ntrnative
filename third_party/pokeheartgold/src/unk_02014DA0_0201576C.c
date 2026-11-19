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

void sub_0201576C(SPLEmitter *emitter, fx16 *force);

void sub_0201576C(SPLEmitter *emitter, fx16 *force) {
    SPLConvergenceBehavior *behavior = sub_02015550(emitter, SPL_BEHAVIOR_CONVERGENCE);
    if (behavior == 0L) {
        *force = 0;
        return;
    }

    *force = behavior->force;
}
