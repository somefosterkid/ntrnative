#include "global.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

typedef struct Camera Camera;
typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    void *texAllocFunc;
    void *palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    u8 pad_32[2];
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    u32 texKeys[16];
    u32 plttKeys[16];
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

s32 sub_020154B0(ParticleSystem *particleSystem);
void Camera_SetLookAtCamUp(const VecFx32 *up, Camera *camera);
void sub_020154B8(ParticleSystem *particleSystem);
void sub_020154C4(ParticleSystem *particleSystem, SPLEmitter *emitter);
void *sub_020154D0(ParticleSystem *particleSystem);
void sub_020154D4(ParticleSystem *particleSystem, VecFx32 *up);
void sub_020154E4(ParticleSystem *particleSystem, const VecFx32 *up);

s32 sub_020154B0(ParticleSystem *particleSystem) {
    return particleSystem->manager->act_emtr_list.node_num;
}

void sub_020154B8(ParticleSystem *particleSystem) {
    SPLManager_DeleteAllEmitters(particleSystem->manager);
}

void sub_020154C4(ParticleSystem *particleSystem, SPLEmitter *emitter) {
    SPLManager_DeleteEmitter(particleSystem->manager, emitter);
}

void *sub_020154D0(ParticleSystem *particleSystem) {
    return particleSystem->heapStart;
}

void sub_020154D4(ParticleSystem *particleSystem, VecFx32 *up) {
    *up = particleSystem->cameraUp;
}

void sub_020154E4(ParticleSystem *particleSystem, const VecFx32 *up) {
    particleSystem->cameraUp = *up;
    Camera_SetLookAtCamUp(up, particleSystem->camera);
}
