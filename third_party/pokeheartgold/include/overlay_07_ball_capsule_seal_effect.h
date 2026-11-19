#ifndef POKEHEARTGOLD_OVERLAY_07_BALL_CAPSULE_SEAL_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_07_BALL_CAPSULE_SEAL_EFFECT_H

#include "overlay_07.h"

#include "library/spl_emitter.h"

#include "filesystem.h"
#include "particle_system.h"
#include "seal_case.h"
#include "sys_task.h"

#define BALL_CAPSULE_SEAL_EFFECT_MAX_SEALS 9

// The ball capsule seal effect (the battle send-out particles). sizeof == 0xB8 on the DS (isActive at 0x88); the
// pointer members make it larger on a 64-bit host, so allocate it with sizeof, never with the DS literal.
struct BallCapsuleSealEffect {
    enum HeapID heapID;
    BallCapsuleConfig ballCapCfg;
    int sealCount;
    ParticleSystem *sealParticleSystems[BALL_CAPSULE_SEAL_EFFECT_MAX_SEALS];
    SEAL *seals[BALL_CAPSULE_SEAL_EFFECT_MAX_SEALS];
    BOOL sealActiveFlags[BALL_CAPSULE_SEAL_EFFECT_MAX_SEALS];
    SPLEmitter *emitter;
    SysTask *task;
    BOOL isActive;
    NARC *particleNarc;
    int state;
    int hasCapsule;
    int ballType;
    int capsuleID;
    BallCapsule capsule;
};

// The per-seal task record ov07_02232AFC allocates for each seal (0x1C bytes on the DS); the emitter callback
// (ov07_02232C64) reads it back through sub_02015504.
typedef struct BallCapsuleSealTask {
    int sealIndex;
    int delay;
    int battlerType;
    BOOL *activeFlag;
    ParticleSystem *particleSystem;
    SEAL *seal;
    void (*callback)(SPLEmitter *);
} BallCapsuleSealTask;

#endif // POKEHEARTGOLD_OVERLAY_07_BALL_CAPSULE_SEAL_EFFECT_H
