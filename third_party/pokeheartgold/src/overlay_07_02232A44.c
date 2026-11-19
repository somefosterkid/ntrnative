#include "global.h"

#include "overlay_07_ball_capsule_seal_effect.h"

void ov07_02232A14(BallCapsuleSealEffect *sealEffect, void (*callback)(SPLEmitter *));
void ov07_02232C28(SPLEmitter *emitter);

void ov07_02232A44(BallCapsuleSealEffect *sealEffect);

void ov07_02232A44(BallCapsuleSealEffect *sealEffect) {
    ov07_02232A14(sealEffect, ov07_02232C28);
}
