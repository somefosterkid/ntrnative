// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/particle.h.
#ifndef BWPORT_GFL_PARTICLE_H
#define BWPORT_GFL_PARTICLE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nnsys/gfd.h"
#include "spl/spl.h"

// Particle systems (particle.c): up to 16 SPL managers, each with its own work memory, resource file and camera. None
// of these functions has a name yet

#define PARTICLE_SYSTEM_MAX 16

// How a system's VRAM is given back when it is freed: the frame managers' state from before its resource was loaded,
// or each key the linked-list managers gave it
enum {
    PARTICLE_VRAM_TEX_FRM = 1 << 0,
    PARTICLE_VRAM_TEX_LNK = 1 << 1,
    PARTICLE_VRAM_PLTT_FRM = 1 << 2,
    PARTICLE_VRAM_PLTT_LNK = 1 << 3,
};

#define PARTICLE_VRAM_KEY_MAX 64

typedef struct {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastEmitter;
    void *workStart;
    void *work;
    void *workEnd;
    SPLTexVRAMAllocFunc texAllocFunc;
    SPLPalVRAMAllocFunc palAllocFunc;
    G3DCamera *camera;
    VecFx32 unk24;
    u16 cameraFov;
    // Whether the resource is someone else's, which freeing the system leaves
    u16 extResource : 1;
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    union {
        NNSGfdFrmTexVramState texState;
        NNSGfdTexKey texKeys[PARTICLE_VRAM_KEY_MAX];
    };
    union {
        NNSGfdFrmPlttVramState plttState;
        NNSGfdPlttKey plttKeys[PARTICLE_VRAM_KEY_MAX];
    };
    u8 vramRelease;
    u8 unk259;
    u8 id;
    u8 cameraProjection;
    u16 texKeyCount;
    u16 plttKeyCount;
} ParticleSystem;

// Sets up and frees the table of systems
void func_02050b80(HeapID heapId);
void func_02050ed4(void);
// Updates and draws every system
void func_02050be8(void);
// func_02050c30 with polygon IDs 5, 6 and 0x3f
ParticleSystem *func_02050c04(void *work, u32 size, BOOL camera, u32 heapId);
// A system working in size bytes of work, its particles drawn with the given polygon IDs, or NULL when all 16 are in
// use
ParticleSystem *func_02050c30(void *work, u32 size, BOOL camera, u32 fixPolyID, u32 minPolyID, u32 maxPolyID,
                              u32 heapId);
void func_02050db8(ParticleSystem *system);
void *func_0205121c(u32 arcId, u32 fileId, u32 heapId);
// Gives the system a resource file, which it uploads now or, through tcbMgr, at the next vertical blank
void func_02051230(ParticleSystem *system, void *resource, BOOL immediate, TCBManager *tcbMgr);
// The same, with a resource the system does not own
void func_02051360(ParticleSystem *system, void *resource, BOOL immediate, TCBManager *tcbMgr);
void func_02051380(ParticleSystem *system, void *resource);
void func_02051394(ParticleSystem *system);
void func_02051414(ParticleSystem *system);
void func_020514f4(ParticleSystem *system);
int func_02051504(void);
int func_02051534(void);
int func_02051574(void);
SPLEmitter *func_020515b4(ParticleSystem *system, int resourceId, const VecFx32 *pos);
// Creates an emitter, calling callback with param readable through func_02051774, or returns -1 when the system
// has 24 emitters
SPLEmitter *func_020515cc(ParticleSystem *system, int resourceId, SPLEmitterCallback callback, void *param);
int func_02051614(ParticleSystem *system);
void func_02051620(ParticleSystem *system);
void func_02051630(ParticleSystem *system, SPLEmitter *emitter);
void *func_02051640(ParticleSystem *system);
// Sets the camera that the particles are drawn with; a NULL projection or vector is the default
void func_02051648(ParticleSystem *system, G3DCameraProjection *projection, u16 fov, const VecFx32 *position,
                   const VecFx32 *upVector, const VecFx32 *target, u32 heapId);
void func_02051758(ParticleSystem *system);
void *func_02051774(void);
G3DCamera *func_02051788(ParticleSystem *system);
void func_02051790(ParticleSystem *system, VecFx32 *pos);
void func_020517a4(ParticleSystem *system, const VecFx32 *pos);

// An emitter's settings
void func_020517b4(SPLEmitter *emitter, VecFx16 *axis);
fx32 func_020517d0(SPLEmitter *emitter);
void func_020517d8(SPLEmitter *emitter, fx32 radius);
fx32 func_020517e0(SPLEmitter *emitter);
void func_020517e8(SPLEmitter *emitter, fx32 length);
fx16 func_020517f0(SPLEmitter *emitter);
void func_02051800(SPLEmitter *emitter, fx32 amplifier);
fx16 func_02051808(SPLEmitter *emitter);
void func_02051818(SPLEmitter *emitter, fx32 amplifier);
fx16 func_02051820(SPLEmitter *emitter);
u16 func_02051830(SPLEmitter *emitter);
void func_02051838(SPLEmitter *emitter, const VecFx32 *pos);
void func_02051880(SPLEmitter *emitter, const VecFx16 *axis);
void func_0205189c(SPLEmitter *emitter, fx32 scale);
void func_020518a4(SPLEmitter *emitter, u16 lifeTime);
void func_020519c4(SPLEmitter *emitter, const VecFx32 *target);
void func_020519e4(SPLEmitter *emitter, const u16 *axis);
void func_02051a04(SPLEmitter *emitter, const VecFx32 *target);
void func_02051a24(SPLEmitter *emitter, SPLEmitterUpdateCallback callback);
void func_02051a2c(SPLEmitter *emitter, void *data);
void *func_02051a34(SPLEmitter *emitter);

// How many emitters a resource file defines
u16 func_02051b04(const void *resource);
// The character base of a background layer of the main (0 to 3) or sub (4 to 7) engine
void *func_02051b0c(u32 layer);

#endif // BWPORT_GFL_PARTICLE_H
