#include "global.h"

#include "wild_encounter.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "filesystem.h"
#include "sys_task.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "camera.h"
#include "particle_system.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "overlay_07_ball_capsule_seal_effect.h"

struct BallCapsule;



struct BoxPokemon;

struct CameraPerspective;

struct CameraPositionHistory;

struct MailMessage;

struct Mail;

struct NARC;

struct ParticleSystem;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct SPLAlphaAnim;

struct SPLBehavior;

struct SPLChildResource;

struct SPLColorAnim;

struct SPLEmitter;

struct SPLEmitterState;

struct SPLParticle;

struct SPLResourceHeader;

struct SPLScaleAnim;

struct SPLTexAnim;

struct SysTask;

struct SysTaskQueue;


enum Seal {
    SEAL_DUMMY = 0,
    HEART_SEAL_A = 1,
    HEART_SEAL_B = 2,
    HEART_SEAL_C = 3,
    HEART_SEAL_D = 4,
    HEART_SEAL_E = 5,
    HEART_SEAL_F = 6,
    STAR_SEAL_A = 7,
    STAR_SEAL_B = 8,
    STAR_SEAL_C = 9,
    STAR_SEAL_D = 10,
    STAR_SEAL_E = 11,
    STAR_SEAL_F = 12,
    LINE_SEAL_A = 13,
    LINE_SEAL_B = 14,
    LINE_SEAL_C = 15,
    LINE_SEAL_D = 16,
    SMOKE_SEAL_A = 17,
    SMOKE_SEAL_B = 18,
    SMOKE_SEAL_C = 19,
    SMOKE_SEAL_D = 20,
    ELE_SEAL_A = 21,
    ELE_SEAL_B = 22,
    ELE_SEAL_C = 23,
    ELE_SEAL_D = 24,
    FOAMY_SEAL_A = 25,
    FOAMY_SEAL_B = 26,
    FOAMY_SEAL_C = 27,
    FOAMY_SEAL_D = 28,
    FIRE_SEAL_A = 29,
    FIRE_SEAL_B = 30,
    FIRE_SEAL_C = 31,
    FIRE_SEAL_D = 32,
    PARTY_SEAL_A = 33,
    PARTY_SEAL_B = 34,
    PARTY_SEAL_C = 35,
    PARTY_SEAL_D = 36,
    FLORA_SEAL_A = 37,
    FLORA_SEAL_B = 38,
    FLORA_SEAL_C = 39,
    FLORA_SEAL_D = 40,
    FLORA_SEAL_E = 41,
    FLORA_SEAL_F = 42,
    SONG_SEAL_A = 43,
    SONG_SEAL_B = 44,
    SONG_SEAL_C = 45,
    SONG_SEAL_D = 46,
    SONG_SEAL_E = 47,
    SONG_SEAL_F = 48,
    SONG_SEAL_G = 49,
    A_SEAL = 50,
    B_SEAL = 51,
    C_SEAL = 52,
    D_SEAL = 53,
    E_SEAL = 54,
    F_SEAL = 55,
    G_SEAL = 56,
    H_SEAL = 57,
    I_SEAL = 58,
    J_SEAL = 59,
    K_SEAL = 60,
    L_SEAL = 61,
    M_SEAL = 62,
    N_SEAL = 63,
    O_SEAL = 64,
    P_SEAL = 65,
    Q_SEAL = 66,
    R_SEAL = 67,
    S_SEAL = 68,
    T_SEAL = 69,
    U_SEAL = 70,
    V_SEAL = 71,
    W_SEAL = 72,
    X_SEAL = 73,
    Y_SEAL = 74,
    Z_SEAL = 75,
    SHOCK_SEAL = 76,
    MYSTERY_SEAL = 77,
    LIQUID_SEAL = 78,
    BURST_SEAL = 79,
    TWINKLE_SEAL = 80,
    SEAL_ID_MAX = 81,
};

typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;

typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;

        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;

typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;

typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;

typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;

typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;

    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;

typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;

typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;

typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;

