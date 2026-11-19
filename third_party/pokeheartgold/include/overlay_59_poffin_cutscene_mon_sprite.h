#ifndef POKEHEARTGOLD_OVERLAY_59_POFFIN_CUTSCENE_MON_SPRITE_H
#define POKEHEARTGOLD_OVERLAY_59_POFFIN_CUTSCENE_MON_SPRITE_H

// PoffinCutsceneMonSprite: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_59_0223BAE8_0223C5C4_0223C5C4.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct Camera;
struct ParticleSystem;
struct PokemonSprite;

typedef struct PoffinCutsceneMonSprite {
    u8 pad_0[8];
    struct Camera *camera;
    void *spriteMan;
    struct { u16 narcID; u16 character; u16 palette; u16 spindaSpots; u8 dummy; u8 unk_C_pad[0x3]; u32 personality; } unused;
    struct PokemonSprite *sprite;
    struct ParticleSystem *unk_24;
} PoffinCutsceneMonSprite;

// sizeof(PoffinCutsceneMonSprite) == 0x28

#endif // POKEHEARTGOLD_OVERLAY_59_POFFIN_CUTSCENE_MON_SPRITE_H
