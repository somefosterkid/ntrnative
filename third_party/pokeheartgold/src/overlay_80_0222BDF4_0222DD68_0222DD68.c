#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "unk_02014DA0.h"
#include "particle_system.h"

typedef struct FrontierParticleSystem FrontierParticleSystem;

struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
};

enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};

ParticleSystem *ov80_022399A4(FrontierParticleSystem *fps, int index, int narcIdx, enum CameraProjection projection);

void *sub_0209680C(void *);

s32 FrtCmd_065(FrontierScriptContext *ctx);

s32 FrtCmd_065(FrontierScriptContext *ctx) {
    u32 v1;
    u32 v2;
    void *v0;

    v0 = sub_0209680C(ctx->frontierSystem->unk0);
    v1 = FrontierScript_ReadVar(ctx);
    v2 = FrontierScript_ReadVar(ctx);
    ov80_022399A4(*(FrontierParticleSystem **)((u8 *)v0 + 0x10), (s32)v1, (s32)v2, (enum CameraProjection)FrontierScript_ReadVar(ctx));
    return 0;
}
