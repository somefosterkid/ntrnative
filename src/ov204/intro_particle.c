// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_particle.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/intro.h"
#include "gfl/particle.h"
#include "work_area.h"

// The particles of the intro

// The SPL manager's work: its emitters and particles, which hold pointers (WORK_AREA_SIZE: 21128 bytes of them on a PC
// target, 17420 on the DS)
#define INTRO_PARTICLE_BUFFER_SIZE WORK_AREA_SIZE(0x4800)

struct IntroParticle {
    HeapID heapId;
    IntroGraphic *graphic;
    void *system;
    u8 buffer[INTRO_PARTICLE_BUFFER_SIZE];
};

// The camera: its up vector, position and target
// pokebw2's sCameraVectors
static VecFx32 data_ov204_021c0b0c[3] = {
    { 0, FX32_ONE, 0 },
    { 0, 0, FX32_CONST(70) },
    { 0, 0, -FX32_ONE },
};

// pokebw2's IntroParticle_Create
IntroParticle *func_ov204_021bfc24(IntroGraphic *graphic, HeapID heapId) {
    IntroParticle *particle;
    G3DCameraProjection projection;

    func_02050b80(heapId);
    particle = GFL_HeapAllocate(heapId, sizeof(IntroParticle), TRUE, "intro_particle.c", 86);
    particle->system = func_02050c04(particle->buffer, INTRO_PARTICLE_BUFFER_SIZE, TRUE, heapId);
    particle->graphic = graphic;
    particle->heapId = heapId;
    projection.type = G3DCAM_PROJECTION_ORTHO;
    projection.param1 = FX32_CONST(4);
    projection.param2 = -FX32_CONST(4);
    projection.param3 = -FX32_CONST(3);
    projection.param4 = FX32_CONST(3);
    projection.near = FX32_ONE;
    projection.far = FX32_CONST(1024);
    projection.ndcRangeOverride = FX32_ONE;
    func_02051758(particle->system);
    func_02051648(particle->system, &projection, FX32_CONST(2), &data_ov204_021c0b0c[1], &data_ov204_021c0b0c[0],
                  &data_ov204_021c0b0c[2], particle->heapId);
    func_02051230(particle->system, func_0205121c(6, 6, particle->heapId), TRUE, FALSE);
    return particle;
}

// pokebw2's IntroParticle_Free
void func_ov204_021bfcd0(IntroParticle *particle) {
    func_02050ed4();
    GFL_HeapFree(particle);
}

// pokebw2's IntroParticle_Update
void func_ov204_021bfce0(IntroParticle *particle) {
    func_02050be8();
}

// pokebw2's IntroParticle_SetPos
void func_ov204_021bfce8(IntroParticle *particle, fx32 x, fx32 y, fx32 z) {
    VecFx32 pos = { 0, 0, 0 };

    pos.x = x;
    pos.y = y;
    pos.z = z;
    func_020515b4(particle->system, 0, &pos);
    func_020515b4(particle->system, 1, &pos);
    func_020515b4(particle->system, 2, &pos);
}
