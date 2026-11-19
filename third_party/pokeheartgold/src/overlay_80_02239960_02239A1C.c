#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "heap.h"
#include "overlay_80_02239960.h"
#include "unk_02026E30.h"
#include "particle_system.h"

struct ParticleSystem;
typedef struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
} FrontierParticleSystem;
void ov80_02239A98(ParticleSystem *ps);
int sub_02015420(void);
int sub_0201543C(void);
int sub_02015460(void);
typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);
typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);
s32 sub_020154B0(ParticleSystem *particleSystem);
void sub_02014EBC(ParticleSystem *particleSystem);
void *sub_020154D0(ParticleSystem *particleSystem);
void sub_02015354(NNSGfdTexKey key);
void sub_02015394(NNSGfdPlttKey key);

void ov80_02239A1C(FrontierParticleSystem *fps, int index);
ParticleSystem *ov80_02239A60(FrontierParticleSystem *fps, int index);
BOOL ov80_02239A74(FrontierParticleSystem *fps);
void ov80_02239A98(ParticleSystem *ps);

void ov80_02239A1C(FrontierParticleSystem *fps, int index) {
    ((fps->particleSys[index] != 0L) ? (void)0 : GF_AssertFail());

    ov80_02239A98(fps->particleSys[index]);
    fps->particleSys[index] = 0L;
}

BOOL ov80_02239A38(void) {
    Thunk_G3X_Reset();

    if (sub_02015420() == 0) {
        return 0;
    }

    if (sub_0201543C() > 0) {
        Thunk_G3X_Reset();
    }

    sub_02015460();

    return 1;
}

ParticleSystem *ov80_02239A60(FrontierParticleSystem *fps, int index) {
    ((fps->particleSys[index] != 0L) ? (void)0 : GF_AssertFail());
    return fps->particleSys[index];
}

BOOL ov80_02239A74(FrontierParticleSystem *fps) {
    for (int i = 0; i < 8; i++) {
        if (fps->particleSys[i] != 0L && sub_020154B0(fps->particleSys[i]) > 0) {
            return 0;
        }
    }

    return 1;
}

void ov80_02239A98(ParticleSystem *ps) {
    void *heap = sub_020154D0(ps);

    sub_02014EBC(ps);
    Heap_Free(heap);
}

u32 ov80_02239AB0(u32 param0, BOOL param1) {
    NNSGfdTexKey v0;

    v0 = NNS_GfdAllocTexVram(param0, param1, 0);
    ((v0 != (u32)0x0) ? (void)0 : GF_AssertFail());
    sub_02015354(v0);

    return NNS_GfdGetTexKeyAddr(v0);
}

u32 ov80_02239AD4(u32 param0, BOOL param1) {
    NNSGfdPlttKey v0;
    u32 v1;

    v0 = NNS_GfdAllocPlttVram(param0, param1, NNS_GFD_ALLOC_FROM_LOW);

    if (v0 == (u32)0x0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    sub_02015394(v0);

    v1 = NNS_GfdGetPlttKeyAddr(v0);

    return v1;
}
