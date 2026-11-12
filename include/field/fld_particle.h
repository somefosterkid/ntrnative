#ifndef BWPORT_FIELD_FLD_PARTICLE_H
#define BWPORT_FIELD_FLD_PARTICLE_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/tcb.h"

// fld_particle.c: the field's particle system (pokebw2's FieldParticle), which fld3d_ci.c's field effects emit from
typedef struct FieldParticle FieldParticle;

FieldParticle *func_ov021_021c1a08(HeapID heapId);
ParticleSystem *func_ov021_021c1a48(FieldParticle *particle);
void func_ov021_021c1a84(FieldParticle *particle);
void *func_ov021_021c1a98(FieldParticle *particle, u32 arcId, u32 fileId);
void func_ov021_021c1aa8(FieldParticle *particle, void *resource, BOOL immediate, TCBManager *tcbMgr);
// 1 when the particle system is gone or has no emitter left
u32 func_ov021_021c1ab4(FieldParticle *particle);
void func_ov021_021c1ad0(FieldParticle *particle);
void func_ov021_021c1aec(void);

#endif
