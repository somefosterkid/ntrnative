#ifndef POKEHEARTGOLD_PARTICLE_SYSTEM_GLOBALS_H
#define POKEHEARTGOLD_PARTICLE_SYSTEM_GLOBALS_H

// One BSS block at 0x021D10A0 (asm/unk_02014DA0.s: _021D10A0 .space 8 + _021D10A8 .space 0x40 = 0x48 bytes, the size of
// this struct on the DS). mwcc addressed all of it from one base, so m2c read the per-slot systems as _021D10A0[2..17].
// On 64-bit every member is a pointer: keep it one object.
struct ParticleSystem;

typedef struct ParticleSystemGlobals {
    struct ParticleSystem *current;     // 0x00: the system uploading textures/palettes (sub_02015300); VRAM key callbacks read it
    void *unk4;                         // 0x04: set/cleared by sub_02015494, read by sub_02015504
    struct ParticleSystem *systems[16]; // 0x08: per-slot systems (sub_02014DB4); allocator callback k uses systems[k]
} ParticleSystemGlobals;

extern ParticleSystemGlobals _021D10A0;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the asm object is DS-sized (and only 4-aligned), so the host-layout object is defined in C
// (src/unk_02014DA0_02014DA0.c) and the slot array label is its member.
#define _021D10A8 (_021D10A0.systems)
#else
// DS: the original code of sub_02014DB4 / sub_02014EBC / sub_0201543C addresses the slot array by its own label,
// _021D10A0 + 8 (the second half of the asm object).
extern struct ParticleSystem *_021D10A8[16];
// DS layout check: 4-byte pointers (the PC build's are wider)
typedef char ParticleSystemGlobals_size_check[(sizeof(ParticleSystemGlobals) == 0x48) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_PARTICLE_SYSTEM_GLOBALS_H
