#ifndef POKEHEARTGOLD_PARTICLE_SYSTEM_H
#define POKEHEARTGOLD_PARTICLE_SYSTEM_H

// ParticleSystem: merged by tools/hg/promote.py from 14 m2c local copies (2 variants, base
// src/unk_02014DA0_02014EBC.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct Camera;
struct SPLEmitter;
struct SPLManager;

typedef struct ParticleSystem {
    struct SPLManager *manager;
    void *resource;
    struct SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    void *texAllocFunc;
    void *palAllocFunc;
    struct Camera *camera;
    struct { fx32 x; fx32 y; fx32 z; } sLotoId;
    u16 cameraFov;
    u8 pad_32[0x2];
    struct { fx32 x; fx32 y; fx32 z; } cameraPos;
    struct { fx32 x; fx32 y; fx32 z; } cameraUp;
    struct { fx32 x; fx32 y; fx32 z; } cameraTarget;
    u32 texKeys[16];  // 0x58: VRAM texture keys allocated while this system uploads (sub_02015354)
    u32 plttKeys[16]; // 0x98: VRAM palette keys (sub_02015394)
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

// sizeof(ParticleSystem) == 0xdc

#endif // POKEHEARTGOLD_PARTICLE_SYSTEM_H
