#include "global.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"

struct SPLConvergenceBehavior;

struct SPLEmitter;

struct SPLEmitterState;

struct SPLParticle;

struct SPLParticleList;

struct SPLResource;

enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};

typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);

typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;

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

void sub_02015708(SPLEmitter *emitter, VecFx32 *target);

void sub_02015708(SPLEmitter *emitter, VecFx32 *target) {
    SPLConvergenceBehavior *behavior = sub_02015550(emitter, SPL_BEHAVIOR_CONVERGENCE);
    if (behavior == NULL) {
        return;
    }

    behavior->target = *target;
}
