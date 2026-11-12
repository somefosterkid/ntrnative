// Black's code at ov021 0x021c1a08-0x021c1a48. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/fld_particle.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/tcb.h"
#include "work_area.h"


struct FieldParticle {
    u32 unk0;
    u16 heapId;
    void *work;
    ParticleSystem *system;
};

FieldParticle *func_ov021_021c1a08(HeapID heapId) {
    FieldParticle *particle;

    particle = GFL_HeapAllocate(heapId, sizeof(FieldParticle), 1, "fld_particle.c", 0x2e);
    particle->heapId = heapId;
    func_02050b80(heapId);
    // the SPL manager's work, whose emitters and particles hold pointers
    particle->work = GFL_HeapAllocate(heapId, WORK_AREA_SIZE(0x4800), 1, "fld_particle.c", 0x34);
    return particle;
}

ParticleSystem *func_ov021_021c1a48(FieldParticle *particle) {
    ParticleSystem *system;
    G3DCamera *cam;
    fx32 afStack_18 [2];

    system = func_02050c04(particle->work, WORK_AREA_SIZE(0x4800), 1, (u32)particle->heapId);
    particle->system = system;
    cam = func_02051788(system);
    afStack_18[1] = 0x1000;
    afStack_18[0] = 0x384000;
    GFL_G3DCameraSetProjectionZNear(cam, afStack_18 + 1);
    GFL_G3DCameraSetProjectionZFar(cam, afStack_18);
    return particle->system;
}

void func_ov021_021c1a84(FieldParticle *particle) {
    if (particle->system != NULL) {
        func_02050db8(particle->system);
        particle->system = NULL;
    }
}

void *func_ov021_021c1a98(FieldParticle *particle, u32 arcId, u32 fileId) {
    return func_0205121c(arcId, fileId, particle->heapId);
}

void func_ov021_021c1aa8(FieldParticle *particle, void *resource, BOOL immediate, TCBManager *tcbMgr) {
    func_02051230(particle->system, resource, immediate, tcbMgr);
}

u32 func_ov021_021c1ab4(FieldParticle *particle) {
    if (particle->system == NULL) {
        return 1;
    }
    if (func_02051614(particle->system) == 0) {
        return 1;
    }
    return 0;
}

void func_ov021_021c1ad0(FieldParticle *particle) {
    func_ov021_021c1a84(particle);
    func_02050ed4();
    GFL_HeapFree(particle->work);
    GFL_HeapFree(particle);
}

void func_ov021_021c1aec(void) {
    func_02050be8();
}