typedef struct SPLTexAnim {
    u8 textures[8];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;

typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;

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

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/application/capsule_menu/main_static.c (tools/pokehg_abi.py)
u8 CapsuleMenu_GetSealParticleIndex(u32 index);
#else
u8 CapsuleMenu_GetSealParticleIndex(u8 index);
#endif

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

ParticleSystem *ov07_0221FEE4(NARC *narc, enum HeapID heapID, int memberIdx, BOOL uploadImmediately);

SEAL *CapsuleGetSealI(BallCapsule *ballCapsule, int sealNum);


void SysTask_Destroy(SysTask *task);


BallCapsuleSealEffect *ov12_02235FE0(enum HeapID heapID);

BallCapsuleSealEffect *ov12_02236004(enum HeapID heapID, const BallCapsuleConfig *ballCapCfg);

void ov07_02232730(SysTask *param0, void *ballCapSealEffPtr);

void ov07_02232730(SysTask *param0, void *ballCapSealEffPtr) {
    BallCapsuleSealEffect *ballCapSealEff = ballCapSealEffPtr;
    int i;
    int v2;
    int v3;
    SEAL *v5;

    switch (ballCapSealEff->state) {
    case 0:
        ballCapSealEff->particleNarc = NARC_New((enum NarcId)95, ballCapSealEff->heapID);

        for (i = 0; i < 2; i++) {
            v5 = CapsuleGetSealI(&ballCapSealEff->capsule, i);

            if (v5 == 0L) {
                continue;
            }

            v2 = SealOnCapsuleGetID(v5);

            if (v2 == 0) {
                continue;
            }

            if (v2 >= SEAL_ID_MAX) {
                continue;
            }

            v3 = CapsuleMenu_GetSealParticleIndex(v2);

            ballCapSealEff->seals[ballCapSealEff->sealCount] = v5;
            ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) ? (void)0 : GF_AssertFail());
            ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] = ov07_0221FEE4(ballCapSealEff->particleNarc, ballCapSealEff->heapID, v3, 0);

            if (ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) {
                ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] != 0L) ? (void)0 : GF_AssertFail());
                continue;
            }

            ballCapSealEff->sealCount++;
        }

        ballCapSealEff->state++;
        break;
    case 1:
        for (i = 2; i < 4; i++) {
            v5 = CapsuleGetSealI(&ballCapSealEff->capsule, i);

            if (v5 == 0L) {
                continue;
            }

            v2 = SealOnCapsuleGetID(v5);

            if (v2 == 0) {
                continue;
            }

            if (v2 >= (80 + 1)) {
                continue;
            }

            v3 = CapsuleMenu_GetSealParticleIndex(v2);

            ballCapSealEff->seals[ballCapSealEff->sealCount] = v5;
            ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) ? (void)0 : GF_AssertFail());
            ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] = ov07_0221FEE4(ballCapSealEff->particleNarc, ballCapSealEff->heapID, v3, 0);

            if (ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) {
                ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] != 0L) ? (void)0 : GF_AssertFail());
                continue;
            }

            ballCapSealEff->sealCount++;
        }

        ballCapSealEff->state++;
        break;
    case 2:
        for (i = 4; i < 6; i++) {
            v5 = CapsuleGetSealI(&ballCapSealEff->capsule, i);

            if (v5 == 0L) {
                continue;
            }

            v2 = SealOnCapsuleGetID(v5);

            if (v2 == 0) {
                continue;
            }

            if (v2 >= (80 + 1)) {
                continue;
            }

            v3 = CapsuleMenu_GetSealParticleIndex(v2);

            ballCapSealEff->seals[ballCapSealEff->sealCount] = v5;
            ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) ? (void)0 : GF_AssertFail());
            ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] = ov07_0221FEE4(ballCapSealEff->particleNarc, ballCapSealEff->heapID, v3, 0);

            if (ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) {
                ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] != 0L) ? (void)0 : GF_AssertFail());
                continue;
            }

            ballCapSealEff->sealCount++;
        }

        ballCapSealEff->state++;
        break;
    case 3:
        for (i = 6; i < 8; i++) {
            v5 = CapsuleGetSealI(&ballCapSealEff->capsule, i);

            if (v5 == 0L) {
                continue;
            }

            v2 = SealOnCapsuleGetID(v5);

            if (v2 == 0) {
                continue;
            }

            if (v2 >= (80 + 1)) {
                continue;
            }

            v3 = CapsuleMenu_GetSealParticleIndex(v2);

            ballCapSealEff->seals[ballCapSealEff->sealCount] = v5;
            ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) ? (void)0 : GF_AssertFail());
            ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] = ov07_0221FEE4(ballCapSealEff->particleNarc, ballCapSealEff->heapID, v3, 0);

            if (ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] == 0L) {
                ((ballCapSealEff->sealParticleSystems[ballCapSealEff->sealCount] != 0L) ? (void)0 : GF_AssertFail());
                continue;
            }

            ballCapSealEff->sealCount++;
        }

        ballCapSealEff->state = 0xFF;

        NARC_Delete(ballCapSealEff->particleNarc);
        SysTask_Destroy(param0);
        break;
    default:
        break;
    }
}
