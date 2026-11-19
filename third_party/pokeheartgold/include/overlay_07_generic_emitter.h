#ifndef POKEHEARTGOLD_OVERLAY_07_GENERIC_EMITTER_H
#define POKEHEARTGOLD_OVERLAY_07_GENERIC_EMITTER_H

#include "global.h"

struct BattleAnimSystem;
struct ParticleSystem;

enum GenericEmitterCallbackParam {
    GENERIC_EMITTER_CALLBACK_PARAM_PRIORITY = 0,
    GENERIC_EMITTER_CALLBACK_PARAM_TARGET_MODE,
    GENERIC_EMITTER_CALLBACK_PARAM_POSITION,
    GENERIC_EMITTER_CALLBACK_PARAM_AXIS,
    GENERIC_EMITTER_CALLBACK_PARAM_BEHAVIOR,
    GENERIC_EMITTER_CALLBACK_PARAM_CAMERA,

    GENERIC_EMITTER_CALLBACK_PARAM_COUNT
};

// Per-emitter data of the generic battle particle emitter callbacks (ov07_022215B4 and friends).
// sizeof(GenericEmitterCallbackData) == 0x38 on the DS.
typedef struct GenericEmitterCallbackData {
    struct BattleAnimSystem *battleAnimSys;
    struct ParticleSystem *particleSys;
    s8 dir;
    int params[GENERIC_EMITTER_CALLBACK_PARAM_COUNT];
    int startBattler;
    int endBattler;
    VecFx32 position;
} GenericEmitterCallbackData;

#endif // POKEHEARTGOLD_OVERLAY_07_GENERIC_EMITTER_H
