// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/demo_particle.h: the lines in use so
// far.
#ifndef BWPORT_DEMO_DEMO_PARTICLE_H
#define BWPORT_DEMO_DEMO_PARTICLE_H

#include "types.h"
#include "struct_decls.h"
#include "work_area.h"

// The SPL manager's work: its emitters and particles hold pointers (WORK_AREA_SIZE: docs/port-boot.md)
#define DEMO_PARTICLE_BUFFER_SIZE WORK_AREA_SIZE(0x4800)
#define DEMO_PARTICLE_UNIT_COUNT 1

// Particles to emit at a frame, from an emitter of a unit
struct DemoParticleEvent {
    u16 frame;
    u8 unit;
    u8 emitter;
};

// A particle system with its resource
struct DemoParticleUnit {
    u8 buffer[DEMO_PARTICLE_BUFFER_SIZE];
    void *system;
    u8 resourceCount;
};

struct DemoParticle {
    u16 frame;
    u16 index;
    u16 count;
    const DemoParticleEvent *events;
    DemoParticleUnit units[DEMO_PARTICLE_UNIT_COUNT];
    BOOL active;
    // Stops the first unit when it reaches 0
    s32 stopTimer;
};

// Black 2's, not checked against Black
// The archive and file of a unit's resource
struct DemoParticleResource {
    u32 arcId;
    u32 fileId;
};

#endif // BWPORT_DEMO_DEMO_PARTICLE_H
