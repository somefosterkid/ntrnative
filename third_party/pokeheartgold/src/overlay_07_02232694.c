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




BallCapsuleSealEffect *ov07_02232670(enum HeapID heapID);

void ov07_022326C0(BallCapsuleSealEffect *ballCapSealEff, const BallCapsuleConfig *ballCapCfg);


BallCapsuleSealEffect *ov07_02232694(enum HeapID heapID, const BallCapsuleConfig *ballCapCfg);

u32 GetMonData(Pokemon *mon, int param, void *dest);
void ov07_022326C0(BallCapsuleSealEffect *ballCapSealEff, const BallCapsuleConfig *ballCapCfgPtr);

BallCapsuleSealEffect *ov07_02232694(enum HeapID heapID, const BallCapsuleConfig *ballCapCfg) {
    BallCapsuleSealEffect *ballCapSealEff = NULL;

    ballCapSealEff = ov07_02232670(heapID);
    ov07_022326C0(ballCapSealEff, ballCapCfg);

    {
        int v1;

        ballCapSealEff->sealCount = 0;

        for (v1 = 0; v1 < 9; v1++) {
            ballCapSealEff->sealParticleSystems[v1] = NULL;
            ballCapSealEff->seals[v1] = NULL;
            ballCapSealEff->sealActiveFlags[v1] = 0;
        }
    }

    return ballCapSealEff;
}

void ov07_022326C0(BallCapsuleSealEffect *ballCapSealEff, const BallCapsuleConfig *ballCapCfgPtr) {
    ballCapSealEff->ballCapCfg = *ballCapCfgPtr;
    ballCapSealEff->capsuleID = GetMonData(ballCapSealEff->ballCapCfg.mon, MON_DATA_BALL_CAPSULE_ID, 0L);

    ((ballCapSealEff->capsuleID < 12 + 1) ? (void)0 : GF_AssertFail());

    if (ballCapSealEff->capsuleID == 0) {
        ballCapSealEff->hasCapsule = 0;

        if (ballCapSealEff->ballCapCfg.ballTypeOverride != 0) {
            ballCapSealEff->ballType = ballCapSealEff->ballCapCfg.ballTypeOverride;
        } else {
            ballCapSealEff->ballType = GetMonData(ballCapSealEff->ballCapCfg.mon, MON_DATA_POKEBALL, 0L);
        }
    } else {
        ballCapSealEff->hasCapsule = 1;
        GetMonData(ballCapSealEff->ballCapCfg.mon, MON_DATA_BALL_CAPSULE, &ballCapSealEff->capsule);
    }
}
